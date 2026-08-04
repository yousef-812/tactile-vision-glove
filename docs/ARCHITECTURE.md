# 🏗️ TactileVision System Architecture & Haptic Specification

## 1. High-Level System Architecture

```text
+-----------------------+      +-------------------------------+      +---------------------------------+
|  Input Sensing Layer  |      |   AI & Edge Compute Layer     |      |    Haptic Output Matrix Layer   |
|                       |      |                               |      |                                 |
| - Wide-Angle Camera   | ---> | - Depth Anything / MiDaS Nano | ---> | - 8x Micro Coin Vibration Motors|
| - ToF Laser (VL53L0X) |      | - YOLOv8 Hazard Classifier    |      | - PWM Driver Signal (0-255)     |
| - MPU6050 Gyroscope   |      | - Spatial-to-Haptic Translator|      | - Haptic Pattern Generator      |
+-----------------------+      +-------------------------------+      +---------------------------------+
                                               |
                                               v
                                   +-----------------------+
                                   | Caregiver Mobile App  |
                                   | - BLE Bluetooth Sync  |
                                   | - GPS Emergency SOS   |
                                   +-----------------------+
```

---

## 2. Haptic Translator Specification (Algorithm)

The Haptic Translator converts a 2D Depth Matrix ($D$) of size $W \times H$ into an 8-channel PWM Array ($P_1 \dots P_8$).

### Formula for Distance to PWM Intensity:
$$\text{PWM}(d) = \begin{cases} 
255 & \text{if } d \le 0.5\text{m} \quad (\text{CRITICAL DANGER}) \\
\left\lfloor 255 \times \left(1 - \frac{d - 0.5}{3.5}\right) \right\rfloor & \text{if } 0.5\text{m} < d \le 4.0\text{m} \\
0 & \text{if } d > 4.0\text{m} \quad (\text{SAFE ZONE})
\end{cases}$$

### Motor Mapping Table:

| Motor Index | Location on Hand | Trigger Region in Vision Field | Primary Hazard Target |
|---|---|---|---|
| **M1** | Thumb (الإبهام) | Left Quadrant (X: 0% - 33%) | Side obstacles, walls, furniture |
| **M2** | Index Finger (السبابة) | Right Quadrant (X: 67% - 100%) | Side obstacles, pedestrians |
| **M3** | Middle Finger (الوسطى) | Top Quadrant (Y: 0% - 40%) | **Overhead Hazards** (branches, signs) |
| **M4** | Ring Finger (البنصر) | Bottom Quadrant (Y: 60% - 100%) | **Ground Hazards** (stairs, holes) |
| **M5** | Palm Center (راحة اليد) | Central Region (X: 33%-67%, Y: 30%-70%) | **Critical Immediate Collision** |
| **M6** | Wrist Top (معصم أسرع) | Fast Motion Velocity Vector | Rapidly approaching vehicles |
