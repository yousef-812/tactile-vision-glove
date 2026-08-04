import sys
import time
import numpy as np
from haptic_translator import HapticTranslator

# Ensure UTF-8 output encoding for Windows terminals
if hasattr(sys.stdout, 'reconfigure'):
    sys.stdout.reconfigure(encoding='utf-8')

def run_simulation():
    print("=" * 60)
    print("[+] TactileVision AI Haptic Motor Simulator Initialized...")
    print("=" * 60)
    
    translator = HapticTranslator()
    
    # Simulate a sequence of obstacle scenarios
    scenarios = [
        ("Scenario 1: Clear path ahead", np.full((480, 640), 4.0)),
        ("Scenario 2: Low tree branch overhead at 1.1 meters", np.full((480, 640), 3.5)),
        ("Scenario 3: Wall on the left at 0.8 meters", np.full((480, 640), 3.5)),
        ("Scenario 4: Immediate collision hazard right in front! (0.4m)", np.full((480, 640), 3.5)),
    ]
    
    # Inject specific distance values into scenario depth arrays
    scenarios[1][1][50:150, 250:400] = 1.1  # Overhead branch
    scenarios[2][1][:, :200] = 0.8          # Left wall
    scenarios[3][1][200:300, 250:400] = 0.4  # Immediate front danger

    for title, depth_map in scenarios:
        print(f"\n[*] {title}")
        pwms = translator.process_depth_map(depth_map)
        for motor, pwm in pwms.items():
            bar = "#" * (pwm // 15)
            print(f"   - Motor [{motor:15s}]: PWM = {pwm:3d} | {bar}")
        time.sleep(0.2)

    print("\n[SUCCESS] Simulation completed successfully!")

if __name__ == "__main__":
    run_simulation()

