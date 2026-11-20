import sys
from PIL import Image
import ctypes

src = sys.argv[1]
dst = sys.argv[2]
box = [int(v) for v in sys.argv[3:7]] if len(sys.argv) > 6 else None
scale = float(sys.argv[7]) if len(sys.argv) > 7 else 1.0

img = Image.open(src)
if box:
    img = img.crop(box)
if scale != 1.0:
    img = img.resize((int(img.width*scale), int(img.height*scale)), Image.LANCZOS)
img.save(dst)
print("saved", dst, img.size)
