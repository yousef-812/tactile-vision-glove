import sys
import cv2
import numpy as np
import time

# Configure UTF-8 encoding for console output
if hasattr(sys.stdout, 'reconfigure'):
    sys.stdout.reconfigure(encoding='utf-8')

class TactileVisionCameraSystem:
    """
    TactileVision - Real-time Single Camera 3-Zone Spatial AI System
    Simulates Left / Center / Right obstacle detection using webcam.
    Maps detected obstacles to 5 haptic finger motors.
    """

    def __init__(self, camera_index=0):
        self.cap = cv2.VideoCapture(camera_index)
        self.cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
        self.cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)

        # Previous center density for velocity detection
        self.prev_center_density = 0.0
        self.prev_time = time.time()

        # Motor states
        self.motors = {
            "THUMB_LEFT":       {"label": "Thumb (Left)",      "color": (50, 50, 255),   "state": 0},
            "INDEX_RIGHT":      {"label": "Index (Right)",     "color": (0, 200, 255),   "state": 0},
            "MIDDLE_OVERHEAD":  {"label": "Middle (Overhead)", "color": (200, 0, 255),   "state": 0},
            "RING_GROUND":      {"label": "Ring (Ground)",     "color": (0, 220, 180),   "state": 0},
            "PALM_CENTER":      {"label": "Palm (Collision!)", "color": (0, 0, 255),     "state": 0},
        }

    def compute_zone_threat(self, zone_gray):
        """
        Computes threat level (0.0 - 1.0) for a zone.
        Uses edge density: more edges = more objects = closer threat.
        """
        blurred = cv2.GaussianBlur(zone_gray, (9, 9), 0)
        edges = cv2.Canny(blurred, 30, 100)
        density = np.sum(edges > 0) / float(edges.size)
        return float(np.clip(density * 6.0, 0.0, 1.0))

    def compute_velocity(self, current_density):
        """
        Measures how fast the center density is increasing (approaching object).
        """
        now = time.time()
        dt = now - self.prev_time
        if dt < 0.05:
            return 0.0
        velocity = (current_density - self.prev_center_density) / dt
        self.prev_center_density = current_density
        self.prev_time = now
        return velocity

    def analyze_frame(self, frame):
        h, w = frame.shape[:2]
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

        # Divide frame into 3 horizontal zones (Left / Center / Right)
        left_zone   = gray[:, :int(w * 0.33)]
        center_zone = gray[:, int(w * 0.33):int(w * 0.67)]
        right_zone  = gray[:, int(w * 0.67):]

        # Also divide center vertically into top (overhead) and bottom (ground)
        top_zone    = gray[:int(h * 0.40), int(w * 0.33):int(w * 0.67)]
        bottom_zone = gray[int(h * 0.60):, int(w * 0.33):int(w * 0.67)]

        # Compute threat levels per zone (0.0 = safe, 1.0 = danger)
        threat_left    = self.compute_zone_threat(left_zone)
        threat_center  = self.compute_zone_threat(center_zone)
        threat_right   = self.compute_zone_threat(right_zone)
        threat_top     = self.compute_zone_threat(top_zone)
        threat_bottom  = self.compute_zone_threat(bottom_zone)
        velocity       = self.compute_velocity(threat_center)

        # Reset motors
        for m in self.motors.values():
            m["state"] = 0

        # -------- Haptic Decision Engine --------
        THRESHOLD = 0.30  # Minimum threat to trigger motor

        # 1. FAST APPROACH DETECTION (velocity > 0.8 = object coming fast)
        if velocity > 0.8 and threat_center > THRESHOLD:
            self.motors["PALM_CENTER"]["state"] = 2  # EMERGENCY PULSE
            self.motors["MIDDLE_OVERHEAD"]["state"] = 2
        else:
            # 2. Left obstacle
            if threat_left > THRESHOLD:
                self.motors["THUMB_LEFT"]["state"] = 1

            # 3. Right obstacle
            if threat_right > THRESHOLD:
                self.motors["INDEX_RIGHT"]["state"] = 1

            # 4. Overhead obstacle (top-center)
            if threat_top > THRESHOLD:
                self.motors["MIDDLE_OVERHEAD"]["state"] = 1

            # 5. Ground / steps (bottom-center)
            if threat_bottom > THRESHOLD:
                self.motors["RING_GROUND"]["state"] = 1

            # 6. Immediate front collision (high center threat)
            if threat_center > 0.65:
                self.motors["PALM_CENTER"]["state"] = 1

        return {
            "left": threat_left,
            "center": threat_center,
            "right": threat_right,
            "top": threat_top,
            "bottom": threat_bottom,
            "velocity": velocity
        }

    def draw_hud(self, frame, threats):
        h, w = frame.shape[:2]
        overlay = frame.copy()

        # ---- Zone Dividers ----
        cv2.line(overlay, (int(w*0.33), 0), (int(w*0.33), h), (255, 255, 255), 2)
        cv2.line(overlay, (int(w*0.67), 0), (int(w*0.67), h), (255, 255, 255), 2)
        cv2.line(overlay, (int(w*0.33), int(h*0.40)), (int(w*0.67), int(h*0.40)), (0, 220, 220), 1)
        cv2.line(overlay, (int(w*0.33), int(h*0.60)), (int(w*0.67), int(h*0.60)), (0, 220, 220), 1)

        # ---- Zone Threat Color Fills ----
        zones = [
            (0, int(w*0.33), (0, 0, 255), threats["left"]),
            (int(w*0.33), int(w*0.67), (255, 100, 0), threats["center"]),
            (int(w*0.67), w, (0, 200, 255), threats["right"]),
        ]
        for x1, x2, color, threat in zones:
            alpha = threat * 0.45
            zone_overlay = overlay.copy()
            cv2.rectangle(zone_overlay, (x1, 0), (x2, h), color, -1)
            cv2.addWeighted(zone_overlay, alpha, overlay, 1 - alpha, 0, overlay)

        # ---- Zone Labels & Threat Bars ----
        zone_labels = [
            (int(w*0.08), "LEFT", threats["left"]),
            (int(w*0.44), "CENTER", threats["center"]),
            (int(w*0.76), "RIGHT", threats["right"]),
        ]
        for x, label, threat in zone_labels:
            bar_color = (0, int(255*(1-threat)), int(255*threat)) if threat < 0.65 else (0, 0, 255)
            cv2.putText(overlay, label, (x - 20, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (255,255,255), 2)
            cv2.rectangle(overlay, (x - 20, 40), (x - 20 + int(100 * threat), 55), bar_color, -1)
            cv2.rectangle(overlay, (x - 20, 40), (x + 80, 55), (200, 200, 200), 1)

        # ---- Velocity Alert ----
        if threats["velocity"] > 0.8:
            cv2.putText(overlay, "!! FAST APPROACH !!",
                        (int(w*0.25), int(h*0.5)),
                        cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0, 0, 255), 3)

        # ---- Motor HUD Box (Bottom-Left) ----
        bx, by, bw, bh = 10, h - 220, 260, 215
        cv2.rectangle(overlay, (bx, by), (bx+bw, by+bh), (20, 20, 20), -1)
        cv2.rectangle(overlay, (bx, by), (bx+bw, by+bh), (100, 100, 100), 2)
        cv2.putText(overlay, "HAPTIC GLOVE MOTORS", (bx+10, by+25),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 1)

        motor_list = list(self.motors.items())
        for i, (key, motor) in enumerate(motor_list):
            my = by + 50 + i * 33
            state = motor["state"]
            if state == 2:
                dot_color = (0, 0, 255)
                status_text = "!! EMERGENCY !!"
            elif state == 1:
                dot_color = motor["color"]
                status_text = "ON  |||"
            else:
                dot_color = (60, 60, 60)
                status_text = "off"

            cv2.circle(overlay, (bx + 20, my), 10, dot_color, -1)
            cv2.putText(overlay, motor["label"], (bx + 38, my + 5),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.42, (220, 220, 220), 1)
            cv2.putText(overlay, status_text, (bx + 175, my + 5),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.38,
                        (0, 0, 255) if state == 2 else (0, 255, 120) if state == 1 else (100, 100, 100), 1)

        # Final blend
        return cv2.addWeighted(overlay, 0.88, frame, 0.12, 0)

    def start(self):
        if not self.cap.isOpened():
            print("[ERROR] Cannot open webcam!")
            return

        print("[+] TactileVision 3-Zone Camera AI System Started")
        print("[+] Press 'Q' or 'ESC' to quit")
        prev_time = time.time()

        while True:
            ret, frame = self.cap.read()
            if not ret:
                break

            frame = cv2.flip(frame, 1)  # Mirror display
            threats = self.analyze_frame(frame)
            hud = self.draw_hud(frame, threats)

            # FPS
            now = time.time()
            fps = 1.0 / max(now - prev_time, 1e-5)
            prev_time = now
            cv2.putText(hud, f"FPS: {int(fps)}", (10, 25),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 100), 2)

            cv2.imshow("TactileVision - 3-Zone AI Camera System (Press Q to quit)", hud)

            key = cv2.waitKey(1) & 0xFF
            if key in [27, ord('q'), ord('Q')]:
                break

        self.cap.release()
        cv2.destroyAllWindows()
        print("[+] TactileVision System Stopped.")


if __name__ == "__main__":
    system = TactileVisionCameraSystem(camera_index=0)
    system.start()
