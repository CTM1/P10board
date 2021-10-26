from pythonosc.dispatcher import Dispatcher
from pythonosc.osc_server import BlockingOSCUDPServer
import serial
import time

ser = serial.Serial(port='/dev/ttyACM0', baudrate=2000000, timeout=.1)

def is_string(elem):
    return type(elem) is str

def print_handler(address, *args):
    for elem in args:
        if elem:
            print("->", elem, type(elem))
    print(f"{address}: {args}")


def default_handler(address, *args):
    print(f"DEFAULT {address}: {args}")

def scroll_handler(address, *args):
    charAsIntArray = [0]
    for elem in args:
        if is_string(elem):
            for char in elem:
                charAsIntArray.append(ord(char))
        print("->", elem, type(elem)) 
        ser.write(bytearray(charAsIntArray))

def draw_handler(address, *args):
    values = []
    for elem in args:
        values.append(elem)
        print("->", elem, type(elem))
    print("Values sent :")
    print(values)
    print("Bytearray sent: ")
    print(bytearray(values))
    ser.write(bytearray(values))
    print(ser.read())

dispatcher = Dispatcher()
dispatcher.map("/input/draw", draw_handler)
dispatcher.map("/text/scroll", scroll_handler)
dispatcher.set_default_handler(default_handler)

ip = "127.0.0.1"
port = 1337

server = BlockingOSCUDPServer((ip, port), dispatcher)
server.serve_forever()  # Blocks forever
