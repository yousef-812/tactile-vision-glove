#include <Arduino.h>

// TactileVision ESP32-S3 Haptic Glove Driver Firmware for Wokwi Simulator & Hardware
// Pin Definitions for 5 Haptic Vibration Motors (PWM Controlled)

#define MOTOR_THUMB_LEFT      4   // PWM Channel 0 (Thumb)
#define MOTOR_INDEX_RIGHT     5   // PWM Channel 1 (Index)
#define MOTOR_MIDDLE_OVERHEAD 6   // PWM Channel 2 (Middle)
#define MOTOR_RING_GROUND     7   // PWM Channel 3 (Ring)
#define MOTOR_PALM_CENTER     15  // PWM Channel 4 (Palm)

void setMotorPWM(int motorPin, int pwmChannel, int pwmValue) {
    ledcWrite(pwmChannel, constrain(pwmValue, 0, 255));
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("=================================================");
    Serial.println("🖐️ TactileVision ESP32-S3 Glove Firmware Booting");
    Serial.println("   Wokwi Hardware Simulator Ready!");
    Serial.println("=================================================");

    // Configure PWM channels (50Hz frequency, 8-bit resolution: 0-255)
    ledcSetup(0, 50, 8); ledcAttachPin(MOTOR_THUMB_LEFT, 0);
    ledcSetup(1, 50, 8); ledcAttachPin(MOTOR_INDEX_RIGHT, 1);
    ledcSetup(2, 50, 8); ledcAttachPin(MOTOR_MIDDLE_OVERHEAD, 2);
    ledcSetup(3, 50, 8); ledcAttachPin(MOTOR_RING_GROUND, 3);
    ledcSetup(4, 50, 8); ledcAttachPin(MOTOR_PALM_CENTER, 4);

    Serial.println("✅ All 5 Haptic Motor PWM Channels Initialized.");
    Serial.println("   Starting Wokwi Self-Test Sequence...");
}

void runWokwiSelfTest() {
    int pins[] = {MOTOR_THUMB_LEFT, MOTOR_INDEX_RIGHT, MOTOR_MIDDLE_OVERHEAD, MOTOR_RING_GROUND, MOTOR_PALM_CENTER};
    const char* names[] = {"Thumb (Left)", "Index (Right)", "Middle (Overhead)", "Ring (Ground)", "Palm (Collision)"};

    for (int i = 0; i < 5; i++) {
        Serial.print("⚡ Testing Motor [");
        Serial.print(names[i]);
        Serial.println("] -> Pulse 255 PWM");
        
        ledcWrite(i, 255);
        delay(400);
        ledcWrite(i, 0);
        delay(200);
    }
}

void loop() {
    // 1. Run periodic self-test for Wokwi visual feedback
    runWokwiSelfTest();

    // 2. Read Serial command packet if sent from Python AI Engine (Format: "PWM:t,i,m,r,p\n")
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

            Serial.print("   [RECIEVED AI PWM MATRIX] -> ");
            Serial.println(data);
        }
    }
    delay(1000);
}
