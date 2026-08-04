# 🚀 SETUP & RUN GUIDE - TactileVision AI Camera System
> **دليل التثبيت والتشغيل الكامل على أي جهاز**
> اتبع الخطوات دي بالترتيب وهيشتغل على أي كمبيوتر عنده كاميرا!

---

## 📋 المتطلبات الأساسية (Requirements)

| الأداة | الإصدار المطلوب | رابط التحميل |
|---|---|---|
| **Python** | 3.9 أو أعلى | https://www.python.org/downloads/ |
| **Git** | أي إصدار | https://git-scm.com/downloads |
| **كاميرا ويب** | مدمجة أو خارجية | - |

---

## ⚡ الخطوة 1: تحميل المشروع من GitHub

افتح الـ **Terminal / Command Prompt** وشغّل:

```bash
git clone https://github.com/yousef-812/tactile-vision-glove.git
```

ثم ادخل على مجلد المشروع:

```bash
cd tactile-vision-glove
```

---

## 📦 الخطوة 2: تثبيت المكتبات المطلوبة

```bash
pip install opencv-python numpy
```

> ⏳ انتظر حتى تكتمل عملية التثبيت (قد تأخذ دقيقة حسب سرعة الإنترنت)

---

## ▶️ الخطوة 3: تشغيل نظام الكاميرا الذكية

```bash
python ai_engine/live_camera_haptic_vision.py
```

---

## 🎮 الخطوة 4: كيفية التجربة

بعد فتح شاشة الكاميرا، جرب السيناريوهات دي:

| الفعل أمام الكاميرا | المحرك اللي هيشتغل | اللمبة في Wokwi |
|---|---|---|
| 👈 حرك يدك في **الجانب الأيسر** | **Thumb (Left)** | 🔴 أحمر |
| 👉 حرك يدك في **الجانب الأيمن** | **Index (Right)** | 🟡 أصفر |
| ⬆️ حرك يدك في **المنتصف العلوي** | **Middle (Overhead)** | 🟣 بنفسجي |
| ⬇️ حرك يدك في **المنتصف السفلي** | **Ring (Ground)** | 🩵 سماوي |
| ✋ اقرب يدك **بسرعة كبيرة** للكاميرا | **Palm - EMERGENCY !!** | 🔴 أحمر شديد + وميض |

### 🛑 للإنهاء:
اضغط **`Q`** أو **`ESC`** على شاشة الكاميرا.

---

## 🗺️ شرح الشاشة (HUD Explanation)

```
┌────────────────────────────────────────────────────────┐
│  LEFT         │       CENTER        │        RIGHT      │
│  [بار أحمر]   │    [بار برتقالي]    │    [بار أزرق]    │
│               │                     │                   │
│    ← الجهة اليسرى ← الأمام → الجهة اليمنى →            │
│                                                        │
│  ┌─────────────────────────┐                          │
│  │  HAPTIC GLOVE MOTORS    │                          │
│  │  ● Thumb (Left)   → off │                          │
│  │  ● Index (Right)  → ON  │                          │
│  │  ● Middle         → off │                          │
│  │  ● Ring (Ground)  → off │                          │
│  │  ● Palm     → EMERGENCY │                          │
│  └─────────────────────────┘                          │
└────────────────────────────────────────────────────────┘
```

---

## ❓ حل المشاكل الشائعة (Troubleshooting)

### ❌ خطأ: `No module named 'cv2'`
```bash
pip install opencv-python
```

### ❌ خطأ: `Cannot open webcam`
- تأكد إن الكاميرا متوصلة وشغالة.
- جرب تغيير رقم الكاميرا في الكود من `0` إلى `1`:
  ```python
  system = TactileVisionCameraSystem(camera_index=1)
  ```

### ❌ الشاشة بتظهر لكن مافيش استجابة للحركة
- تأكد إن الإضاءة في الغرفة كافية.
- حرك يدك بشكل أبطأ وأوضح أمام الكاميرا.

---

## 📁 هيكل المشروع

```
tactile-vision-glove/
├── README.md                          ← نظرة عامة على المشروع
├── ai_engine/
│   ├── live_camera_haptic_vision.py   ← ✅ شغّل الملف ده
│   ├── haptic_translator.py           ← خوارزمية تحويل العمق للاهتزاز
│   └── main_inference.py              ← محاكي الاختبار بدون كاميرا
├── firmware/
│   ├── src/main.cpp                   ← كود ESP32-S3
│   └── diagram.json                   ← مخطط Wokwi
└── docs/
    ├── ARCHITECTURE.md                ← التوثيق الهندسي
    └── FULL_PROJECT_PLAN_FOR_FARES.md ← خطة المشروع الكاملة
```

---

## 🔗 روابط مهمة

- **GitHub Repository:** https://github.com/yousef-812/tactile-vision-glove
- **Wokwi Simulator:** https://wokwi.com/projects/471464228464549889

---

> **Made with ❤️ by Eng Yousef & Fares Saif**
