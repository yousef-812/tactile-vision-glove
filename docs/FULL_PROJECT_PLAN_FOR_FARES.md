# 📋 خطة تنفيذ مشروع TactileVision التفصيلية والشاملة
> **دليل العمل المتكامل لهندسة وتطوير مشروع: القفاز اللمسي الذكي للمكفوفين (Tactile-Vision Haptic Glove)**
> **إعداد:** م. يوسف يسري & فارس سيف  
> **التاريخ:** أغسطس 2026

---

## 🎯 1. ملخص المشروع والهدف الرئيسي (Executive Summary)

مشروع **TactileVision** هو نظام مساعد أمني وصحي للمكفوفين وضِعاف البصر، يهدف إلى تمكين الكفيف من الإحساس بالبيئة ثلاثية الأبعاد المحيطة به دون الحاجة لحاسة السمع (التي يعتمد عليها كلياً) ودون الاعتماد فقط على العصا البيضاء.

### 🌟 النقاط التي تجعل المشروع الأول في المسابقات:
1. **الابتكار العلمي:** الاعتماد على نظرية **الإحلال الحسي العصبي (Sensory Substitution)** حيث يعيد المخ رسم الخريطة المكانية من خلال اهتزازات الجلد.
2. **كشف العوائق العلوية والمتحركة:** كشف لافتات المحلات المعلقة، الأبواب المفتوحة، الموتوسيكلات، والسلالم قبل الاصطدام بها.
3. **ديمو تفاعلي صادم للمحكمين:** إمكانية جعل أحد المحكمين يجرب القفاز وهو **معصوب العينين** وتفادي العوائق بنجاح كامل.

---

## 👥 2. توزيع المهام والمسؤوليات (Division of Roles)

تم تقسيم المشـروع بدقة لتغطية كافة أركانه التقنية والهندسية:

### 🔵 مهام المهندس يوسف (Software, AI & Mobile Lead):
1. **محرك الذكاء الاصطناعي (AI & Vision Engine):**
   - كتابة وتدريب نموذج تقدير العمق (Monocular Depth Estimation) باستخدام خوارزميات خفيفة مثل (MiDaS Nano / Depth Anything ONNX).
   - دمج نموذج كشف الأجسام والسريع (YOLOv8 Nano) لاكتشاف السيارات والسلالم والمارة.
2. **خوارزمية الترجمة الحركية (Haptic Translator Algorithm):**
   - تحويل خريطة العمق الـ 3D إلى إشارات PWM (0-255) وتوزيعها على مناطق اليد.
3. **تطبيق الموبايل (Companion Mobile App - Flutter):**
   - تصميم واجهة الموبايل للأهل وتكامل البلوتوث (BLE).
   - تتبع موقع الكفيف المباشر (GPS) ونظام استغاثة الطوارئ (SOS).

### 🟢 مهام الأخ فارس (Hardware, Embedded Systems & Design Lead):
1. **تجميع الدائرة الإلكترونية والعتاد (Hardware Assembly):**
   - توصيل وتجميع مصفوفة محركات الاهتزاز الخمسة (Micro Coin ERM Motors) وترانزستورات التحكم (NPN Transistors/Drivers).
   - توصيل مستشعر المسافة الليزري (ToF VL53L0X) ومستشعر الحركة والسقوط (MPU6050 Gyroscope) بـ ESP32-S3.
2. **كود الميكروكنترولر (Embedded Firmware C++):**
   - برمجة الـ ESP32 لاستقبال إشارات الـ PWM وتحويلها لـ 5 قنوات اهتزاز مستقلة.
   - قراءة بيانات السقوط المفاجئ من MPU6050 وإطلاق إشارة الاستغاثة.
3. **التصميم الفيزيائي وخياطة القفاز (Physical Glove Design):**
   - توزيع المحركات على الأصابع وراحة اليد وتثبيتها بقماش قفاز مريح ومريح للجلد مع إخفاء الأسلاك بشكل محمي ورائع.

---

## 📅 3. الخطة الزمنية التفصيلية (4-Week Execution Plan)

### 🗓️ الأسبوع الأول: شراء القطع وتجهيز البيئة (Hardware Sourcing & Setup)
- [ ] **فارس:** شراء المكونات الإلكترونية من محلات الإلكترونيات (ESP32-S3, 8x ERM Motors, ToF VL53L0X, MPU6050, LiPo Battery 3.7V, TP4056 Charger).
- [ ] **فارس:** اختبار كل محرك اهتزاز منفصلاً بواسطة إشارة PWM من الـ ESP32 وتأكيد التوصيل.
- [ ] **يوسف:** ضبط بيئة العمل على جهاز الكمبيوتر (Python 3.10+, OpenCV, PyTorch/ONNX, VS Code).
- [ ] **يوسف:** كتابة وتشغيل محاكي الـ Haptic Translator واختبار خوارزمية التحويل.

### 🗓️ الأسبوع الثاني: تطوير الكود الأساسي والدمج الأول (Core Prototyping)
- [ ] **فارس:** تجميع لوحة التوصيل (Breadboard Prototype) كاملة وتوصيل جميع المحركات والمستشعرات مع الـ ESP32.
- [ ] **فارس:** كتابة كود الاستجابة لإشارات البلوتوث والسيريال على الـ ESP32.
- [ ] **يوسف:** ربط الكاميرا المباشرة بمحرك الذكاء الاصطناعي واستخراج خريطة العمق لحظياً (Real-time Depth Map).
- [ ] **يوسف:** ربط مخرجات الـ AI بإرسال أوامر الـ PWM إلى الـ ESP32 عبر البلوتوث/السيريال.

