from PIL import Image
import os, sys

d = r"C:\Users\Administrator\.cursor\projects\d-Program-Cpp-Project-QT-SmartGround\assets"
out = r"d:\Program\Cpp\Project\QT\SmartGround\shots"


def load(tag):
    fs = [f for f in os.listdir(d) if tag in f]
    return Image.open(os.path.join(d, fs[0])).convert("RGB")


tag = sys.argv[1]
scale = float(sys.argv[2]) if len(sys.argv) > 2 else 3.0
box = None
if len(sys.argv) > 6:
    box = tuple(int(x) for x in sys.argv[3:7])
im = load(tag)
if box:
    im = im.crop(box)
im = im.resize((int(im.width * scale), int(im.height * scale)), Image.LANCZOS)
p = os.path.join(out, "crop_%s.png" % tag)
im.save(p)
print(p, im.size)
