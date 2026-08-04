#include <Arduino.h>
#include <esp_arduino_version.h>

// TactileVision ESP32-S3 Haptic Glove Driver Firmware for Wokwi Simulator & Hardware
// Compatible with ESP32 Arduino Core 2.x and Core 3.0+

#define MOTOR_THUMB_LEFT      4   // Pin 4 (Thumb)
#define MOTOR_INDEX_RIGHT     5   // Pin 5 (Index)
#define MOTOR_MIDDLE_OVERHEAD 6   // Pin 6 (Middle)
#define MOTOR_RING_GROUND     7   // Pin 7 (Ring)
#define MOTOR_PALM_CENTER     15  // Pin 15 (Palm)

// Helper function for backwards & forwards compatible PWM writing
void setMotorPWM(int pin, int channel, int pwmValue) {
    pwmValue = constrain(pwmValue, 0, 255);
#if defined(ESP_ARDUINO_VERSION_MAJOR) && (ESP_ARDUINO_VERSION_MAJOR >= 3)
    ledcWrite(pin, pwmValue);
#else
    ledcWrite(channel, pwmValue);
#endif
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("=================================================");
    Serial.println("🖐️ TactileVision ESP32-S3 Glove Firmware Booting");
    Serial.println("   Wokwi Hardware Simulator Ready!");
    Serial.println("=================================================");

    // Configure PWM channels (50Hz frequency, 8-bit resolution: 0-255)
#if defined(ESP_ARDUINO_VERSION_MAJOR) && (ESP_ARDUINO_VERSION_MAJOR >= 3)
    // New ESP32 Arduino Core 3.0+ API (Used by Wokwi)
    ledcAttach(MOTOR_THUMB_LEFT, 50, 8);
    ledcAttach(MOTOR_INDEX_RIGHT, 50, 8);
    ledcAttach(MOTOR_MIDDLE_OVERHEAD, 50, 8);
    ledcAttach(MOTOR_RING_GROUND, 50, 8);
    ledcAttach(MOTOR_PALM_CENTER, 50, 8);
#else
    // Legacy ESP32 Arduino Core 2.x API
    ledcSetup(0, 50, 8); ledcAttachPin(MOTOR_THUMB_LEFT, 0);
    ledcSetup(1, 50, 8); ledcAttachPin(MOTOR_INDEX_RIGHT, 1);
    ledcSetup(2, 50, 8); ledcAttachPin(MOTOR_MIDDLE_OVERHEAD, 2);
    ledcSetup(3, 50, 8); ledcAttachPin(MOTOR_RING_GROUND, 3);
    ledcSetup(4, 50, 8); ledcAttachPin(MOTOR_PALM_CENTER, 4);
#endif

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
        
        setMotorPWM(pins[i], i, 255);
        delay(400);
        setMotorPWM(pins[i], i, 0);
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
            
            setMotorPWM(MOTOR_THUMB_LEFT, 0, t);
            setMotorPWM(MOTOR_INDEX_RIGHT, 1, i);
            setMotorPWM(MOTOR_MIDDLE_OVERHEAD, 2, m);
            setMotorPWM(MOTOR_RING_GROUND, 3, r);
            setMotorPWM(MOTOR_PALM_CENTER, 4, p);

            Serial.print("   [RECIEVED AI PWM MATRIX] -> ");
            Serial.println(data);
        }
    }
    delay(1000);
}
