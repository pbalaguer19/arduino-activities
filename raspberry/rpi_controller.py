# PI (maestro) solicita 3 bytes a un Arduino (esclavo)
# [pi] bytes = bus.read_i2c_block_data(address,0,3) -> Wire.onRequest(handler);

import smbus
import time
from enum import Enum

class Movement(Enum):
    UP = 0
    DOWN = 1
    RIGHT = 2
    LEFT = 3

BASE_PITCH = 0.0
BASE_ROLL = -5.0

bus = smbus.SMBus(1)
address = 0x04

movement = None
acceleration = 0

def readString():
    bytes = []
    try:
        bytes = bus.read_i2c_block_data(address, 0, 32)
    except IOError:
        pass

    return bytes

def process_data(text):
    global movement, acceleration

    if text.split("/")[0] is not text:
        pitch = float(text.split("/")[0])
        roll = float(text.split("/")[1].split(" ")[0])

        acceleration = int(text.split(" ")[1])


        if abs(roll - BASE_ROLL) > abs(pitch - BASE_PITCH):
            if roll < BASE_ROLL - 5: movement = Movement.DOWN
            if roll > BASE_ROLL + 5: movement = Movement.UP
        else:
            if pitch < BASE_PITCH - 5: movement = Movement.RIGHT
            if pitch > BASE_PITCH + 5: rmovement = Movement.LEFT

while True:

    time.sleep(0.3)
    bytes = readString()

    string = ""
    i = 0

    while i < len(bytes) and chr(bytes[i]) != '%':
        string += chr(bytes[i])
        i += 1

    process_data(string)
    print "Movement: {}  Acc: {}".format(movement, acceleration)
