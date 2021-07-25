import sys
import numpy
from PIL import Image

if (len(sys.argv) != 2):
    print("Usage:\npython3 img2matrix.py [FILE]")
    sys.exit(2)

image = Image.open(sys.argv[1])
image.convert("RGB")
pixel_values = []
pixel_values = list(image.getdata())

black_and_white = []

#for item in pixel_values:
#    for i in range (0, 3):
#        if (item[0] != 0):
#           black_and_white.append(item[0]);
#        else:
#            black_and_white.append(0);

a = 0
b = 0


for i, item in enumerate(pixel_values):
    if (i % 2) == 0:
        a = (item[0] >> 4) & 0xF
    else:
        b = (item[0] >> 4) & 0xF
        black_and_white.append((b << 4) | a )

print(str(black_and_white).strip('[]'));
