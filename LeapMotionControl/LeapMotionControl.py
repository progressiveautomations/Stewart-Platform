import sys
import serial
import Leap
import numpy as np

# Dependencies: numpy, serial

# Serial-related constants
SERIAL_PORT = "COM3"
BAUD_RATE = 115200

# Platform position-related matrices and constants

# Base actuator positions (6 x 3)
BASE_POSITIONS = np.matrix([[-246.34, 86.42, 0],
                            [-198.16, 170.38, 0],
                            [198.16, 170.38, 0],
                            [246.34, 86.42, 0],
                            [48.48, -256.80, 0],
                            [-48.48, -256.80, 0]])

# End effector positions (6 x 4)
PLATFORM_POSITIONS = np.matrix([[-225.6, -73.26, 0, 1.0],
                                [-49.35, 232.01, 0, 1.0],
                                [49.35, 232.01, 0, 1.0],
                                [225.60, -73.26, 0, 1.0],
                                [176.25, -158.75, 0, 1.0],
                                [-176.25, -158.75, 0, 1.0]])

HOME_POSITION_HEIGHT = 319.0
MIN_ACTUATOR_LEN = 335.0

NUM_ACTUATORS = 6


class LeapListener(Leap.Listener):

    def on_init(self, controller):
        print "Initializing"
        # self.ser = serial.Serial(SERIAL_PORT, BAUD_RATE)

    def on_connect(self,controller):
        print "Connected"
    
    def on_disconnect(self, controller):
        print "Disconnected"

    def on_exit(self, controller):
        print "Exited"

    def on_frame(self, controller):
        frame = controller.frame()
        if len(frame.hands) == 1:
            hand = frame.hands.rightmost
            if hand.is_valid:
                pos = hand.palm_position
                pitch = hand.direction.pitch
                roll = hand.direction.roll
                yaw = hand.direction.yaw

                # 4x4 affine transform matrix
                transform_matrix = np.matrix([[np.cos(yaw) * np.cos(pitch), np.cos(pitch) * np.sin(yaw), -np.sin(pitch), 0],
                                              [np.cos(yaw) * np.sin(pitch) * np.sin(roll) - np.sin(yaw) * np.cos(roll), np.cos(yaw) * np.cos(roll) + np.sin(roll) * np.sin(yaw) * np.sin(pitch), np.cos(pitch) * np.sin(roll), 0],
                                              [np.cos(yaw) * np.sin(pitch) * np.cos(roll) + np.sin(yaw) * np.sin(roll), -np.cos(yaw) * np.sin(roll) + np.cos(roll) * np.sin(yaw) * np.sin(pitch), np.cos(pitch) * np.cos(roll), 0],
                                              [0, 0, 0, 1]])

                # Multiply transform matrix with end-effector joint position, add position of end-effector, add height of 'home' position to z-coordinate to compensate
                # Equation: transform_matrix * PLATFORM_POSITIONS[i].T + [x, -z, y + home].T
                actuator_lengths = []
                for i in range(NUM_ACTUATORS):
                    effector_pos = transform_matrix * PLATFORM_POSITIONS[i].T + np.matrix([pos.x, -pos.z, pos.y + HOME_POSITION_HEIGHT, 0]).T
                    actuator_lengths[i] = np.linalg.norm(effector_pos[:3] - BASE_POSITIONS[i].T) - MIN_ACTUATOR_LEN;

                print actuator_lengths

def main():
    listener = LeapListener()
    controller = Leap.Controller()

    controller.add_listener(listener)

    raw_input("Press enter to exit...")

    controller.remove_listener(listener)

if __name__ == "__main__":
    main()
