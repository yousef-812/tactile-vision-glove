import 'package:flutter/material.dart';

void main() {
  runApp(const TactileVisionApp());
}

class TactileVisionApp extends StatelessWidget {
  const TactileVisionApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'TactileVision Caregiver Portal',
      debugShowCheckedModeBanner: false,
      theme: ThemeData(
        brightness: Brightness.dark,
        primarySwatch: Colors.teal,
        scaffoldBackgroundColor: const Color(0xFF121212),
        useMaterial3: true,
      ),
      home: const DashboardScreen(),
    );
  }
}

class DashboardScreen extends StatefulWidget {
  const DashboardScreen({super.key});

  @override
  State<DashboardScreen> createState() => _DashboardScreenState();
}

class _DashboardScreenState extends State<DashboardScreen> {
  bool isConnected = true;
  int batteryLevel = 85;
  String gloveStatus = "Active & Sensing";

  final Map<String, int> motorPwms = {
    'Thumb (Left)': 45,
    'Index (Right)': 20,
    'Middle (Overhead)': 190,
    'Ring (Ground)': 10,
    'Palm (Collision)': 0,
  };

  void triggerEmergencySos() {
    ScaffoldMessenger.of(context).showSnackBar(
      const SnackBar(
        content: Text('🚨 EMERGENCY SOS: Location sent to Family & Emergency Contacts!'),
        backgroundColor: Colors.redAccent,
        duration: Duration(seconds: 4),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('🖐️ TactileVision Caregiver Portal'),
        centerTitle: true,
        backgroundColor: const Color(0xFF1E1E1E),
        elevation: 2,
        actions: [
          IconButton(
            icon: const Icon(Icons.refresh),
            onPressed: () {},
          )
        ],
      ),
      body: SingleChildScrollView(
        padding: const EdgeInsets.all(16.0),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            // Status Card
            Card(
              color: const Color(0xFF1E1E1E),
              shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(16)),
              child: Padding(
                padding: const EdgeInsets.all(16.0),
                child: Row(
                  children: [
                    CircleAvatar(
                      backgroundColor: isConnected ? Colors.green.withOpacity(0.2) : Colors.red.withOpacity(0.2),
                      radius: 28,
                      child: Icon(
                        isConnected ? Icons.bluetooth_connected : Icons.bluetooth_disabled,
                        color: isConnected ? Colors.green : Colors.red,
                        size: 30,
                      ),
                    ),
                    const SizedBox(width: 16),
                    Expanded(
                      child: Column(
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Text(
                            isConnected ? 'Glove Connected' : 'Glove Disconnected',
                            style: const TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
                          ),
                          Text(
                            'Status: $gloveStatus',
                            style: const TextStyle(color: Colors.grey),
                          ),
                        ],
                      ),
                    ),
                    Chip(
                      avatar: const Icon(Icons.battery_charging_full, color: Colors.tealAccent, size: 18),
                      label: Text('$batteryLevel%'),
                      backgroundColor: Colors.black26,
                    )
                  ],
                ),
              ),
            ),
            const SizedBox(height: 20),

            // Live Haptic Matrix Monitor
            const Text(
              '🖐️ Real-time Haptic Motor Matrix',
              style: TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
            ),
            const SizedBox(height: 10),
            Card(
              color: const Color(0xFF1E1E1E),
              shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(16)),
              child: Padding(
                padding: const EdgeInsets.all(16.0),
                child: Column(
                  children: motorPwms.entries.map((entry) {
                    final motorName = entry.key;
                    final pwm = entry.value;
                    final isHigh = pwm > 150;
                    return Padding(
                      padding: const EdgeInsets.symmetric(vertical: 8.0),
                      child: Row(
                        children: [
                          Expanded(
                            flex: 3,
                            child: Text(motorName, style: const TextStyle(fontSize: 14)),
                          ),
                          Expanded(
                            flex: 5,
                            child: LinearProgressIndicator(
                              value: pwm / 255.0,
                              backgroundColor: Colors.grey[800],
                              color: isHigh ? Colors.redAccent : Colors.tealAccent,
                              minHeight: 8,
                            ),
                          ),
                          const SizedBox(width: 12),
                          Text('$pwm PWM', style: TextStyle(color: isHigh ? Colors.redAccent : Colors.grey)),
                        ],
                      ),
                    );
                  }).toList(),
                ),
              ),
            ),
            const SizedBox(height: 20),

            // Live GPS & Emergency Section
            const Text(
              '📍 Live Location & Safety',
              style: TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
            ),
            const SizedBox(height: 10),
            Card(
              color: const Color(0xFF1E1E1E),
              shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(16)),
              child: ListTile(
                leading: const Icon(Icons.location_on, color: Colors.tealAccent, size: 36),
                title: const Text('Cairo, Egypt (Live GPS)'),
                subtitle: const Text('Accuracy: High (3 meters) • Fall Sensor Active'),
                trailing: IconButton(
                  icon: const Icon(Icons.map, color: Colors.tealAccent),
                  onPressed: () {},
                ),
              ),
            ),
            const SizedBox(height: 24),

            // Emergency SOS Button
            SizedBox(
              width: double.infinity,
              height: 54,
              child: ElevatedButton.icon(
                style: ElevatedButton.styleFrom(
                  backgroundColor: Colors.redAccent,
                  shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(16)),
                ),
                icon: const Icon(Icons.warning_amber_rounded, color: Colors.white, size: 28),
                label: const Text(
                  'TRIGGER EMERGENCY SOS',
                  style: TextStyle(fontSize: 16, fontWeight: FontWeight.bold, color: Colors.white),
                ),
                onPressed: triggerEmergencySos,
              ),
            ),
          ],
        ),
      ),
    );
  }
}
