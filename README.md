# 💊 Smart Medicine Dispenser System (IoT)

An IoT-based Smart Medicine Dispenser designed to help patients take medicines on time.  
The system uses NodeMCU (ESP8266), Servo Motors, and Blynk Cloud for real-time monitoring, alerts, and control.

---

## 🚀 Features

- ⏰ Automatic medicine reminders (Morning, Afternoon, Night)
- 🔔 Buzzer alert at scheduled time
- ⏳ 20-second confirmation window using push button
- ✅ If button pressed → Medicine Taken
- ❌ If not pressed → Medicine Missed
- 📲 Real-time status on Blynk Dashboard
- 📧 Email & Push Notification alerts
- ⚙️ Manual control via mobile app
- 💡 LED indication for active medicine box
- 🔄 Configurable timings from Blynk

---

## 🛠️ Hardware Components

- NodeMCU (ESP8266)
- Servo Motors (3x)
- Push Button
- Buzzer
- LEDs (3x)
- Jumper Wires
- Power Supply (5V external recommended)

---

## 🧠 Working Principle

1. The system fetches real-time time using NTP.
2. At scheduled times, a buzzer alert is triggered.
3. User has 20 seconds to press the button.
4. If button is pressed:
   - Selected medicine boxes open
   - Status → "Medicine Taken"
5. If not pressed:
   - No box opens
   - Status → "Medicine Missed"
6. Notifications are sent via Blynk (Push + Email).

---

## 📲 Blynk Setup

- Create Template in Blynk
- Add Datastreams (V1–V20)
- Create Event:
  - Event Name: `medicine_alert`
  - Enable: Push Notification + Email
  - Message: `{EVENT_VALUE}`

---

## 💻 Technologies Used

- Arduino IDE
- ESP8266 (NodeMCU)
- Blynk IoT Platform
- Embedded C++

---

---

## 📌 Future Improvements

- RTC Module for offline use
- Voice assistant integration
- Mobile app notifications with history tracking
- Multiple user support

---

## 👨‍💻 Author

Mayank Aneja(24MCA20481)
and 
Rohit Negi(24MCA20093)
(MCA Project)
