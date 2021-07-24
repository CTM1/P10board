from pythonosc.dispatcher import Dispatcher
from pythonosc.osc_server import BlockingOSCUDPServer
import serial

ser = serial.Serial(port='/dev/ttyACM0', baudrate=9600, timeout=.1)

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
    str2Send = "0,"
    for elem in args:
        if is_string(elem):
            str2Send += elem
        print("->", elem, type(elem))
    print("Formatted string sent: " + str2Send)
    ser.write(str2Send.encode())

def draw_handler(address, *args):
    str2Send = "1,"
    for elem in args:
        elem = int(elem)
        str2Send += str(elem)
        str2Send += ","
        print("->", elem, type(elem))
    print("Formatted string sent" + str2Send)
    ser.write(str2Send.encode())

dispatcher = Dispatcher()
dispatcher.map("/input/draw", draw_handler)
dispatcher.map("/text/scroll", scroll_handler)
dispatcher.set_default_handler(default_handler)

ip = "127.0.0.1"
port = 1337

server = BlockingOSCUDPServer((ip, port), dispatcher)
server.serve_forever()  # Blocks forever
