#include <Arduino.h>

// TactileVision ESP32-S3 Haptic Glove Driver Firmware
// Pin Definitions for 5 Haptic Vibration Motors (PWM Controlled)

#define MOTOR_THUMB_LEFT      4   // PWM Channel 0
#define MOTOR_INDEX_RIGHT     5   // PWM Channel 1
#define MOTOR_MIDDLE_OVERHEAD 6   // PWM Channel 2
#define MOTOR_RING_GROUND     7   // PWM Channel 3
#define MOTOR_PALM_CENTER     15  // PWM Channel 4

void setup() {
    Serial.begin(115200);
    Serial.println("🖐️ TactileVision ESP32-S3 Glove Firmware Booting...");

    // Configure PWM channels (50Hz frequency, 8-bit resolution: 0-255)
    ledcSetup(0, 50, 8); ledcAttachPin(MOTOR_THUMB_LEFT, 0);
    ledcSetup(1, 50, 8); ledcAttachPin(MOTOR_INDEX_RIGHT, 1);
    ledcSetup(2, 50, 8); ledcAttachPin(MOTOR_MIDDLE_OVERHEAD, 2);
    ledcSetup(3, 50, 8); ledcAttachPin(MOTOR_RING_GROUND, 3);
    ledcSetup(4, 50, 8); ledcAttachPin(MOTOR_PALM_CENTER, 4);

    Serial.println("✅ Haptic Motor Matrix PWM Channels Initialized.");
}

void loop() {
    // Read serial command packet from AI Engine (Format: "PWM:thumb,index,middle,ring,palm\n")
    if (Serial.available()) {
        String data = Serial.readStringUntil('\n');
        if (data.startsWith("PWM:")) {
            int t, i, m, r, p;
            sscanf(data.c_str(), "PWM:%d,%d,%d,%d,%d", &t, &i, &m, &r, &p);
            
            ledcWrite(0, constrain(t, 0, 255));
            ledcWrite(1, constrain(i, 0, 255));
            ledcWrite(2, constrain(m, 0, 255));
            ledcWrite(3, constrain(r, 0, 255));
            ledcWrite(4, constrain(p, 0, 255));
        }
    }
}
