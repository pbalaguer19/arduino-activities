# PI (maestro) solicita 3 bytes a un Arduino (esclavo)
# [pi] bytes = bus.read_i2c_block_data(address,0,3) -> Wire.onRequest(handler);

import smbus
import time
from enum import Enum
import socket
from ctypes import *
import datetime

""" This class defines a C-like struct """
class timeval(Structure):
    _fields_ = [("tv_sec", c_long), ("tv_usec", c_long)]

class State(Structure):
    _fields_ = [("direction", c_uint32),
                ("acceleration", c_float),
                ("heartrate", c_float),
                ("time", timeval)]

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
heartrate = 0

SERVER_ADDR = ('192.168.10.2', 32987)

def readString():
    bytes = []
    try:
        bytes = bus.read_i2c_block_data(address, 0, 32)
    except IOError:
        pass

    return bytes

def process_data(text):
    global movement, heartrate

    if text.split("/")[0] is not text:
        pitch = float(text.split("/")[0])
        roll = float(text.split("/")[1].split(" ")[0])

        heartrate = int(text.split(" ")[1])


        if abs(roll - BASE_ROLL) > abs(pitch - BASE_PITCH):
            if roll < BASE_ROLL - 5: movement = Movement.DOWN
            if roll > BASE_ROLL + 5: movement = Movement.UP
        else:
            if pitch < BASE_PITCH - 5: movement = Movement.RIGHT
            if pitch > BASE_PITCH + 5:
                movement = Movement.LEFT

def TimestampMillisec64():
    return int((datetime.datetime.utcnow() - datetime.datetime(1970, 1, 1)).total_seconds() * 1000)

def TimestampSec64():
    return int((datetime.datetime.utcnow() - datetime.datetime(1970, 1, 1)).total_seconds())

def send_data():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    t = timeval(TimestampSec64(), TimestampMillisec64())
    state = State(movement.value, 0.0, heartrate, t)
    sock.sendto(state, SERVER_ADDR)

while True:

    time.sleep(0.3)
    bytes = readString()

    string = ""
    i = 0

    while i < len(bytes) and chr(bytes[i]) != '%':
        string += chr(bytes[i])
        i += 1

    process_data(string)
    print "Movement: {}  Heartrate: {}".format(movement, heartrate)
    if movement:
        send_data()
