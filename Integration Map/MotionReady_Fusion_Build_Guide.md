# Motion-ready Fusion build guide

This package separates the room systems into 22 colored mesh parts. The room is about 4.5 m by 4.5 m, with walls about 2.8 m high. The external tank body is about 900 mm across and 1.8 m tall. Units and positions are already in millimetres.

## Build the Fusion assembly

1. In Fusion, create a **New Design** and save it as `MotionReady_SmartHome_Assembly_v1`.
2. Choose **Mesh > Insert Mesh**. Open `Fusion_Components` and select the 22 `MotionReady_*.obj` component files together. Keep `MotionReady_Colors.mtl` beside them so Fusion can read the colors.
3. Set the unit to **Millimeter**. Leave the X, Y, Z position values at `0`; do not use **Center** or **Move to Ground**. Click **OK**. Press **F** to fit the full room and tank in view.
4. In the Browser, select the imported mesh bodies and use **Create Components from Bodies**. The file names identify the component groups. Keep the room shell, sensors, controller, tank structure, display, panel, pump housing, and entry track fixed. Ground those fixed components.
5. Leave these seven moving components ungrounded: `MotionReady_Sliding_Door`, `MotionReady_Fan_Rotor`, `MotionReady_Tank_Level_Indicator`, `MotionReady_Tank_Auto_Switch`, `MotionReady_Tank_Manual_Fill_Button`, `MotionReady_Tank_Pump_Impeller`, and `MotionReady_Tank_Valve_Handle`.
6. Use **As-Built Joint** for each moving part. Select the fixed component first and the moving component second. Pick the joint type and axis from the table below. Since the parts are already positioned, this joint type keeps them in place as the relationship is added.
7. In each joint's **Motion Limits**, enter the range below. Preview the joint before clicking **OK**. If the motion goes the opposite way, reverse the signs in that row.
8. To show motion, right-click the joint in the Browser and choose **Animate Joint**. For a chosen position, use **Drive Joint**.

## Motion setup

| Moving part | Attach to | Joint | Axis | Range from the current model position |
|---|---|---|---|---|
| Sliding door | Entry track and servo | Slider | X | -1033 mm to 0 mm; the current position is open, and -1033 mm closes it |
| Ceiling fan blades | Fan stator | Revolute | Z | Free rotation; animate the joint to show spinning |
| Tank level indicator | Tank gauge | Slider | Z | -1080 mm to +580 mm; bottom to top of the gauge |
| AUTO switch | Tank control panel | Slider | Y | 0 to 8 mm press travel, toward +Y |
| MANUAL FILL button | Tank control panel | Slider | Y | 0 to 8 mm press travel, toward +Y |
| Pump impeller | Pump housing | Revolute | Y | Free rotation; animate the joint to show pump operation |
| Tank valve handle | Pump housing | Revolute | Z | 0 to 90 degrees |

The door starts open to the right of its opening. The tank gauge indicator starts at roughly the displayed 65% level. The level indicator is a moving visual marker; it does not change the tank's volume geometry. The LCD is a visual placeholder and does not receive live Arduino data in Fusion.

## Sensor and controller behavior shown by the project

- PIR motion at the entrance: opens the sliding door and can request the room light.
- Light sensor: allows the room light to turn on only when the room is dim and motion is present.
- Temperature sensor: turns on the ceiling fan when the set temperature threshold is exceeded.
- Tank level sensor: the tank controller starts filling at or below 25%, stops at or above 90%, and stops manual filling at 95% for safety.
- Tank AUTO and MANUAL FILL buttons are shown on the outside tank panel; the LCD displays the percentage.

The room and tank Arduino sketches and the five renamed Tinkercad source boards are in the package. Fusion's joints are for a visual motion demonstration. The sensor logic runs in the Arduino/Tinkercad part of the project, not as a live simulation inside Fusion.

## If colors do not appear

Confirm `MotionReady_Colors.mtl` is still in the same folder as the OBJ files. In Fusion Preferences, check the mesh appearance option if OBJ colors are not shown. The 3MF and OBJ inside `Visual_Reference` are one-piece visual previews; use the 22 component OBJ files to build the movable assembly.

