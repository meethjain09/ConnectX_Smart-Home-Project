# Smart Home Automation — motion-ready project summary

This project presents a colorful, approximately 4.5 m × 4.5 m smart room with four walls, an outside-facing entry sign, a sliding entrance, a ceiling fan, room light, PIR motion sensor, light sensor, temperature sensor, controller, and entry buzzer. A prominent cylindrical water tank is placed outside the room. Its front panel shows AUTO and MANUAL FILL controls, a level gauge, and a percentage display.

The Fusion package has separate colored mesh files for the room shell and each system. Movable parts are separated so a Fusion assembly can show the door sliding, fan and pump impeller rotating, tank level marker travelling up or down, the two tank buttons pressing, and the valve handle turning. The exact joints and motion limits are in `MotionReady_Fusion_Build_Guide.md`.

The controller sketches are separate for the room and the water tank. The room sketch handles motion-based door/light behavior and temperature-based fan behavior. The tank sketch reads the ultrasonic water-level sensor, updates the percentage display, and controls automatic or manual filling. The Tinkercad `.brd` files are included with renamed filenames so they do not overwrite the originals.

The moving display in Fusion is a visual demonstration. The live sensor decisions and percentage calculation are handled by the controller code and Tinkercad circuits; Fusion does not run that electronics simulation.
