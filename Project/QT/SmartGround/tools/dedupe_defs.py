import re
import sys
import collections

DEF = re.compile(r'^(?:[A-Za-z_][\w:<>,\*&\[\]\s]*?\b)?([A-Za-z_]\w*)::(\w+)\s*\(', re.M)


def block_end(text, start):
    i = text.find('{', start)
    if i < 0:
        return -1
    depth = 0
    while i < len(text):
        c = text[i]
        if c == '{':
            depth += 1
        elif c == '}':
            depth -= 1
            if depth == 0:
                j = i + 1
                while j < len(text) and text[j] in '\r\n':
                    j += 1
                return j
        elif c == '"':
            i += 1
            while i < len(text) and text[i] != '"':
                if text[i] == '\\':
                    i += 1
                i += 1
        elif c == "'":
            i += 1
            while i < len(text) and text[i] != "'":
                if text[i] == '\\':
                    i += 1
                i += 1
        i += 1
    return -1


def dedupe(path):
    with open(path, encoding='utf-8', newline='') as f:
        text = f.read()
    changed = 0
    for _ in range(200):
        found = None
        for m in DEF.finditer(text):
            name = m.group(2)
            if name.startswith('~'):
                continue
            if m.group(1) != 'IModule':
                continue
            sig = m.group(0)
            found = found or {}
            found.setdefault((m.group(1), name), []).append(m.start())
        dup = None
        for k, positions in found.items():
            if len(positions) > 1:
                dup = (k, positions)
                break
        if not dup:
            break
        k, positions = dup
        keep = positions[-1]
        for p in positions:
            if p == keep:
                continue
            e = block_end(text, p)
            if e > 0:
                text = text[:p] + text[e:]
                changed += 1
    with open(path, 'w', encoding='utf-8', newline='') as f:
        f.write(text)
    return changed


if __name__ == '__main__':
    for p in sys.argv[1:]:
        n = dedupe(p)
        print(p, 'removed', n)
