#include <Arduino.h>

// TactileVision ESP32 Haptic Glove Driver Firmware
// Pin Definitions for 5 Haptic Vibration Motors

#define MOTOR_THUMB_LEFT      4   // Pin 4 (Thumb)
#define MOTOR_INDEX_RIGHT     5   // Pin 5 (Index)
#define MOTOR_MIDDLE_OVERHEAD 18  // Pin 18 (Middle)
#define MOTOR_RING_GROUND     19  // Pin 19 (Ring)
#define MOTOR_PALM_CENTER     21  // Pin 21 (Palm)

void setMotorState(int pin, bool state) {
    digitalWrite(pin, state ? HIGH : LOW);
}

void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.println("=================================================");
    Serial.println("🖐️ TactileVision ESP32 Glove Firmware Booting");
    Serial.println("   Wokwi Simulator Ready!");
    Serial.println("=================================================");

    pinMode(MOTOR_THUMB_LEFT, OUTPUT);
    pinMode(MOTOR_INDEX_RIGHT, OUTPUT);
    pinMode(MOTOR_MIDDLE_OVERHEAD, OUTPUT);
    pinMode(MOTOR_RING_GROUND, OUTPUT);
    pinMode(MOTOR_PALM_CENTER, OUTPUT);

    Serial.println("✅ All 5 Haptic Motor Pins Configured as OUTPUT.");
}

void runSelfTest() {
    int pins[] = {MOTOR_THUMB_LEFT, MOTOR_INDEX_RIGHT, MOTOR_MIDDLE_OVERHEAD, MOTOR_RING_GROUND, MOTOR_PALM_CENTER};
    const char* names[] = {"Thumb (Left)", "Index (Right)", "Middle (Overhead)", "Ring (Ground)", "Palm (Collision)"};

    for (int i = 0; i < 5; i++) {
        Serial.print("⚡ Testing Motor [");
        Serial.print(names[i]);
        Serial.println("] -> ON");
        
        digitalWrite(pins[i], HIGH);
        delay(600);
        digitalWrite(pins[i], LOW);
        delay(200);
    }
}

void loop() {
    runSelfTest();
}
