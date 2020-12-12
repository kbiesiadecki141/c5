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

LED_SERVICE_UUID = "32e61089-2b22-4db5-a914-43ce41986c70"
LED_CHAR_UUIDS   = "32e68911-2b22-4db5-a914-43ce41986c70"

CHATTER_SERVICE_UUID = "32e60000-2b22-4db5-a914-43ce41986c70"
CHATTER_CHAR_UUID    = "32e60001-2b22-4db5-a914-43ce41986c70"
'''
{0x70,0x6C,0x98,0x41,0xCE,0x43,0x14,0xA9,
                0xB5,0x4D,0x22,0x2B,0x89,0x10,0xE6,0x32}
'''

class RobotController():

    def __init__(self, address):

        print("connecting...")
        self.robot = Peripheral(addr)
        print("connected")

        # keep state for keypresses
        self.pressed = {"up": False, "down": False, "right": False, "left": False}

        # Get service
        sv = self.robot.getServiceByUUID(LED_SERVICE_UUID)
        # Get characteristic
        self.ch = sv.getCharacteristics(LED_CHAR_UUIDS)[0]

        # Get service
        sv = self.robot.getServiceByUUID(CHATTER_SERVICE_UUID)
        # Get characteristic
        self.chatter_char = sv.getCharacteristics(CHATTER_CHAR_UUID)[0]

        keyboard.hook(self.on_key_event)

    def on_key_event(self, event):
        # print key name
        print(event.name)
        # if a key unrelated to direction keys is pressed, ignore
        if event.name not in self.pressed: return
        # if a key is pressed down
        if event.event_type == keyboard.KEY_DOWN:
            # if that key is already pressed down, ignore
            if self.pressed[event.name]: return
            # set state of key to pressed
            self.pressed[event.name] = True

            # TODO write to characteristic to change direction
            if event.name == "up":
              print("potato") 
 
            led_state = bool(int(self.ch.read().hex()))
            self.ch.write(bytes([not led_state]))

            self.chatter_char.write(bytes("Greetings", 'utf-8'))

        else:
            # set state of key to released
            self.pressed[event.name] = False

            # TODO write to characteristic to stop moving in this direction

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        self.robot.disconnect()

with RobotController(addr) as robot:
    getpass('Use arrow keys to control robot')
