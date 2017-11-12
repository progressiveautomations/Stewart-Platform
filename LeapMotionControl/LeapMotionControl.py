import sys
import serial
import Leap
import numpy as np

# Dependencies: numpy, serial
base_positions = np.matrix([ [ -246.34,86.42,0 ],[ -198.16,170.38,0 ],[ 198.16,170.38,0 ],[ 246.34,86.42,0 ],[ 48.48,-256.80,0 ],[ -48.48,-256.80,0 ] ])
platform_positions = np.matrix([ [ -225.6,-73.26,0, 1.0 ],[ -49.35,232.01,0,1.0 ],[ 49.35,232.01,0,1.0 ],[ 225.60,-73.26,0,1.0 ],[ 176.25,-158.75,0,1.0 ],[ -176.25,-158.75,0,1.0 ] ])

class LeapListener(Leap.Listener):

    def on_init(self, controller):
        print "Initializing"
        self.ser = serial.Serial("COM3", 151200)

    def on_connect(self,controller):
        print "Connected"
        

def main():
    listener = LeapListener()
    controller = Leap.Controller()

    controller.add_listener(listener)

    raw_input("Press enter to exit...")

    controller.remove_listener(listener)

if __name__ == "__main__":
    main()