### 🗓️ الأسبوع الثالث: تجميع القفاز النهائي وتطبيق الموبايل (Final Assembly & Mobile App)
- [ ] **فارس:** تثبيت المحركات والأسلاك على قفاز مرن، وتصميم بيت محمي (3D Printed or Compact Box) للبطارية ولوحة الـ ESP32 على المعصم.
- [ ] **فارس:** دمج خوارزمية كشف السقوط (Fall Detection) من مستشعر MPU6050.
- [ ] **يوسف:** بناء واجهات تطبيق الموبايل بـ Flutter (شاشة التتبع + زري الطوارئ واختبار المحركات).
- [ ] **يوسف:** ربط التطبيق بخدمة الرسائل النصية/الواتساب لإرسال موقع الكفيف فور السقوط.

### 🗓️ الأسبوع الرابع: الاختبار الميداني والعرض التقديمي (Field Testing & Pitching)
- [ ] **فارس ويوسف:** إجراء "اختبار معصوب العينين" (Blindfold Test) داخل الممر وتجربة تفادي العوائق الحقيقية.
- [ ] **يوسف:** إعداد شرائح العرض التقديمي (Pitch Deck) باللغة الإنجليزية للمسابقات.
- [ ] **فارس ويوسف:** تصوير فيديو ديمو ممتاز (Demo Video 2 Minutes) يوضح المشكلة والحل والديمو العملي.

---

## 🔌 4. مخطط التوصيلات الإلكترونية (Hardware Pinout Mapping)

جدول التوصيل الخاص بـ **ESP32-S3**:

| القطعة (Component) | نوع الإشارة | رجل الـ ESP32-S3 | الوظيفة |
| :--- | :--- | :--- | :--- |
| **محرك الإبهام (Thumb)** | PWM Output | `GPIO 4` | عائق يسار |
| **محرك السبابة (Index)** | PWM Output | `GPIO 5` | عائق يمين |
| **محرك الوسطى (Middle)** | PWM Output | `GPIO 6` | عائق علوي معلق |
| **محرك البنصر (Ring)** | PWM Output | `GPIO 7` | عائق أرضي / سلالم |
| **محرك راحة اليد (Palm)** | PWM Output | `GPIO 15` | خطر اصطدام عاجل |
| **ToF VL53L0X (SDA)** | I2C Data | `GPIO 21` | قياس مسافة بالليزر |
| **ToF VL53L0X (SCL)** | I2C Clock | `GPIO 22` | قياس مسافة بالليزر |
| **MPU6050 Gyro (SDA/SCL)**| I2C Shared | `GPIO 21 / 22` | كشف السقوط |
| **Micro Camera (Rx/Tx)** | Serial / SPI | `GPIO 17 / 18` | بث الصورة |

---

## 🗣️ 5. سيناريو العرض التقديمي بالإنجليزية للمحكمين (English Pitch Deck Script)

عند الوقوف أمام لجنة التحكيم في المسابقة:

> **"Good morning judges. Over 43 million people worldwide live with total blindness. Traditional white canes only detect obstacles on the ground, leaving visually impaired individuals vulnerable to hanging tree branches, signboards, and oncoming traffic."**
> 
> **"Introducing TactileVision: A wearable haptic-vision glove that transforms 3D spatial environments into intuitive tactile vibrations on the human palm using Sensory Substitution."**
> 
> **"Instead of cluttering the user's hearing with audio, our system maps overhead hazards to the middle finger, side obstacles to the thumb and index, and emergency collisions directly to the palm. Today, we invite one of the judges to put on a blindfold and experience seeing the world through touch!"**

---

## 🛡️ 6. إدارة المخاطر وتأمين الخطة (Risk Management)

1. **ماذا لو تأخرت إشارات البلوتوث؟**
   - **الحل:** تم إضافة مستشعر الليزر (ToF VL53L0X) على القفاز ليعمل كـ "فرملة طوارئ مباشرة" مستقلة على الـ ESP32 لضمان الاهتزاز الفوري حتى لو حدث تأخير في الـ AI.
2. **ماذا لو كانت إضاءة المكان ضعيفة؟**
   - **الحل:** تزويد القفاز أو النظارة بـ Micro LED صغير يعمل تلقائياً عند الإضاءة المنخفضة.
3. **ارتفاع حرارة المحركات:**
   - **الحل:** برمجة المحركات بنظام النبضات (Pulsing) بدلاً من التشغيل المستمر لتوفير الطاقة ومنع السخونة.

---

## 📂 المجلد والمستندات المرتبطة في المشروع:
- مجلد المشروع الرئيسي: [`H:\project\tactile_vision_glove`](file:///H:/project/tactile_vision_glove)
- توثيق المعمارية الفنية: [`docs/ARCHITECTURE.md`](file:///H:/project/tactile_vision_glove/docs/ARCHITECTURE.md)
- كود المحاكاة واختبار الـ AI: [`ai_engine/main_inference.py`](file:///H:/project/tactile_vision_glove/ai_engine/main_inference.py)
- كود الـ ESP32 Firmware: [`firmware/src/main.cpp`](file:///H:/project/tactile_vision_glove/firmware/src/main.cpp)
