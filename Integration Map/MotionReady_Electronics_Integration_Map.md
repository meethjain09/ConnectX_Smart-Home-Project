# Smart Home Integration and Wiring Map

## System layout

Use two Arduino Uno controllers to keep the room and tank wiring clear. The room Uno runs `MotionReady_Room_Controller.ino`. A separate tank Uno runs `MotionReady_Tank_Controller.ino`. The original tank `.brd` already has its own Uno, LCD, HC-SR04, buzzer and status LEDs. It has no pump, pump driver or control switches, so the tank additions below complete that subsystem.

## Room controller: existing smart-home features

| Uno pin | Connect to | Notes |
|---|---|---|
| A0 | TMP36 output | TMP36 supply to 5 V; ground to common GND. |
| A1 | PIR output | PIR VCC to 5 V; GND to common GND. |
| A2 | Piezo buzzer + | Buzzer - to GND. |
| A3 | LDR divider midpoint | 5 V -> LDR -> A3 -> 1 kOhm -> GND. |
| A4 | Doorway HC-SR04 ECHO | Digital input; keeps D13 free. |
| A5 | White motion LED | A5 -> 220 Ohm -> LED anode; cathode to GND. |
| D2-D7 | Room 16x2 LCD | RS=D2, E=D3, D4-D7=D4-D7. |
| D8 | Red fan-status LED | Use its 1 kOhm current-limiting resistor. |
| D9 | SG90 sliding-door drive | Servo rotates a rack/slider along the horizontal rails; the CAD door moves sideways. |
| D10 | Lamp driver input | Use a small LED+220 Ohm in simulation or a correctly rated low-voltage driver. |
| D11 | Fan MOSFET gate | Add 220 Ohm series resistor and 10 kOhm gate-to-GND pulldown. |
| D12 | Doorway HC-SR04 TRIG | Sensor VCC to 5 V and GND to common GND. |

The doorway sensor opens the sliding door at 25 cm; the servo returns it after 3 seconds without another near reading. The track and slim pull handle in the room model replace the old swing-door arm and knob.

## Water-tank controller: added fill system

| Tank Uno pin | Connect to | Notes |
|---|---|---|
| D2-D5 | LCD data D7-D4 | LCD data pins 14,13,12,11 connect to D2,D3,D4,D5 respectively. |
| D6 | Tank HC-SR04 ECHO | Distance from tank-top sensor to water surface. |
| D7 | Tank HC-SR04 TRIG | VCC to 5 V; GND to common GND. |
| D8 | Green LED | Water is above the low threshold; use 220 Ohm. |
| D9 | Red LED | Low level or pump running; use 220 Ohm. |
| D10 | Piezo buzzer + | Buzzer - to GND. Sounds briefly when fill stops at the high threshold. |
| D11 | LCD E | LCD pin 6. |
| D12 | LCD RS | LCD pin 4. |
| A0 | AUTO slide switch | Switch between A0 and GND; ON enables automatic fill. |
| A1 | MANUAL FILL pushbutton | Button between A1 and GND; hold while AUTO is off. |
| A2 | Pump MOSFET gate | Add 220 Ohm series resistor and 10 kOhm gate-to-GND pulldown. |

The tank LCD uses the board's 10 kOhm contrast potentiometer. Keep the LCD backlight current-limiting resistor. Replace the source board's 100 Ohm LED resistors with 220 Ohm for the Uno LEDs.

### Tank control behavior

- The display calculates `water % = (tank depth - air gap) / tank depth x 100` from the top-mounted HC-SR04. Change `TANK_DEPTH_CM` in the tank sketch to the measured inside depth.
- AUTO mode starts the pump at 25% or lower and stops at 90% or higher. It holds the current state between those levels so the pump does not chatter.
- With AUTO off, hold MANUAL FILL to run the pump. It still stops at 95%.
- If the sensor gives no valid echo, the pump turns off and the screen reports a sensor error.

## Pump driver and water-safe layout

The supplied water-level board does not include a motor. Add a small low-voltage pump, a logic-level N-channel MOSFET, the 220 Ohm gate resistor, 10 kOhm pulldown, and a flyback diode across the pump. Power the pump from its own suitable supply; join supply negative to the tank Uno GND. Never power the pump from an Uno pin. Keep the pump, water and wet tubing away from exposed electronics; place the controller and connections in a splash-resistant enclosure for any physical demonstration.

## Why there are two Unos

The room controller already uses its convenient pins for the door, LCD, light, motion and fan. The supplied tank board also assigns many pins to its own LCD and sensors. Keeping the tank on its own Uno avoids pin collisions and preserves each design's wiring. In Tinkercad, add a second Uno to the same design and select each board's sketch separately.

