from PIL import Image
import os, sys

d = r"C:\Users\Administrator\.cursor\projects\d-Program-Cpp-Project-QT-SmartGround\assets"
out = r"d:\Program\Cpp\Project\QT\SmartGround\shots"


def load(tag):
    fs = [f for f in os.listdir(d) if tag in f]
    return Image.open(os.path.join(d, fs[0])).convert("RGB")


tag = sys.argv[1]
cols = int(sys.argv[2]) if len(sys.argv) > 2 else 2
rows = int(sys.argv[3]) if len(sys.argv) > 3 else 2
scale = float(sys.argv[4]) if len(sys.argv) > 4 else 3.0
im = load(tag)
w, h = im.size
cw, ch = w // cols, h // rows
for r in range(rows):
    for c in range(cols):
        box = (c * cw, r * ch, (c + 1) * cw if c < cols - 1 else w,
               (r + 1) * ch if r < rows - 1 else h)
        sub = im.crop(box).resize((int((box[2] - box[0]) * scale), int((box[3] - box[1]) * scale)), Image.LANCZOS)
        p = os.path.join(out, "g_%s_%d%d.png" % (tag, r, c))
        sub.save(p)
        print(p, sub.size)
