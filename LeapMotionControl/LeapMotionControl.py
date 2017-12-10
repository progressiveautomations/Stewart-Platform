import sys
sys.dont_write_bytecode = True

import Leap
import numpy as np
import serial
import time

# Leap Motion constants
FRAME_RATE = 1  # number of frames to skip before sending/printing data

# Serial constants
USE_SERIAL = True  # set False for debugging if no Arduino present
SERIAL_PORT = 'COM5'
BAUD_RATE = 9600

# Platform position matrices and constants
NUM_ACTUATORS = 6
HOME_POSITION_HEIGHT = 319.0
MIN_ACTUATOR_LEN = 335.0
BASE_POS = np.matrix([  # base actuator positions (6 1x3 vectors)
    [-246.34, 86.42, 0],
    [-198.16, 170.38, 0],
    [198.16, 170.38, 0],
    [246.34, 86.42, 0],
    [48.48, -256.80, 0],
    [-48.48, -256.80, 0]
])
END_EFF_POS = np.matrix([  # end effector positions (6 1x4 vectors)
    [-225.6, -73.26, 0, 1.0],
    [-49.35, 232.01, 0, 1.0],
    [49.35, 232.01, 0, 1.0],
    [225.60, -73.26, 0, 1.0],
    [176.25, -158.75, 0, 1.0],
    [-176.25, -158.75, 0, 1.0]
])


class LeapListener(Leap.Listener):

    def on_init(self, controller):
        print "-I- Initializing"
        self.frame_count = 0
        if USE_SERIAL:
            try:
                self.ser = serial.Serial(
                    port=SERIAL_PORT,
                    baudrate=BAUD_RATE,
                    write_timeout=1,  # catch any input hangs (>1s)
                    # xonxoff=1  # enable software flow control; needs further investigation for noticeable difference
                )
            except serial.SerialException:
                print "-E- Serial object unavailable! Exiting..."
                exit()

    def on_connect(self, controller):
        print "-I- Connected"
    
    def on_disconnect(self, controller):
        print "-W- Disconnected"

    def on_exit(self, controller):
        '''
        Called when the listener instance is removed from the controller.
        Also closes the serial connection (if in use).
        '''
        print "-I- Exited"
        if USE_SERIAL:
            self.ser.close()

    def on_frame(self, controller):
        '''
        Called when new tracking data arrives.
        '''
        frame = controller.frame()
        if len(frame.hands) > 0:
            # Grab rightmost hand
            hand = frame.hands.rightmost

            if hand.is_valid:
                # Get hand position, RPY data
                pos = hand.palm_position

                pitch = hand.direction.pitch
                yaw = hand.direction.yaw
                roll = hand.palm_normal.roll

                # 4x4 affine transform matrix, listed by row entry for easier reading
                transform_matrix = np.matrix([
                    [
                        np.cos(yaw) * np.cos(pitch),
                        np.cos(pitch) * np.sin(yaw),
                        -np.sin(pitch),
                        0
                    ],
                    
                    [
                        np.cos(yaw) * np.sin(pitch) * np.sin(roll) - np.sin(yaw) * np.cos(roll),
                        np.cos(yaw) * np.cos(roll) + np.sin(roll) * np.sin(yaw) * np.sin(pitch),
                        np.cos(pitch) * np.sin(roll),
                        0
                    ],

                    [
                        np.cos(yaw) * np.sin(pitch) * np.cos(roll) + np.sin(yaw) * np.sin(roll),
                        -np.cos(yaw) * np.sin(roll) + np.cos(roll) * np.sin(yaw) * np.sin(pitch),
                        np.cos(pitch) * np.cos(roll),
                        0
                    ],
                    
                    [
                        0,
                        0,
                        0,
                        1
                    ]
                ])

                # Multiply transform matrix with end-effector joint position, add position of end-effector, add height of 'home' position to z-coordinate to compensate
                # Equation: transform_matrix * PLATFORM_POSITIONS[i].T + [x, -z, y + home, 0].T = effector_pos
                # Dims:     ( 4 x 4 )           ( 4 x 1 )                 ( 4 x 1 )                ( 4 x 1 )
                actuator_lengths = []
                for i in xrange(NUM_ACTUATORS):
                    effector_pos = transform_matrix * END_EFF_POS[i].T + np.matrix([pos.x, -pos.z, pos.y + HOME_POSITION_HEIGHT, 0]).T

                    # Get length of extension from min actuator length
                    # Equation: norm(effector_pos - base_pos) - MIN_ACTUATOR_LEN = rel_length
                    # Dims:          ( 3 x 1 )      ( 3 x 1 )      ( 1 x 1 )       ( 1 x 1 )
                    actuator_lengths.append(np.linalg.norm(effector_pos[:3,:] - BASE_POS[i].T) - MIN_ACTUATOR_LEN)

                # Assemble actuator lengths into a serial output string (comma delimited, angle bracket enclosed, no spacing)
                # Include line endings for flexibility on parser side (i.e. if matching string with line endings)
                ser_string = ''.join(('<', ','.join([str(int(l)) for l in actuator_lengths]), '>\n\r'))

                # Print and send the input string
                print ser_string.strip()  # exclude line endings when printing for debug
                if USE_SERIAL:
                    try:
                        self.ser.write(ser_string)
                    except serial.SerialTimeoutException, serial.SerialException:  # try to reopen the device under timeout
                        for __ in xrange(5):  # print timeout message multiple times for visibility
                            print "-W- Timeout or port closed detected!"
                        try:
                            self.ser.close()
                            self.ser.open()
                        except serial.SerialException:
                            pass


def main():
    '''
    Initialize Leap Motion listener and controller.
    Close the controller and script after receiving prompt.
    '''
    listener = LeapListener()
    controller = Leap.Controller()
    controller.add_listener(listener)

    try:
        while True:
            listener.on_frame(controller)
            time.sleep(0.5)
    except KeyboardInterrupt:
        pass

    controller.remove_listener(listener)


if __name__ == '__main__':
    main()
