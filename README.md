# 🖐️ TactileVision: Haptic-Vision Glove for the Visually Impaired
> **القفاز اللمسي الذكي لنقل الرؤية البصرية إلى إحساس لمسي عصبوني للمكفوفين**

[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Status](https://img.shields.io/badge/Status-In%20Development-green.svg)]()
[![Target](https://img.shields.io/badge/Domain-Assistive%20Tech%20%7C%20AI%20%7C%20IoT-orange.svg)]()

---

## 📌 1. Overview (المفهوم والهدف)

**TactileVision** هو مشروع مسابقات وتخرج مبتكر يهدف إلى مساعدة المكفوفين وضِعاف البصر على استكشاف العالم المحيط بهم بدون الحاجة للسمع، وذلك عبر تحويل المشهد ثلاثي الأبعاد إلى **خريطة اهتزازات لمسية عصبونية (Haptic Matrix)** على أصابع وراحة اليد باستغلال خاصية **الإحلال الحسي العصبي (Sensory Substitution & Neuroplasticity)**.

### 🌟 أهم المشكلات التي يحلها المشروع:
- **العوائق العلوية المعلقة:** كأغصان الأشجار ولافتات المحلات والأبواب المعلقة التي لا تكشفها العصا البيضاء.
- **الأجسام المتحركة:** السيارات، الموتوسيكلات، والمارة المسارعين.
- **الحفاظ على حاسة السمع:** الجهاز لا ينطق أصواتاً لكي لا يشوش على حاسة السمع التي يعتمد عليها الكفيف كلياً.
- **شبكة الأمان والنجاة:** كشف سقوط الكفيف (Fall Detection) وإرسال استغاثة SOS مع موقعه الحي (GPS) لعائلته.

---

## 🗺️ 2. Project File Structure (خريطة هيكل الملفات)

```text
tactile_vision_glove/
├── README.md                            # Document Overview & Folder Map
├── docs/                                # Detailed Documentation & Specs
│   ├── ARCHITECTURE.md                  # Haptic Matrix & System Architecture
│   ├── HARDWARE_BOM.md                  # Bill of Materials & Wiring Diagrams
│   └── PRESENTATION_PITCH.md            # Competition Pitch Script & Judge Demo Plan
│
├── firmware/                            # Embedded Microcontroller Code (ESP32-S3 / C++)
│   ├── src/
│   │   ├── main.cpp                     # Microcontroller Entry & Main Loop
│   │   ├── haptic_controller.cpp/.h     # PWM Matrix Driver for ERM/LRA Vibration Motors
│   │   ├── depth_sensor.cpp/.h          # ToF Laser (VL53L0X) Distance Reading
│   │   └── fall_detection.cpp/.h        # MPU6050 Gyroscope Fall & Impact Detection
│   └── platformio.ini                   # PlatformIO Config & Dependencies
│
├── ai_engine/                           # AI & Computer Vision Engine (Python)
│   ├── models/                          # Lightweight ONNX Models (Depth & YOLOv8 Nano)
│   ├── haptic_translator.py             # 3D Depth Map -> Haptic Motor Grid Conversion
│   ├── object_classifier.py             # Obstacle & Hazard Detection System
│   └── main_inference.py                # Main Vision-to-Haptic Pipeline
│
├── mobile_app/                          # Caregiver Mobile App (Flutter / React Native)
│   ├── lib/                             # Application Logic & UI
│   │   ├── screens/                     # Live Map, Battery, Alert Settings
│   │   └── services/                    # Bluetooth BLE & SOS Emergency Service
│   └── pubspec.yaml                     # Mobile Project Configuration
│
└── tests/                               # Simulation & Testing Suite
    ├── haptic_simulator.py              # Visual 2D Motor Matrix Vibration Simulator
    └── test_depth_mapping.py            # Unit Tests for Depth-to-PWM Translation
```

---

## ✋ 3. Haptic Mapping Logic (خوارزمية مصفوفة الاهتزاز)

يتم توزيع **8 محركات اهتزاز ميكرونية (Micro Vibration Motors)** على القفاز بالشكل التالي:

```text
                     [وسطى (Middle)]
                 (عائق علوي مرتفع / لافتات)
                            │
     [سبابة (Index)] ───────┼─────── [إبهام (Thumb)]
    (عائق على اليمين)        │       (عائق على اليسار)
                            │
                     [بنصر (Ring)]
                 (عائق أرضي / سلالم)
                            │
                   [راحة اليد (Palm)]
             (خطر عاجل / جسم قريب جداً)
```

### 📶 قواعد الترميز اللمسي (Encoding Rules):
1. **الموقع والارتفاع (Spatial Direction):** المحرك المقابل لاتجاه الحاجز هو من يهتز.
2. **المسافة والشدة (Distance PWM):**
   - **بعيد (3-4م):** اهتزاز خفيف نبضي متباعد.
   - **متوسط (1.5-2م):** اهتزاز متوسط المستمر.
   - **قريب جداً (أقل من 60سم):** اهتزاز فائق السرعة في راحة اليد (خطر عاجل).

---

## 🛠️ 4. Hardware Components (العتاد المطلوب)

- **Main Board:** ESP32-S3 (Wi-Fi + BLE + AI Vector Extensions) أو Raspberry Pi Zero 2W.
- **Motors:** 8x Micro Coin Vibration Motors (ERM 3V).
- **Sensors:** 
  - ToF Laser Distance Sensor (VL53L0X).
  - 6-Axis Gyro/Accelerometer (MPU6050).
  - Ultra Wide-Angle Micro Camera Module.
- **Power:** 3.7V Rechargeable LiPo Battery + TP4056 Charging Board.

---

## ⚡ 5. Quick Start (تشغيل التجربة الأولية)

### تشغيل المحاكي البرمجي (AI Haptic Simulator):
```bash
# Move to AI engine folder
cd ai_engine

# Install dependencies
pip install opencv-python numpy

# Run the real-time haptic simulator
python main_inference.py
```

---

## 📜 License & Authors
- **Team Lead / Developers:** Eng Yousef & Fares Saif
- **License:** MIT License
