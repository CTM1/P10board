import sys
import numpy
from PIL import Image

if (len(sys.argv) != 2):
    print("Usage:\npython3 img2matrix.py [FILE]");
    sys.exit(2);

image = Image.open(sys.argv[1])
image.convert("RGB")
pixel_values = []
pixel_values = list(image.getdata())

black_and_white = []

for item in pixel_values:
    for i in range (0, 3):
        if (item[i] != 0):
            black_and_white.append(0);
        else:
            black_and_white.append(1);

print(str(black_and_white).strip('[]'));
