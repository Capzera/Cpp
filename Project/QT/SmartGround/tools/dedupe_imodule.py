import re
import sys

path = sys.argv[1]
text = open(path, 'rb').read().decode('utf-8', errors='replace').replace('\r\n', '\n')


def find_defs(t):
    res = []
    for m in re.finditer(r'^[^\n;{}]*\bIModule::(\w+)\s*\(', t, re.M):
        name = m.group(1)
        ls = t.rfind('\n', 0, m.start()) + 1
        i = t.find('{', m.end())
        if i < 0:
            continue
        seg = t[m.end():i]
        if seg.count('(') - seg.count(')') != -1 or ';' in seg:
            continue
        depth = 0
        j = i
        ok = False
        while j < len(t):
            c = t[j]
            if c == '{':
                depth += 1
            elif c == '}':
                depth -= 1
                if depth == 0:
                    res.append((name, ls, j + 1))
                    ok = True
                    break
            elif c in '"\'':
                q = c
                j += 1
                while j < len(t) and t[j] != q:
                    if t[j] == '\\':
                        j += 1
                    j += 1
            j += 1
        if not ok:
            continue
    return res


def grab(t, name):
    for n, s, e in find_defs(t):
        if n == name:
            return t[s:e]
    return None


target = sys.argv[2] if len(sys.argv) > 2 else None

defs = find_defs(text)
seen = {}
for name, s, e in defs:
    seen.setdefault(name, []).append((s, e))

dups = {k: v for k, v in seen.items() if len(v) > 1}
for name, lst in dups.items():
    if target and name != target:
        continue
    keep = lst[-1]
    for s, e in reversed(lst[:-1]):
        text = text[:s] + text[e:]

open(path, 'wb').write(text.encode('utf-8'))

defs = find_defs(text)
import collections
c = collections.Counter(n for n, s, e in defs)
print('funcs', len(defs), 'dups', {k: v for k, v in c.items() if v > 1})
