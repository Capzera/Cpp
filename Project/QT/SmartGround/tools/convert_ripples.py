import re, pathlib

root = pathlib.Path(r"d:\ProgramCpp_PLACEHOLDER")
root = pathlib.Path(r"d:\Program\Cpp\Project\QT\SmartGround\src\modules")

call_re = re.compile(r"playRipple\(")

def split_args(s, start):
    args = []
    depth = 0
    cur = []
    i = start
    while i < len(s):
        c = s[i]
        if c in "([{":
            depth += 1
        elif c in ")]}":
            if depth == 0:
                args.append("".join(cur).strip())
                return args, i
            depth -= 1
        if c == "," and depth == 0:
            args.append("".join(cur).strip())
            cur = []
        else:
            cur.append(c)
        i += 1
    return None, -1

changed = []
for path in sorted(root.glob("*.cpp")):
    text = path.read_text(encoding="utf-8")
    orig = text
    out = []
    pos = 0
    while True:
        m = call_re.search(text, pos)
        if not m:
            out.append(text[pos:])
            break
        args, end = split_args(text, m.end())
        if not args or len(args) != 4:
            out.append(text[pos:m.end()])
            pos = m.end()
            continue
        first, color, radius, dur = args
        mc = re.fullmatch(r"(?P<obj>.+)->centerPos\(\)", first)
        if not mc:
            out.append(text[pos:m.end()])
            pos = m.end()
            continue
        obj = mc.group("obj")
        mr = re.fullmatch(r"(?P<ch>.+)->cellHeight\(\)\s*\*\s*(?P<ratio>[\d.]+)", radius)
        if not mr:
            out.append(text[pos:m.end()])
            pos = m.end()
            continue
        ch, ratio = mr.group("ch"), mr.group("ratio")
        repl = (f"playCellRipple({obj}, {color}, {ratio}, {dur})")
        out.append(text[pos:m.start()])
        out.append(repl)
        pos = end + 1
    text = "".join(out)
    if text != orig:
        path.write_text(text, encoding="utf-8")
        changed.append(path.name)

print("changed:", changed)
