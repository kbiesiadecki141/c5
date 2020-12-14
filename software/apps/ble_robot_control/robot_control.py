#!/usr/bin/env python3
import struct
import time
import keyboard
from getpass import getpass
from bluepy.btle import Peripheral, DefaultDelegate
import argparse

parser = argparse.ArgumentParser(description='Print advertisement data from a BLE device')
parser.add_argument('addr', metavar='A', type=str, help='Address of the form XX:XX:XX:XX:XX:XX')
args = parser.parse_args()
addr = args.addr.lower()
if len(addr) != 17:
    raise ValueError("Invalid address supplied")

CHATTER_SERVICE_UUID = "32e60000-2b22-4db5-a914-43ce41986c70"
CHATTER_CHAR_UUID    = "32e60001-2b22-4db5-a914-43ce41986c70"

class RobotController():

    def __init__(self, address):

        print("Connecting...")
        self.robot = Peripheral(addr)
        print("Connected.")

        # keep state for keypresses
        self.pressed = {"up": False, "down": False, "right": False, "left": False, 
                        "t": False, "s": False}

        # Get service
        sv = self.robot.getServiceByUUID(CHATTER_SERVICE_UUID)
        # Get characteristic
        self.chatter_char = sv.getCharacteristics(CHATTER_CHAR_UUID)[0]

        # Listen to keyboard.
        keyboard.hook(self.on_key_event)

    def on_key_event(self, event):
        # if a key unrelated to direction keys is pressed, ignore
        if event.name not in self.pressed: return
        # if a key is pressed down
        if event.event_type == keyboard.KEY_DOWN:
            # if that key is already pressed down, ignore
            if self.pressed[event.name]: return
            # set state of key to pressed
            self.pressed[event.name] = True
            print(event.name)

            if event.name == "up":
              self.chatter_char.write(bytes("FORWARD\0", 'utf-8'))
            elif event.name == "down":
              self.chatter_char.write(bytes("BACKWARD\0", 'utf-8'))
            elif event.name == "left":
              self.chatter_char.write(bytes("TURN LEFT\0", 'utf-8'))
            elif event.name == "right":
              self.chatter_char.write(bytes("TURN RIGHT\0", 'utf-8'))

        else:
            # set state of key to released
            self.pressed[event.name] = False
            self.chatter_char.write(bytes("STOP\0", 'utf-8'))

            # TODO write to characteristic to stop moving in this direction

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        self.robot.disconnect()

with RobotController(addr) as robot:
    getpass('Use arrow keys to control robot')
