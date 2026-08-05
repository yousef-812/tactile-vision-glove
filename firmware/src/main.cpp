#include <Arduino.h>

// TactileVision Full 3D Spatial & Velocity Haptic Simulator Firmware
// Pins for 5 Haptic Vibration Motors
#define MOTOR_THUMB_LEFT      4   // Pin 4 (Thumb - Left Obstacle)
#define MOTOR_INDEX_RIGHT     5   // Pin 5 (Index - Right Obstacle)
#define MOTOR_MIDDLE_OVERHEAD 6   // Pin 6 (Middle - Overhead/Front Obstacle)
#define MOTOR_RING_GROUND     7   // Pin 7 (Ring - Ground/Caution)
#define MOTOR_PALM_CENTER     15  // Pin 15 (Palm - Fast Emergency Hazard)

// 3 Ultrasonic Sonar Sensors for 3D Spatial Coverage
#define TRIG_LEFT   12
#define ECHO_LEFT   13

#define TRIG_CENTER 35
#define ECHO_CENTER 36

#define TRIG_RIGHT  37
#define ECHO_RIGHT  38

// Velocity calculation variables
float prevCenterDist = 400.0;
unsigned long prevTime = 0;

void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.println("=========================================================");
    Serial.println("🖐️ TactileVision 3D Spatial & Velocity Simulator Ready");
    Serial.println("   Left, Front, and Right Sensors Active!");
    Serial.println("=========================================================");

    // Motor Pins
    pinMode(MOTOR_THUMB_LEFT, OUTPUT);
    pinMode(MOTOR_INDEX_RIGHT, OUTPUT);
    pinMode(MOTOR_MIDDLE_OVERHEAD, OUTPUT);
    pinMode(MOTOR_RING_GROUND, OUTPUT);
    pinMode(MOTOR_PALM_CENTER, OUTPUT);

    // Left Sensor
    pinMode(TRIG_LEFT, OUTPUT);
    pinMode(ECHO_LEFT, INPUT);

    // Center Sensor
    pinMode(TRIG_CENTER, OUTPUT);
    pinMode(ECHO_CENTER, INPUT);

    // Right Sensor
    pinMode(TRIG_RIGHT, OUTPUT);
    pinMode(ECHO_RIGHT, INPUT);
}

float measureDistanceCM(int trigPin, int echoPin) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH, 25000);
    if (duration == 0) return 400.0;
    return duration * 0.0343 / 2.0;
}

void loop() {
    // Check if Serial Command is available (from Python AI webcam simulator)
    if (Serial.available() > 0) {
        String data = Serial.readStringUntil('\n');
        data.trim();
        if (data.startsWith("PWM:")) {
            int t = 0, i = 0, m = 0, r = 0, p = 0;
            if (sscanf(data.c_str(), "PWM:%d,%d,%d,%d,%d", &t, &i, &m, &r, &p) == 5) {
                Serial.printf("📥 [AI Vision Control] PWM -> Thumb:%d | Index:%d | Middle:%d | Ring:%d | Palm:%d\n", t, i, m, r, p);
                
                // Write digital states to the simulated LED pins
                digitalWrite(MOTOR_THUMB_LEFT, t > 0 ? HIGH : LOW);
                digitalWrite(MOTOR_INDEX_RIGHT, i > 0 ? HIGH : LOW);
                digitalWrite(MOTOR_MIDDLE_OVERHEAD, m > 0 ? HIGH : LOW);
                digitalWrite(MOTOR_RING_GROUND, r > 0 ? HIGH : LOW);
                digitalWrite(MOTOR_PALM_CENTER, p > 0 ? HIGH : LOW);
                
                delay(10); // Tiny debounce/refresh delay
                return;    // Bypasses sonar sensing when under live AI webcam control
            }
        }
    }

    // Fallback: 3D Sonar Sensing Logic (when no active serial packet is received)
    unsigned long currentTime = millis();
    float dt = (currentTime - prevTime) / 1000.0; // Time delta in seconds
    if (dt < 0.05) dt = 0.05;

    // Read 3D Spatial Distances
    float distLeft   = measureDistanceCM(TRIG_LEFT, ECHO_LEFT);
    float distCenter = measureDistanceCM(TRIG_CENTER, ECHO_CENTER);
    float distRight  = measureDistanceCM(TRIG_RIGHT, ECHO_RIGHT);

    // Calculate Approach Velocity (cm / second)
    float velocityCMs = (prevCenterDist - distCenter) / dt;
    prevCenterDist = distCenter;
    prevTime = currentTime;

    Serial.print("📍 [3D Spatial Sensing] -> Left: ");
    Serial.print(distLeft, 0);
    Serial.print("cm | Center: ");
    Serial.print(distCenter, 0);
    Serial.print("cm | Right: ");
    Serial.print(distRight, 0);
    Serial.print("cm | Velocity: ");
    Serial.print(velocityCMs, 0);
    Serial.println(" cm/s");

    // Reset motor states
    digitalWrite(MOTOR_THUMB_LEFT, LOW);
    digitalWrite(MOTOR_INDEX_RIGHT, LOW);
    digitalWrite(MOTOR_MIDDLE_OVERHEAD, LOW);
    digitalWrite(MOTOR_RING_GROUND, LOW);
    digitalWrite(MOTOR_PALM_CENTER, LOW);

    // 1. FAST APPROACH / HIGH VELOCITY EMERGENCY MODE (جسم متقدم بسرعة فائقة)
    if (velocityCMs > 60.0 && distCenter < 120.0) {
        Serial.println("   🚨 FAST APPROACH DETECTED! Rapid Emergency Pulse Mode!");
        // Rapid Pulsing Flash on Palm & Middle Motor
        for (int k = 0; k < 3; k++) {
            digitalWrite(MOTOR_PALM_CENTER, HIGH);
            digitalWrite(MOTOR_MIDDLE_OVERHEAD, HIGH);
            delay(50);
            digitalWrite(MOTOR_PALM_CENTER, LOW);
            digitalWrite(MOTOR_MIDDLE_OVERHEAD, LOW);
            delay(50);
        }
        return;
    }

    // 2. LEFT OBSTACLE HAZARD (عائق على اليسار)
    if (distLeft < 150.0) {
        Serial.println("   👈 LEFT OBSTACLE: Thumb Motor ON!");
        digitalWrite(MOTOR_THUMB_LEFT, HIGH);
    }

    // 3. RIGHT OBSTACLE HAZARD (عائق على اليمين)
    if (distRight < 150.0) {
        Serial.println("   👉 RIGHT OBSTACLE: Index Motor ON!");
        digitalWrite(MOTOR_INDEX_RIGHT, HIGH);
    }

    // 4. FRONT / OVERHEAD OBSTACLE (عائق أمامي / علوي)
    if (distCenter < 150.0) {
        if (distCenter < 50.0) {
            Serial.println("   🛑 IMMEDIATE FRONT COLLISION: Palm Motor ON!");
            digitalWrite(MOTOR_PALM_CENTER, HIGH);
        } else {
            Serial.println("   ⬆️ OVERHEAD / FRONT OBSTACLE: Middle Motor ON!");
            digitalWrite(MOTOR_MIDDLE_OVERHEAD, HIGH);
        }
    }

    delay(150);
}
