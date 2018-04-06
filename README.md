# Stewart Platform

# Quick Start Guide
1. Ensure that power to the platform is turned off, the USB cable from the platform is unplugged from the computer, and all GUI instances are closed.
2. (if Leap Motion is to be used) Ensure that the Leap Motion service and software is started, and the controller is plugged into the computer.
3. Turn on the power to the platform, and plug the USB cable from the platform into the computer.
4. Launch the GUI; in the 'Options' menu, click "Select COM Port".
5. Find the COM port corresponding to the Arduino Due (should show up in the Device Description on the left hand side, as shown), and change the baud rate to 115200.
6. Click connect, and the platform should immediately begin its calibration routine by moving all actuators to full extension, and retracting to minimum position. DO NOT SEND ANY COMMANDS TO THE PLATFORM DURING THIS PHASE!
7. Check the beginning of the log to ensure calibration has succeeded.
8. Manually control the actuators by moving the sliders and/or entering values into the corresponding textbox. To enable Leap Motion, ensure its status is "Connected", and tick the "Enable Leap Motion" checkbox above the sliders.

# Assembly Instructions
Before assembling, ensure that all the wires are adequately connected within the enclosure, including the power supply wires, the actuator power wires, the actuator signal wires, and the Arduino Due wires.

[Pictures of wiring]

Attach the 3 platform legs to the 3 corners of the base plate (Note: the base plate is larger than the top plate, and has 4 slots cut into it.), such that the 6 metal flanges are facing upwards. This is done using the ¾” long, ⅜”-16 threaded bolts, with one washer.

[Picture of base leg]

Next, position the enclosure underneath the base, so that the enclosure posts align with the slots in the base plate, with the power and USB connectors facing out as shown.

[Enclosure picture]

Attach the enclosure to the base place using 4 1” long,  ¼”-20 threaded bolts, with a washer for each. Ensure the bolts are aligned in the slots such that the corners of the enclosure do not extend beyond the edge of the base plates, like shown.

[Enclosure corner]

Attach the actuators as shown in the following picture. Close-up views of the joints are provided. Use 2 washers (one on each side), a lock-washer, and a ¼”-28 nut.

[Joint pictures]

To avoid problems, ensure that the actuator gearboxes are facing upwards as shown in the picture.

[Picture of actuator positions]

Once the platform is assembled, attach the maze to the top plate by aligning the 3 corner holes on each, and secure it with 3 1¼” long, ⅜”-16 threaded bolts, with one washer on each, and a nut.

[Maze bolt pics]

Connect all the actuator connectors to the appropriate receptacles on the enclosure, by routing the cables around and under the base plate. These can be bundled to be neat and out-of-view.

[Pic of cables and connectors]

Finally, connect the power and USB cables to the front of the enclosure, and connect the other end of the USB cable. The platform is now assembled!

Troubleshooting

Not working? Check all power connectors.

Arduino not seen by computer? Check cables.

# Software Architecture

# Troubleshooting

## Qt GUI build instructions
For Leap Motion control with Python: Python 2.7 x64 on Windows/MacOS (NB: for MacOS use version downloaded from python.org), with numpy and pyserial installed.

For GUI: Qt 5 (5.10.1 recommended), with serialport module in addition to the core libraries
