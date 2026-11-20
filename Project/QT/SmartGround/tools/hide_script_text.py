import os
import re

MODULES = r"d:\Program\Cpp\Project\QT\SmartGround\src\modules"

NARRATIVE = {
    "m_noteText", "m_mathText", "m_resultText", "m_probeText", "m_windowText",
    "m_stateText", "m_stepText", "m_gapText", "m_verdictText", "m_heapText",
    "m_depthText", "m_scanText", "m_accText", "m_relationText", "m_wordText",
    "m_aValue", "m_bValue", "m_rValue",
}


def match_paren(text, open_idx):
    depth = 0
    i = open_idx
    in_str = False
    in_chr = False
    while i < len(text):
        c = text[i]
        if in_str:
            if c == "\\":
                i += 2
                continue
            if c == '"':
                in_str = False
        elif in_chr:
            if c == "\\":
                i += 2
                continue
            if c == "'":
                in_chr = False
        else:
            if c == '"':
                in_str = True
            elif c == "'":
                in_chr = True
            elif c == "(":
                depth += 1
            elif c == ")":
                depth -= 1
                if depth == 0:
                    return i
        i += 1
    return -1


def convert(path):
    with open(path, "r", encoding="utf-8") as f:
        text = f.read()
    orig = text

    pat = re.compile(r"(?<![\w>])(" + "|".join(sorted(NARRATIVE)) + r")->setPlainText\(")
    while True:
        m = pat.search(text)
        if not m:
            break
        name = m.group(1)
        open_idx = m.end() - 1
        close_idx = match_paren(text, open_idx)
        if close_idx < 0:
            break
        args = text[open_idx + 1:close_idx]
        replacement = f"setScriptText({name}, {args})"
        text = text[:m.start()] + replacement + text[close_idx + 1:]

    cpat = re.compile(
        r"(?<![\w>])(" + "|".join(sorted(NARRATIVE)) + r")\s*=\s*(m_scene|scene)->addText\(")
    while True:
        m = cpat.search(text)
        if not m:
            break
        name = m.group(1)
        scene = m.group(2)
        open_idx = m.end() - 1
        close_idx = match_paren(text, open_idx)
        if close_idx < 0:
            break
        args = text[open_idx + 1:close_idx]
        replacement = f"{name} = createScriptText({scene}, {args})"
        text = text[:m.start()] + replacement + text[close_idx + 1:]

    if text != orig:
        with open(path, "w", encoding="utf-8") as f:
            f.write(text)
        return True
    return False


changed = []
for name in sorted(os.listdir(MODULES)):
    if not name.endswith(".cpp"):
        continue
    if convert(os.path.join(MODULES, name)):
        changed.append(name)

print("changed:", len(changed))
for c in changed:
    print("  ", c)
