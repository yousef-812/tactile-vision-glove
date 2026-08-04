#include <Arduino.h>

// TactileVision Interactive Distance-Sensing Firmware for Wokwi & ESP32-S3

#define MOTOR_THUMB_LEFT      4   // Pin 4 (Thumb)
#define MOTOR_INDEX_RIGHT     5   // Pin 5 (Index)
#define MOTOR_MIDDLE_OVERHEAD 6   // Pin 6 (Middle)
#define MOTOR_RING_GROUND     7   // Pin 7 (Ring)
#define MOTOR_PALM_CENTER     15  // Pin 15 (Palm)

#define PIN_TRIG 12
#define PIN_ECHO 13

void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.println("=================================================");
    Serial.println("🖐️ TactileVision Interactive Distance Mode Ready");
    Serial.println("   Change distance slider on Wokwi to test!");
    Serial.println("=================================================");

    pinMode(MOTOR_THUMB_LEFT, OUTPUT);
    pinMode(MOTOR_INDEX_RIGHT, OUTPUT);
    pinMode(MOTOR_MIDDLE_OVERHEAD, OUTPUT);
    pinMode(MOTOR_RING_GROUND, OUTPUT);
    pinMode(MOTOR_PALM_CENTER, OUTPUT);

    pinMode(PIN_TRIG, OUTPUT);
    pinMode(PIN_ECHO, INPUT);
}

float readDistanceCM() {
    digitalWrite(PIN_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(PIN_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIG, LOW);

    long duration = pulseIn(PIN_ECHO, HIGH, 30000);
    if (duration == 0) return 400.0;
    return duration * 0.0343 / 2.0;
}

void loop() {
    float distance = readDistanceCM();
    
    Serial.print("📍 Distance Measured: ");
    Serial.print(distance);
    Serial.println(" cm");

    // Clear all motor states
    digitalWrite(MOTOR_THUMB_LEFT, LOW);
    digitalWrite(MOTOR_INDEX_RIGHT, LOW);
    digitalWrite(MOTOR_MIDDLE_OVERHEAD, LOW);
    digitalWrite(MOTOR_RING_GROUND, LOW);
    digitalWrite(MOTOR_PALM_CENTER, LOW);

    // Dynamic Haptic Response based on Distance
    if (distance <= 50) {
        // Critical Emergency Collision Hazard (< 50cm) -> Light Palm & Middle
        Serial.println("   🚨 CRITICAL DANGER (<50cm): Palm Collision Motor ON!");
        digitalWrite(MOTOR_PALM_CENTER, HIGH);
        digitalWrite(MOTOR_MIDDLE_OVERHEAD, HIGH);
    } 
    else if (distance <= 150) {
        // Medium Warning Distance (50cm - 150cm) -> Light Thumb & Index
        Serial.println("   ⚠️ WARNING ZONE (50-150cm): Thumb & Index Motors ON!");
        digitalWrite(MOTOR_THUMB_LEFT, HIGH);
        digitalWrite(MOTOR_INDEX_RIGHT, HIGH);
    } 
    else if (distance <= 250) {
        // Caution Distance (150cm - 250cm) -> Light Ring Finger
        Serial.println("   ℹ️ CAUTION ZONE (150-250cm): Ring Motor ON!");
        digitalWrite(MOTOR_RING_GROUND, HIGH);
    } 
    else {
        Serial.println("   ✅ SAFE ZONE (>250cm): Path Clear.");
    }

    delay(250);
}
