import os
import re

MODULES = r"d:\Program\Cpp\Project\QT\SmartGround\src\modules"


def match_paren(text, open_idx):
    depth = 0
    i = open_idx
    in_str = False
    while i < len(text):
        c = text[i]
        if in_str:
            if c == "\\":
                i += 2
                continue
            if c == '"':
                in_str = False
        else:
            if c == '"':
                in_str = True
            elif c == "(":
                depth += 1
            elif c == ")":
                depth -= 1
                if depth == 0:
                    return i
        i += 1
    return -1


def split_ternary(arg):
    depth = 0
    in_str = False
    i = 0
    qpos = -1
    while i < len(arg):
        c = arg[i]
        if in_str:
            if c == "\\":
                i += 2
                continue
            if c == '"':
                in_str = False
        else:
            if c == '"':
                in_str = True
            elif c == "(":
                depth += 1
            elif c == ")":
                depth -= 1
            elif c == "?" and depth == 0 and qpos < 0:
                qpos = i
            elif c == ":" and depth == 0 and qpos >= 0:
                return arg[:qpos].strip(), arg[qpos + 1:].strip()
        i += 1
    return None, None


pat = re.compile(r"(?<![\w>])createScriptText\(")

changed = []
for name in sorted(os.listdir(MODULES)):
    if not name.endswith(".cpp"):
        continue
    path = os.path.join(MODULES, name)
    with open(path, "r", encoding="utf-8") as f:
        text = f.read()
    orig = text
    out = []
    pos = 0
    while True:
        m = pat.search(text, pos)
        if not m:
            break
        open_idx = m.end() - 1
        close_idx = match_paren(text, open_idx)
        if close_idx < 0:
            break
        args = text[open_idx + 1:close_idx]
        if "," not in args:
            pos = close_idx + 1
            continue
        head, rest = args.split(",", 1)
        cond, alt = split_ternary(rest)
        if not (cond and alt):
            pos = close_idx + 1
            continue
        if not cond.startswith("m_valid") or not alt.startswith('"'):
            pos = close_idx + 1
            continue
        semi = text.find(";", close_idx)
        if semi < 0:
            pos = close_idx + 1
            continue
        line_start = text.rfind("\n", 0, m.start()) + 1
        indent = text[line_start:m.start()]
        if indent.strip():
            pos = close_idx + 1
            continue
        item = head.strip()
        new_call = f"createScriptText({item}, {cond} : \"\")"
        tail = f"\n{indent}if (!m_valid) setStatusText({item}, {alt});"
        out.append(text[pos:m.start()])
        out.append(new_call)
        out.append(tail)
        pos = semi + 1
    out.append(text[pos:])
    text = "".join(out)
    if text != orig:
        with open(path, "w", encoding="utf-8") as f:
            f.write(text)
        changed.append(name)

print("changed:", len(changed))
for c in changed:
    print("  ", c)
