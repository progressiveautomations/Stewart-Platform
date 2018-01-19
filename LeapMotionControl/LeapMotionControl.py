import sys
import serial
import Leap
import numpy as np

# Dependencies: numpy, pyserial

NO_SERIAL = False # used for debugging if no Arduino present
FRAME_RATE = 1 # Number of frames to skip before sending/printing data    

SERIAL_PORT = '/dev/cu.usbmodem1421'
BAUD_RATE = 74880  # found to be the most stable standard rate to avoid board hang

NUM_ACTUATORS = 6

# Base actuator positions (6 1x3 vectors)
BASE_POS = np.matrix([[-246.34, 86.42, 0],
                      [-198.16, 170.38, 0],
                      [198.16, 170.38, 0],
                      [246.34, 86.42, 0],
                      [48.48, -256.80, 0],
                      [-48.48, -256.80, 0]])

# End effector positions (6 1x4 vectors)
END_EFF_POS = np.matrix([[-225.6, -73.26, 0, 1.0],
                         [-49.35, 232.01, 0, 1.0],
                         [49.35, 232.01, 0, 1.0],
                         [225.60, -73.26, 0, 1.0],
                         [176.25, -158.75, 0, 1.0],
                         [-176.25, -158.75, 0, 1.0]])

HOME_POSITION_HEIGHT = 319.0
MIN_ACTUATOR_LEN = 335.0


def assemble_serial_output(actuators):
    """
    Assembles a string to send over serial output given list of actuator positions.

    :param actuators: List of numbers representing desired actuator positions.
    :type actuators: list
    :return: String to send over serial.
    :rtype: str
    """
    s = ""
    for l in actuators:
        s += str(int(l)) + " "

    return s


class LeapListener(Leap.Listener):

    def on_init(self, controller):
        print "Initializing"
        self.frame_count = 0
        if not NO_SERIAL:
            self.ser = serial.Serial(SERIAL_PORT, BAUD_RATE)

    def on_connect(self,controller):
        print "Connected"
    
    def on_disconnect(self, controller):
        print "Disconnected"

    def on_exit(self, controller):
        """
        Called when the listener instance is removed from the controller.
        Also closes the serial connection (if in use).
        """
        print "Exited"
        if not NO_SERIAL:
            self.ser.close()

    def on_frame(self, controller):
        """
        Called when new tracking data arrives.
        """
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

                # 4x4 affine transform matrix
                transform_matrix = np.matrix([[np.cos(yaw) * np.cos(pitch), np.cos(pitch) * np.sin(yaw), -np.sin(pitch), 0],
                                              [np.cos(yaw) * np.sin(pitch) * np.sin(roll) - np.sin(yaw) * np.cos(roll), np.cos(yaw) * np.cos(roll) + np.sin(roll) * np.sin(yaw) * np.sin(pitch), np.cos(pitch) * np.sin(roll), 0],
                                              [np.cos(yaw) * np.sin(pitch) * np.cos(roll) + np.sin(yaw) * np.sin(roll), -np.cos(yaw) * np.sin(roll) + np.cos(roll) * np.sin(yaw) * np.sin(pitch), np.cos(pitch) * np.cos(roll), 0],
                                              [0, 0, 0, 1]])

                # Multiply transform matrix with end-effector joint position, add position of end-effector, add height of 'home' position to z-coordinate to compensate
                # Equation: transform_matrix * PLATFORM_POSITIONS[i].T + [x, -z, y + home, 0].T = effector_pos
                # Dims:     ( 4 x 4 )           ( 4 x 1 )                 ( 4 x 1 )                ( 4 x 1 )
                actuator_lengths = []
                for i in range(NUM_ACTUATORS):
                    effector_pos = transform_matrix * END_EFF_POS[i].T + np.matrix([pos.x, -pos.z, pos.y + HOME_POSITION_HEIGHT, 0]).T

                    # Get length of extension from min actuator length
                    # Equation: norm(effector_pos - base_pos) - MIN_ACTUATOR_LEN = rel_length
                    # Dims:          ( 3 x 1 )      ( 3 x 1 )      ( 1 x 1 )       ( 1 x 1 )
                    actuator_lengths.append(np.linalg.norm(effector_pos[:3,:] - BASE_POS[i].T) - MIN_ACTUATOR_LEN)

                # Assemble into serial output string
                ser_string = assemble_serial_output(actuator_lengths)

                # Limit output rate, print and send string
                if self.frame_count > FRAME_RATE:
                    print ser_string

                    if not NO_SERIAL:
                        self.ser.write(ser_string)

                    self.frame_count = 0
                else:
                    self.frame_count += 1

def main():

    # Initialize listener and controller
    listener = LeapListener()
    controller = Leap.Controller()

    controller.add_listener(listener)

    # Required to keep tracking going
    raw_input("Press enter to exit...\n")

    controller.remove_listener(listener)

if __name__ == "__main__":
    main()
