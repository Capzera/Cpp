import re
import sys

ROOT = r'd:\Program\Cpp\Project\QT\SmartGround'


def find_defs(text):
    res = []
    for m in re.finditer(r'\bIModule::(\w+)\s*\(', text):
        name = m.group(1)
        line_start = text.rfind('\n', 0, m.start()) + 1
        i = text.find('{', m.end())
        if i < 0:
            continue
        seg = text[m.end():i]
        if seg.count('(') - seg.count(')') != -1:
            continue
        if ';' in seg:
            continue
        depth = 0
        j = i
        ok = False
        while j < len(text):
            c = text[j]
            if c == '{':
                depth += 1
            elif c == '}':
                depth -= 1
                if depth == 0:
                    res.append((name, line_start, j + 1))
                    ok = True
                    break
            elif c in '"\'':
                q = c
                j += 1
                while j < len(text) and text[j] != q:
                    if text[j] == '\\':
                        j += 1
                    j += 1
            j += 1
        if not ok:
            continue
    return res


def main():
    path = sys.argv[1]
    text = open(path, encoding='utf-8', newline='').read().replace('\r\n', '\n')
    while True:
        defs = find_defs(text)
        seen = {}
        for name, s, e in defs:
            seen.setdefault(name, []).append((s, e))
        dups = {k: v for k, v in seen.items() if len(v) > 1}
        if not dups:
            break
        name, lst = sorted(dups.items())[0]
        keep = lst[-1]
        for s, e in reversed(lst[:-1]):
            text = text[:s] + text[e:]
    open(path, 'w', encoding='utf-8', newline='').write(text)
    defs = find_defs(text)
    seen = {}
    for name, s, e in defs:
        seen[name] = seen.get(name, 0) + 1
    print('funcs:', len(defs), 'dups:', [k for k, v in seen.items() if v > 1])


if __name__ == '__main__':
    main()
