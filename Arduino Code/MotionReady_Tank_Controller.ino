#include <LiquidCrystal.h>
#include <stdio.h>

// Separate controller for the outside water tank.
// Pin assignments match the supplied Water Level Motor board, with the missing
// pump driver and two control switches added on unused analog pins.
const byte PIN_AUTO_SWITCH = A0;  // Slide switch to GND means AUTO is enabled.
const byte PIN_MANUAL_FILL = A1;  // Hold pushbutton to GND to fill in MANUAL mode.
const byte PIN_PUMP_GATE   = A2;  // MOSFET gate through 220 ohm; add 10k pulldown.
const byte PIN_GREEN_LED   = 8;   // Water level is safe / tank has water.
const byte PIN_RED_LED     = 9;   // Low water or pump is running.
const byte PIN_BUZZER      = 10;
const byte PIN_ECHO        = 6;
const byte PIN_TRIG        = 7;

// Source board LCD wiring: RS, E, D4, D5, D6, D7.
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Measure from the ultrasonic sensor face to the tank's inside bottom.
// Change this to the actual inside depth used in the model/simulation.
const float TANK_DEPTH_CM = 150.0;
const float AUTO_START_PERCENT = 25.0;
const float AUTO_STOP_PERCENT = 90.0;
const float MANUAL_HIGH_LIMIT_PERCENT = 94.0;
const unsigned long SENSOR_INTERVAL_MS = 500;
const unsigned long LCD_INTERVAL_MS = 500;

float waterPercent = 0.0;
bool sensorValid = false;
bool pumpOn = false;
unsigned long lastSensorReadMs = 0;
unsigned long lastLcdUpdateMs = 0;

bool readWaterPercent(float &percentOut) {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(3);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // If no echo returns, stop filling rather than risk an overflow.
  unsigned long pulseUs = pulseIn(PIN_ECHO, HIGH, 30000UL);
  if (pulseUs == 0) return false;

  float airGapCm = pulseUs / 58.0;
  if (airGapCm < 2.0 || airGapCm > TANK_DEPTH_CM + 10.0) return false;

  float level = (TANK_DEPTH_CM - airGapCm) * 100.0 / TANK_DEPTH_CM;
  if (level < 0.0) level = 0.0;
  if (level > 100.0) level = 100.0;
  percentOut = level;
  return true;
}

void printLcdLine(byte row, const char *message) {
  lcd.setCursor(0, row);
  byte i = 0;
  while (i < 16 && message[i] != '\0') {
    lcd.print(message[i]);
    i++;
  }
  while (i < 16) {
    lcd.print(' ');
    i++;
  }
}

void updateDisplay() {
  char line[17];
  if (!sensorValid) {
    printLcdLine(0, "SENSOR FAULT");
    printLcdLine(1, "PUMP STOPPED");
    return;
  }

  int shownPercent = (int)(waterPercent + 0.5);
  snprintf(line, sizeof(line), "WATER: %3d%%", shownPercent);
  printLcdLine(0, line);

  const bool autoEnabled = digitalRead(PIN_AUTO_SWITCH) == LOW;
  const bool manualRequested = digitalRead(PIN_MANUAL_FILL) == LOW;
  if (pumpOn && autoEnabled) printLcdLine(1, "AUTO: FILLING");
  else if (pumpOn && manualRequested) printLcdLine(1, "MANUAL: FILL");
  else if (autoEnabled) printLcdLine(1, "AUTO: HOLD");
  else printLcdLine(1, "MANUAL: IDLE");
}

void setup() {
  pinMode(PIN_AUTO_SWITCH, INPUT_PULLUP);
  pinMode(PIN_MANUAL_FILL, INPUT_PULLUP);
  pinMode(PIN_PUMP_GATE, OUTPUT);
  pinMode(PIN_GREEN_LED, OUTPUT);
  pinMode(PIN_RED_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  digitalWrite(PIN_TRIG, LOW);
  digitalWrite(PIN_PUMP_GATE, LOW);
  digitalWrite(PIN_GREEN_LED, LOW);
  digitalWrite(PIN_RED_LED, LOW);

  lcd.begin(16, 2);
  lcd.clear();
  printLcdLine(0, "SMART WATER TANK");
  printLcdLine(1, "Starting...");
}

void loop() {
  unsigned long now = millis();
  if (now - lastSensorReadMs >= SENSOR_INTERVAL_MS) {
    lastSensorReadMs = now;
    float newPercent = 0.0;
    sensorValid = readWaterPercent(newPercent);
    if (sensorValid) waterPercent = newPercent;

    bool autoEnabled = digitalRead(PIN_AUTO_SWITCH) == LOW;
    bool manualRequested = digitalRead(PIN_MANUAL_FILL) == LOW;
    bool wasPumpOn = pumpOn;

    if (!sensorValid) {
      pumpOn = false;
    } else if (autoEnabled) {
      // Hysteresis: start low, stop high, and hold the current state between.
      if (!pumpOn && waterPercent <= AUTO_START_PERCENT) pumpOn = true;
      if (pumpOn && waterPercent >= AUTO_STOP_PERCENT) pumpOn = false;
    } else {
      // Manual filling still has a hard high-level stop.
      pumpOn = manualRequested && waterPercent < MANUAL_HIGH_LIMIT_PERCENT;
    }

    digitalWrite(PIN_PUMP_GATE, pumpOn ? HIGH : LOW);
    digitalWrite(PIN_RED_LED, (pumpOn || (sensorValid && waterPercent <= AUTO_START_PERCENT)) ? HIGH : LOW);
    digitalWrite(PIN_GREEN_LED, (sensorValid && waterPercent > AUTO_START_PERCENT && !pumpOn) ? HIGH : LOW);

    if (wasPumpOn && !pumpOn && sensorValid && waterPercent >= AUTO_STOP_PERCENT) {
      tone(PIN_BUZZER, 1900, 180); // short notification when automatic fill stops
    }
  }

  if (now - lastLcdUpdateMs >= LCD_INTERVAL_MS) {
    lastLcdUpdateMs = now;
    updateDisplay();
  }
}


