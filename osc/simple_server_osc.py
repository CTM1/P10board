from pythonosc.dispatcher import Dispatcher
from pythonosc.osc_server import BlockingOSCUDPServer
import serial

ser = serial.Serial(port='/dev/ttyACM0', baudrate=9600, timeout=.1)


def print_handler(address, *args):
    for elem in args:
        print("->", elem, type(elem))
        ser.write(elem)
    print(f"{address}: {args}")


def default_handler(address, *args):
    print(f"DEFAULT {address}: {args}")


dispatcher = Dispatcher()
dispatcher.map("/something/*", print_handler)
dispatcher.set_default_handler(default_handler)

ip = "127.0.0.1"
port = 1337

server = BlockingOSCUDPServer((ip, port), dispatcher)
server.serve_forever()  # Blocks forever
