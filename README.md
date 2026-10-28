# ConnectX Smart Home Project

A prototype smart-home system that combines room automation with an independently controlled water tank. The project includes Arduino firmware, circuit references, Fusion 360 models and parts, and build documentation for assembling and demonstrating the system.

## Project at a glance

The system is divided across two Arduino Uno controllers:

- **Room controller:** measures temperature and light, detects motion, operates a servo-driven sliding door, controls a room light and fan, and reports status on an LCD.
- **Tank controller:** measures water level with an ultrasonic sensor, displays the level, and controls automatic or manual filling through a pump driver.

The Fusion assets provide a visual, motion-ready model of the room and tank. Sensor decisions and control logic run in the Arduino/Tinkercad circuits; the Fusion model is not connected to live hardware data.

## Features

### Room

- Opens a sliding entrance when the doorway ultrasonic sensor detects an object within 25 cm; closes it after three seconds without another near reading.
- Controls the room-light output from the LDR reading and configurable darkness threshold.
- Indicates PIR motion with an LED and sounds a short entry chime on a rising motion signal.
- Switches the fan on at 28 °C and off at 26 °C, using hysteresis to reduce rapid switching.
- Displays temperature, fan, light, and door status on a 16×2 LCD.

### Water tank

- Calculates a water-level percentage from the tank depth and the ultrasonic sensor’s air-gap measurement.
- In AUTO mode, starts filling at or below 25% and stops at or above 90%.
- In MANUAL mode, fills while the button is held, with a 95% upper limit.
- Stops the pump if the sensor reading is invalid and displays a sensor fault.
- Shows fill status on an LCD and uses LEDs and a buzzer for status feedback.

### 3D model

- Includes a room, outside tank, sensors, controls, and visual component references.
- Provides separate mesh components and a Fusion archive for assembly work.
- Identifies movable parts and joint ranges in the Fusion build guide.

## Repository contents

```text
ConnectX_Smart Home Project/
├── Arduino Code/
│   ├── MotionReady_Room_Controller.ino
│   └── MotionReady_Tank_Controller.ino
├── Circuit_Schematics/
│   └── Circuit diagrams and schematics
├── Fusion_Components/
│   ├── MotionReady_*.obj
│   ├── MotionReady_Colors.mtl
│   └── Visual_Reference/
├── Fusion_Model/
│   ├── SmartHomeRoom.f3d.f3z
│   └── Fusion Merged Link.md
├── Integration Map/
│   ├── MotionReady_Electronics_Integration_Map.md
│   ├── MotionReady_Fusion_Build_Guide.md
│   ├── MotionReady_Project_Summary.md
│   └── MotionReady_Tinkercad_Assembly_Steps.md
├── Tinkercad Files/
│   ├── MotionReady_Source_*.brd
│   └── tinkercad links.md
└── Smart_Home_Source_and_Modifications_Disclosure.pdf
```

## Getting started

### Arduino firmware

1. Open the relevant `.ino` sketch in the Arduino IDE or the corresponding Tinkercad circuit.
2. Select **Arduino Uno** as the board.
3. Wire components according to the [electronics integration map](Integration%20Map/MotionReady_Electronics_Integration_Map.md).
4. Upload or run the sketch for that subsystem.
5. Set `TANK_DEPTH_CM` in the tank sketch to the measured distance from the ultrasonic sensor face to the inside bottom of the tank. Calibrate `DARK_THRESHOLD` in the room sketch for the LDR and lighting conditions.

The room and tank sketches are separate programs for separate Uno boards. They are not intended to be combined into one sketch.

### Tinkercad Circuits

Use the `.brd` files and links in [Tinkercad Files](Tinkercad%20Files/tinkercad%20links.md) as circuit references. For an integrated demonstration, use two Uno boards and assign the room and tank sketches to their respective boards. Follow the [Tinkercad assembly steps](Integration%20Map/MotionReady_Tinkercad_Assembly_Steps.md).

### Fusion 360

Open the Fusion archive in `Fusion_Model` for the supplied model, or create an assembly from the OBJ components in `Fusion_Components`. Keep `MotionReady_Colors.mtl` beside the OBJ files when importing to preserve material colors. The [Fusion build guide](Integration%20Map/MotionReady_Fusion_Build_Guide.md) describes component setup, joints, axes, and motion limits.

## Hardware and wiring notes

- The wiring tables and pin assignments in the integration map are the reference for each controller.
- The water-tank design requires a pump, logic-level N-channel MOSFET, gate resistor, gate pulldown, flyback diode, and a separate suitable pump supply. Connect the supply negative to the tank Uno ground; do not power a pump from an Uno pin.
- Use current-limiting resistors for LEDs and a suitable driver for the room lamp and fan. Keep water and wet tubing away from exposed electronics; enclose the controller and connections for physical demonstrations.
- The tank's ultrasonic sensor should be mounted above the water and aimed toward the surface. Adjust the depth constant and validate readings for the actual tank geometry.

## Behavior and implementation notes

- The room door is controlled by the doorway HC-SR04 sensor. The PIR signal drives the motion LED and entry chime.
- In the current room sketch, the lamp output is controlled by the LDR threshold independently of PIR motion. This differs from a motion-gated light behavior described in some project notes; change the sketch if motion-gated lighting is desired.
- The Fusion joints animate visual components only. The tank level indicator, for example, is not driven by the Arduino's live water-level reading.
- Pin assignments and thresholds are defined near the top of each sketch and can be adjusted for the selected components and simulation.

## Project documentation

- [Electronics integration map](Integration%20Map/MotionReady_Electronics_Integration_Map.md) — pin assignments, wiring, tank thresholds, and pump-driver guidance.
- [Fusion build guide](Integration%20Map/MotionReady_Fusion_Build_Guide.md) — mesh import, assembly structure, joints, and motion ranges.
- [Tinkercad assembly steps](Integration%20Map/MotionReady_Tinkercad_Assembly_Steps.md) — instructions for combining both controller circuits.
- [Project summary](Integration%20Map/MotionReady_Project_Summary.md) — model and subsystem overview.
- [Source and modifications disclosure](Smart_Home_Source_and_Modifications_Disclosure.pdf) — project source/modification disclosure.

## License and attribution

No license file is included in this project package. Add a license and attribution details if you plan to redistribute or publish the project.
