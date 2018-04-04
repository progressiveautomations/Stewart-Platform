# Stewart Platform

# Quick Start Guide
1. Ensure that power to the platform is turned off, the USB cable from the platform is unplugged from the computer, and all GUI instances are closed.
2. (if Leap Motion is to be used) Ensure that the Leap Motion service and software is started, and the controller is plugged into the computer.
3. Turn on the power to the platform, and plug the USB cable from the platform into the computer.
4. Launch the GUI; in the 'Options' menu, click "Select COM Port".
5. Find the COM port corresponding to the Arduino Due (should show up in the Device Description on the left hand side, as shown), and change the baud rate to 115200.
6a. Click connect, and the platform should immediately begin its calibration routine by moving all actuators to full extension, and retracting to minimum position. DO NOT SEND ANY COMMANDS TO THE PLATFORM DURING THIS PHASE!
6b. Check the beginning of the log to ensure calibration has succeeded.
7. Manually control the actuators by moving the sliders and/or entering values into the corresponding textbox. To enable Leap Motion, ensure its status is "Connected", and tick the "Enable Leap Motion" checkbox above the sliders.

# Software Architecture

# Troubleshooting

## Qt GUI build instructions
For Leap Motion control with Python: Python 2.7 x64 on Windows/MacOS (NB: for MacOS use version downloaded from python.org), with numpy and pyserial installed.

For GUI: Qt 5 (5.10.1 recommended), with serialport module in addition to the core libraries
