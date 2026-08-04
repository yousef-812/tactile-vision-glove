import numpy as np

class HapticTranslator:
    """
    Translates 3D Depth Map regions into 8-Channel Haptic Motor PWM intensities (0-255).
    """
    def __init__(self, max_distance_m=4.0, min_distance_m=0.5):
        self.max_dist = max_distance_m
        self.min_dist = min_distance_m
        
    def distance_to_pwm(self, distance_m: float) -> int:
        """
        Converts distance in meters to 8-bit PWM value (0 to 255).
        """
        if distance_m <= self.min_dist:
            return 255
        elif distance_m >= self.max_dist:
            return 0
        else:
            ratio = 1.0 - ((distance_m - self.min_dist) / (self.max_dist - self.min_dist))
            return int(ratio * 255)

    def process_depth_map(self, depth_map_meters: np.ndarray) -> dict:
        """
        Processes a depth map array and returns PWM intensities for each finger/palm motor.
        """
        h, w = depth_map_meters.shape
        
        # Divide depth map into 5 key spatial zones
        left_zone   = depth_map_meters[:, :int(w * 0.33)]
        right_zone  = depth_map_meters[:, int(w * 0.67):]
        top_zone    = depth_map_meters[:int(h * 0.40), int(w * 0.33):int(w * 0.67)]
        bottom_zone = depth_map_meters[int(h * 0.60):, int(w * 0.33):int(w * 0.67)]
        center_zone = depth_map_meters[int(h * 0.30):int(h * 0.70), int(w * 0.33):int(w * 0.67)]

        # Calculate minimum distance (closest obstacle) in each region
        dist_left   = np.min(left_zone) if left_zone.size > 0 else self.max_dist
        dist_right  = np.min(right_zone) if right_zone.size > 0 else self.max_dist
        dist_top    = np.min(top_zone) if top_zone.size > 0 else self.max_dist
        dist_bottom = np.min(bottom_zone) if bottom_zone.size > 0 else self.max_dist
        dist_center = np.min(center_zone) if center_zone.size > 0 else self.max_dist

        # Map to PWM motor array
        motor_pwms = {
            "THUMB_LEFT":       self.distance_to_pwm(dist_left),
            "INDEX_RIGHT":      self.distance_to_pwm(dist_right),
            "MIDDLE_OVERHEAD":  self.distance_to_pwm(dist_top),
            "RING_GROUND":      self.distance_to_pwm(dist_bottom),
            "PALM_CENTER":      self.distance_to_pwm(dist_center),
        }
        
        return motor_pwms

if __name__ == "__main__":
    translator = HapticTranslator()
    # Test simulation
    fake_depth = np.full((480, 640), 3.0)
    fake_depth[50:150, 250:400] = 1.2 # Fake overhead obstacle (Middle finger trigger)
    pwms = translator.process_depth_map(fake_depth)
    print("Haptic PWM Matrix output:", pwms)
