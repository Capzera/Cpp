from PIL import Image
import os

d = r"C:\Users\Administrator\.cursor\projects\d-Program-Cpp-Project-QT-SmartGround\assets"
out = r"d:\Program\Cpp\Project\QT\SmartGround\shots"


def load(tag):
    fs = [f for f in os.listdir(d) if tag in f]
    return Image.open(os.path.join(d, fs[0])).convert("RGB")


for t in ["29b4fe2b", "b7a88b6a", "109c3d9a", "e0620fad", "1cab74bc"]:
    im = load(t)
    im.resize((im.width * 3, im.height * 3), Image.LANCZOS).save(os.path.join(out, "z_" + t + ".png"))
    print(t, "ok")
