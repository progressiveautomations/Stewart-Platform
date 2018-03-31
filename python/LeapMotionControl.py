import sys
sys.dont_write_bytecode = True

import Leap
import numpy as np
import serial
import time

# Serial constants
USE_SERIAL = True  # set False for debugging if no Arduino present
SERIAL_PORT = 'COM7'
BAUD_RATE = 115200  # found to be the most stable standard rate to avoid board hang

# Platform position matrices and constants
NUM_ACTUATORS = 6
HOME_POSITION_HEIGHT = 330.0
MIN_ACTUATOR_LEN = 360.0
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

                # Calculate sin,cos of RPY values for matrix entries (for reuse)
                cos_roll = np.cos(roll)
                cos_pitch = np.cos(pitch)
                cos_yaw = np.cos(yaw)
                sin_roll = np.sin(roll)
                sin_pitch = np.sin(pitch)
                sin_yaw = np.sin(yaw)

                # 4x4 affine transform matrix, listed by row entry for easier reading
                transform_matrix = np.matrix([
                    [
                        cos_yaw * cos_pitch,
                        cos_pitch * sin_yaw,
                        -sin_pitch,
                        0
                    ],

                    [
                        cos_yaw * sin_pitch * sin_roll - sin_yaw * cos_roll,
                        cos_yaw * cos_roll + sin_roll * sin_yaw * sin_pitch,
                        cos_pitch * sin_roll,
                        0
                    ],

                    [
                        cos_yaw * sin_pitch * cos_roll + sin_yaw * sin_roll,
                        -cos_yaw * sin_roll + cos_roll * sin_yaw * sin_pitch,
                        cos_pitch * cos_roll,
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

                # Assemble actuator lengths into a serial output string (space-separated)
                ser_string = ''.join([' '.join([str(int(l)) for l in actuator_lengths]), '\n'])

                # Print and send the input string
                print ser_string.strip()  # print for debug; don't need line endings in this case
                if USE_SERIAL:
                    try:
                        self.ser.write(ser_string)
                    except serial.SerialTimeoutException, serial.SerialException:  # try to reopen the device under timeout
                        try:
                            self.ser.close()
                            self.ser.open()
                        except serial.SerialException:
                            pass  # unable to reconnect, try again next pass (may raise exception for some abnormal error)


def main():
    '''
    Initialize Leap Motion listener and controller.
    Close the controller and script after receiving prompt.
    '''
    listener = LeapListener()
    controller = Leap.Controller()

    controller.add_listener(listener)

    raw_input("Press enter to exit...\n")

    controller.remove_listener(listener)


if __name__ == '__main__':
    main()
