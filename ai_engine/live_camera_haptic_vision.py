import cv2
import numpy as np
import time
import sys
from haptic_translator import HapticTranslator

# Configure UTF-8 encoding for console output
if hasattr(sys.stdout, 'reconfigure'):
    sys.stdout.reconfigure(encoding='utf-8')

class LiveHapticVisionSystem:
    """
    Real-time Camera Vision Pipeline with Interactive Haptic Glove HUD Simulator.
    Allows testing the entire AI system using just a PC webcam without physical hardware.
    """
    def __init__(self, camera_index=0):
        self.cap = cv2.VideoCapture(camera_index)
        self.translator = HapticTranslator(max_distance_m=3.5, min_distance_m=0.5)
        
        # Color definitions for HUD (BGR)
        self.COLOR_SAFE = (0, 230, 115)     # Green
        self.COLOR_WARN = (0, 204, 255)     # Yellow
        self.COLOR_DANGER = (50, 50, 255)   # Red
        self.COLOR_BG = (20, 20, 20)        # Dark Slate
        
    def estimate_pseudo_depth(self, frame):
        """
        Calculates a pseudo-depth/obstacle map using frame luminance, contours, and optical contrast.
        Higher intensity/proximity implies closer obstacles.
        """
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        blurred = cv2.GaussianBlur(gray, (21, 21), 0)
        
        # Invert normalized gradient to simulate distance in meters (0.5m = close, 3.5m = far)
        norm_grad = cv2.Laplacian(blurred, cv2.CV_64F)
        abs_grad = cv2.convertScaleAbs(norm_grad)
        
        # Map gradient density to distance range [0.5m - 3.5m]
        depth_map = 3.5 - (abs_grad / 255.0) * 3.0
        depth_map = np.clip(depth_map, 0.5, 3.5)
        return depth_map

    def draw_haptic_hud(self, frame, pwms):
        """
        Renders a futuristic Haptic Glove Overlay showing real-time motor vibration states.
        """
        h, w, _ = frame.shape
        overlay = frame.copy()
        
        # Draw Vision Partition Grid Lines
        cv2.line(overlay, (int(w * 0.33), 0), (int(w * 0.33), h), (255, 255, 255), 1, cv2.LINE_AA)
        cv2.line(overlay, (int(w * 0.67), 0), (int(w * 0.67), h), (255, 255, 255), 1, cv2.LINE_AA)
        cv2.line(overlay, (int(w * 0.33), int(h * 0.40)), (int(w * 0.67), int(h * 0.40)), (0, 255, 255), 1, cv2.LINE_AA)
        cv2.line(overlay, (int(w * 0.33), int(h * 0.60)), (int(w * 0.67), int(h * 0.60)), (0, 255, 255), 1, cv2.LINE_AA)

        # Region Labels
        cv2.putText(overlay, "LEFT (THUMB)", (20, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 1)
        cv2.putText(overlay, "RIGHT (INDEX)", (int(w * 0.68), 30), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 1)
        cv2.putText(overlay, "OVERHEAD (MIDDLE)", (int(w * 0.35), 30), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 255), 1)
        cv2.putText(overlay, "GROUND (RING)", (int(w * 0.35), int(h * 0.95)), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 255), 1)

        # Render Glove HUD Widget (Bottom Left Box)
        box_w, box_h = 240, 220
        bx, by = 20, h - box_h - 20
        cv2.rectangle(overlay, (bx, by), (bx + box_w, by + box_h), self.COLOR_BG, -1)
        cv2.rectangle(overlay, (bx, by), (bx + box_w, by + box_h), (100, 100, 100), 2)
        cv2.putText(overlay, "HAPTIC GLOVE STATE", (bx + 15, by + 25), cv2.FONT_HERSHEY_SIMPLEX, 0.55, (255, 255, 255), 2)

        # Motor Locations inside Widget
        motors_ui = [
            ("Thumb (Left)",      pwms["THUMB_LEFT"],     (bx + 30,  by + 70)),
            ("Index (Right)",     pwms["INDEX_RIGHT"],    (bx + 130, by + 70)),
            ("Middle (Overhead)", pwms["MIDDLE_OVERHEAD"],(bx + 80,  by + 110)),
            ("Ring (Ground)",     pwms["RING_GROUND"],    (bx + 80,  by + 150)),
            ("Palm (Collision)",  pwms["PALM_CENTER"],    (bx + 80,  by + 190)),
        ]

        for name, pwm, (mx, my) in motors_ui:
            # Color transition based on PWM (Green -> Yellow -> Red)
            if pwm > 180:
                color = self.COLOR_DANGER
            elif pwm > 80:
                color = self.COLOR_WARN
            else:
                color = self.COLOR_SAFE
                
            radius = int(8 + (pwm / 255.0) * 8)
            cv2.circle(overlay, (mx, my), radius, color, -1)
            cv2.circle(overlay, (mx, my), radius + 2, (255, 255, 255), 1)
            cv2.putText(overlay, f"{pwm}", (mx + 15, my + 5), cv2.FONT_HERSHEY_SIMPLEX, 0.4, (220, 220, 220), 1)

        # Blend Overlay
        return cv2.addWeighted(overlay, 0.85, frame, 0.15, 0)

    def start(self):
        if not self.cap.isOpened():
            print("[ERROR] Cannot access webcam. Running headless test mode...")
            return

        print("[+] Starting Live Camera Haptic Vision Simulator...")
        print("[+] Press 'q' or 'ESC' on the video window to exit.")

        prev_time = time.time()
        
        while True:
            ret, frame = self.cap.read()
            if not ret:
                print("[WARN] Failed to grab camera frame.")
                break
                
            frame = cv2.flip(frame, 1) # Mirror display
            depth_map = self.estimate_pseudo_depth(frame)
            pwms = self.translator.process_depth_map(depth_map)
            
            # FPS calculation
            curr_time = time.time()
            fps = 1.0 / (curr_time - prev_time + 1e-5)
            prev_time = curr_time
            
            hud_frame = self.draw_haptic_hud(frame, pwms)
            cv2.putText(hud_frame, f"FPS: {int(fps)}", (20, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 0), 2)
            
            cv2.imshow("TactileVision - Live AI Haptic Simulator", hud_frame)
            
            key = cv2.waitKey(1) & 0xFF
            if key in [27, ord('q')]:
                break
                
        self.cap.release()
        cv2.destroyAllWindows()

if __name__ == "__main__":
    app = LiveHapticVisionSystem()
    app.start()
