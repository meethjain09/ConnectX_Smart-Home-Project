# Build the combined Tinkercad Circuits design

The package has two coordinated Arduino controllers: one for the room and one for the external tank. Use the included `.brd` exports as references, then build one Tinkercad Circuits design with two Arduino Uno boards and separate sketches.

## Add the room controller

1. Add the room Uno and its breadboard.
2. Wire the door HC-SR04, SG90 servo, LDR/lamp, PIR/buzzer/LED, TMP36/LCD and fan MOSFET using the room section of `MotionReady_Electronics_Integration_Map.md`.
3. Select the room Uno in the Code panel and paste `MotionReady_Room_Controller.ino`.

## Add the tank controller

1. Add a second Uno and breadboard beside the room circuit.
2. Recreate the supplied tank board: HC-SR04, 16x2 LCD and contrast potentiometer, red/green LEDs, buzzer, plus the AUTO and MANUAL FILL switches.
3. Add the missing pump path: low-voltage pump, logic-level N-MOSFET, 220 Ohm gate resistor, 10 kOhm gate pulldown and flyback diode. Use a separate pump supply and connect its negative to the tank Uno GND.
4. Wire it using the tank section of `MotionReady_Electronics_Integration_Map.md`.
5. Select the tank Uno in the Code panel and paste `MotionReady_Tank_Controller.ino`.

## Show the behavior

- Set `TANK_DEPTH_CM` to the sensor-to-bottom distance. The LCD displays the calculated percentage.
- In AUTO, the pump starts at 25% or lower and stops at 90%; the red/green LEDs show status.
- Turn AUTO off and hold MANUAL FILL to run the pump; it still stops at 95%.
- The room controller independently opens the sliding door, controls the light and fan, and reports room status on its LCD.

Save the Tinkercad design after wiring both boards. If your Code panel does not let you choose each Uno separately, keep the tank controller as a second linked Tinkercad circuit rather than combining both sketches into one Arduino program. In Fusion, import the updated room-and-tank 3MF and save your `.f3d` file.

