#define BLYNK_TEMPLATE_ID "TMPL6RKnG6cPV"
#define BLYNK_TEMPLATE_NAME "SMART MEDICINE DISPENSER"
#define BLYNK_AUTH_TOKEN "jvl--QDP9J49Q7v9DyNJ3lI2QAGcoQXs"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>
#include <time.h>

char ssid[] = "YOUR WIFI NAME";
char pass[] = "YOUR WIFI PASS";

// -------- PINS --------
#define SERVO1 D1
#define SERVO2 D2
#define SERVO3 D3

#define BUZZER D0
#define BUTTON D6

#define LED1 D5
#define LED2 D7
#define LED3 D4

Servo s1, s2, s3;

// -------- VARIABLES --------
int box1, box2, box3;
int mH, mM, aH, aM, nH, nM;
int mB1, mB2, mB3;
int aB1, aB2, aB3;
int nB1, nB2, nB3;

int lastTriggerMinute = -1;
int lastTriggerMode = 0;

// -------- MODE NAME FUNCTION --------
String getModeName(int mode) {
  if (mode == 1) return "Morning";
  if (mode == 2) return "Afternoon";
  if (mode == 3) return "Night";
  return "Manual";
}

// -------- SETUP --------
void setup() {
  Serial.begin(9600);

  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  digitalWrite(BUZZER, LOW);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);

  s1.attach(SERVO1);
  s2.attach(SERVO2);
  s3.attach(SERVO3);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  configTime(19800, 0, "pool.ntp.org");

  while (time(nullptr) < 100000) {
    delay(500);
  }

  Serial.println("READY ✅");
}

// -------- LOOP --------
void loop() {
  Blynk.run();
  checkTime();
}

// -------- TIME CHECK --------
void checkTime() {
  time_t now = time(nullptr);
  struct tm *t = localtime(&now);

  int currH = t->tm_hour;
  int currM = t->tm_min;

  if (currH == mH && currM == mM) {
    if (lastTriggerMinute != currM || lastTriggerMode != 1) {
      lastTriggerMinute = currM;
      lastTriggerMode = 1;
      startProcess(1);
    }
  }

  else if (currH == aH && currM == aM) {
    if (lastTriggerMinute != currM || lastTriggerMode != 2) {
      lastTriggerMinute = currM;
      lastTriggerMode = 2;
      startProcess(2);
    }
  }

  else if (currH == nH && currM == nM) {
    if (lastTriggerMinute != currM || lastTriggerMode != 3) {
      lastTriggerMinute = currM;
      lastTriggerMode = 3;
      startProcess(3);
    }
  }

  else {
    lastTriggerMinute = -1;
    lastTriggerMode = 0;
  }
}

// -------- START PROCESS --------
void startProcess(int mode) {

  beepBuzzer();

  String msg = getModeName(mode) + " Time ⏳ Press Button";
  Blynk.virtualWrite(V5, msg);

  unsigned long startTime = millis();

  while (millis() - startTime < 20000) {

    Blynk.run();

    if (digitalRead(BUTTON) == LOW) {
      delay(200);
      performDispense(mode);
      return;
    }
  }

  // ❌ Missed
  String missMsg = getModeName(mode) + " Medicine Missed ❌";
  Blynk.virtualWrite(V5, missMsg);
}

// -------- MANUAL --------
BLYNK_WRITE(V4) {
  if (param.asInt() == 1) {
    startProcess(4);
  }
}

// -------- DISPENSE --------
void performDispense(int mode) {

  if (mode == 1) {
    if (mB1) moveServo(s1, LED1);
    if (mB2) moveServo(s2, LED2);
    if (mB3) moveServo(s3, LED3);
  }

  else if (mode == 2) {
    if (aB1) moveServo(s1, LED1);
    if (aB2) moveServo(s2, LED2);
    if (aB3) moveServo(s3, LED3);
  }

  else if (mode == 3) {
    if (nB1) moveServo(s1, LED1);
    if (nB2) moveServo(s2, LED2);
    if (nB3) moveServo(s3, LED3);
  }

  else {
    if (box1) moveServo(s1, LED1);
    if (box2) moveServo(s2, LED2);
    if (box3) moveServo(s3, LED3);
  }

  // ✅ Taken
  String takeMsg = getModeName(mode) + " Medicine Taken ✅";
  Blynk.virtualWrite(V5, takeMsg);
}

// -------- BUZZER --------
void beepBuzzer() {
  digitalWrite(BUZZER, HIGH);
  delay(500);
  digitalWrite(BUZZER, LOW);
}

// -------- SERVO (SPEED CONTROL) --------
void moveServo(Servo &s, int ledPin) {

  int maxAngle = 140;
  int speedDelay = 15;

  digitalWrite(ledPin, HIGH);

  // OPEN
  for (int pos = 0; pos <= maxAngle; pos++) {
    s.write(pos);
    delay(speedDelay);
  }

  delay(5000);

  // CLOSE
  for (int pos = maxAngle; pos >= 0; pos--) {
    s.write(pos);
    delay(speedDelay);
  }

  digitalWrite(ledPin, LOW);
}

// -------- BLYNK --------
BLYNK_WRITE(V1) { box1 = param.asInt(); }
BLYNK_WRITE(V2) { box2 = param.asInt(); }
BLYNK_WRITE(V3) { box3 = param.asInt(); }

BLYNK_WRITE(V6) { mH = param.asInt(); }
BLYNK_WRITE(V9) { mM = param.asInt(); }

BLYNK_WRITE(V7) { aH = param.asInt(); }
BLYNK_WRITE(V10){ aM = param.asInt(); }

BLYNK_WRITE(V8) { nH = param.asInt(); }
BLYNK_WRITE(V11){ nM = param.asInt(); }

BLYNK_WRITE(V12) { mB1 = param.asInt(); }
BLYNK_WRITE(V13) { mB2 = param.asInt(); }
BLYNK_WRITE(V14) { mB3 = param.asInt(); }

BLYNK_WRITE(V15) { aB1 = param.asInt(); }
BLYNK_WRITE(V16) { aB2 = param.asInt(); }
BLYNK_WRITE(V17) { aB3 = param.asInt(); }

BLYNK_WRITE(V18) { nB1 = param.asInt(); }
BLYNK_WRITE(V19) { nB2 = param.asInt(); }
BLYNK_WRITE(V20) { nB3 = param.asInt(); }
