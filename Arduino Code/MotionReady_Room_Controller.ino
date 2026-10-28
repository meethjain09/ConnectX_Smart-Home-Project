#include <Servo.h>
#include <LiquidCrystal.h>

// Integrated smart-home sketch for Arduino Uno.
// Pin assignments match SmartHome_Integration_Map.md.
const byte PIN_TEMP       = A0;  // TMP36 output
const byte PIN_PIR        = A1;  // PIR digital output
const byte PIN_BUZZER     = A2;  // piezo sounder
const byte PIN_LDR        = A3;  // photocell divider midpoint
const byte PIN_MOTION_LED = A5;  // white motion indicator LED through 220 ohm
const byte PIN_FAN_LED    = 8;   // existing red fan-status LED through 1k
const byte PIN_SERVO      = 9;   // SG90 drives the horizontal sliding-door rack
const byte PIN_LAMP       = 10;  // lamp driver / low-voltage lamp indicator
const byte PIN_FAN        = 11;  // fan MOSFET gate driver
const byte PIN_TRIG       = 12;  // HC-SR04 trigger
const byte PIN_ECHO       = A4;  // HC-SR04 echo (uses analog pin as digital input)

// The 16x2 LCD wiring from the temperature circuit is kept intact.
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); // RS, E, D4, D5, D6, D7
Servo doorServo;

const int DARK_THRESHOLD = 450;       // Calibrate from Serial Monitor / Tinkercad readings.
const float FAN_ON_C = 28.0;          // Fan turns on at or above this temperature.
const float FAN_OFF_C = 26.0;         // Fan turns off at or below this temperature.
const float DOOR_OPEN_CM = 25.0;      // Open door when someone is this close.
const unsigned long DOOR_HOLD_MS = 3000;
const int DOOR_CLOSED_ANGLE = 0;
const int DOOR_OPEN_ANGLE = 90;

float temperatureC = 25.0;
int lightReading = 0;
bool fanOn = false;
bool lampOn = false;
bool doorOpen = false;
bool pirWasHigh = false;
unsigned long lastNearDoorMs = 0;
unsigned long lastRangeReadMs = 0;
unsigned long lastTempReadMs = 0;
unsigned long lastLightReadMs = 0;
unsigned long lastLcdUpdateMs = 0;

float readTemperatureC() {
  // Average a few readings to steady the TMP36 value.
  unsigned long total = 0;
  for (byte i = 0; i < 8; i++) {
    total += analogRead(PIN_TEMP);
    delay(2);
  }
  float raw = total / 8.0;
  float volts = raw * (5.0 / 1023.0);
  return (volts - 0.5) * 100.0; // TMP36: 500 mV at 0 C, 10 mV per degree C.
}

float readDistanceCm() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(3);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  unsigned long duration = pulseIn(PIN_ECHO, HIGH, 30000UL);
  if (duration == 0) return -1.0; // No echo received.
  return duration / 58.0;
}

void setDoor(bool open) {
  if (open == doorOpen) return;
  doorOpen = open;
  doorServo.write(open ? DOOR_OPEN_ANGLE : DOOR_CLOSED_ANGLE); // rack travel stops
}

void updateDisplay() {
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperatureC, 1);
  lcd.print("C FAN:");
  lcd.print(fanOn ? "ON " : "OFF");
  lcd.print(" ");

  lcd.setCursor(0, 1);
  lcd.print("L:");
  lcd.print(lampOn ? "ON " : "OFF");
  lcd.print(" D:");
  lcd.print(doorOpen ? "OPEN  " : "CLOSED");
  lcd.print(" ");
}

void setup() {
  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_MOTION_LED, OUTPUT);
  pinMode(PIN_FAN_LED, OUTPUT);
  pinMode(PIN_LAMP, OUTPUT);
  pinMode(PIN_FAN, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  digitalWrite(PIN_TRIG, LOW);
  digitalWrite(PIN_MOTION_LED, LOW);
  digitalWrite(PIN_FAN_LED, LOW);
  digitalWrite(PIN_LAMP, LOW);
  digitalWrite(PIN_FAN, LOW);

  doorServo.attach(PIN_SERVO);
  doorServo.write(DOOR_CLOSED_ANGLE);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Smart Home Ready");
}

void loop() {
  unsigned long now = millis();

  // Door: ultrasonic proximity commands the servo-driven horizontal rack slider.
  if (now - lastRangeReadMs >= 100) {
    lastRangeReadMs = now;
    float distanceCm = readDistanceCm();
    if (distanceCm > 0 && distanceCm <= DOOR_OPEN_CM) {
      lastNearDoorMs = now;
      setDoor(true);
    }
  }
  if (doorOpen && now - lastNearDoorMs >= DOOR_HOLD_MS) {
    setDoor(false);
  }

  // Light: the supplied divider has the photocell to +5 V and 1k to GND,
  // so the reading falls in darkness. The lamp output drives a low-voltage driver.
  if (now - lastLightReadMs >= 200) {
    lastLightReadMs = now;
    lightReading = analogRead(PIN_LDR);
    lampOn = (lightReading < DARK_THRESHOLD);
    digitalWrite(PIN_LAMP, lampOn ? HIGH : LOW);
  }

  // Motion: flash/hold the indicator while motion is present and chime on entry.
  bool pirHigh = digitalRead(PIN_PIR) == HIGH;
  digitalWrite(PIN_MOTION_LED, pirHigh ? HIGH : LOW);
  if (pirHigh && !pirWasHigh) {
    tone(PIN_BUZZER, 1800, 140);
  }
  pirWasHigh = pirHigh;

  // Temperature: hysteresis prevents rapid fan switching near the set point.
  if (now - lastTempReadMs >= 500) {
    lastTempReadMs = now;
    temperatureC = readTemperatureC();
    if (!fanOn && temperatureC >= FAN_ON_C) fanOn = true;
    else if (fanOn && temperatureC <= FAN_OFF_C) fanOn = false;
    digitalWrite(PIN_FAN, fanOn ? HIGH : LOW);
    digitalWrite(PIN_FAN_LED, fanOn ? HIGH : LOW);
  }

  if (now - lastLcdUpdateMs >= 500) {
    lastLcdUpdateMs = now;
    updateDisplay();
  }
}

