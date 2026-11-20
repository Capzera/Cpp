import difflib
import json
import os
import re
import subprocess

REPO = r'D:\Program\Cpp'
ROOT = r'd:\Program\Cpp\Project\QT\SmartGround'


def find_func(text, name):
    m = re.search(r'^[^\n]*\b' + re.escape(name) + r'\s*\(', text, re.M)
    if not m:
        return None
    start = text.rfind('\n', 0, m.start()) + 1
    i = text.find('{', m.start())
    if i < 0:
        return None
    depth = 0
    j = i
    while j < len(text):
        c = text[j]
        if c == '{':
            depth += 1
        elif c == '}':
            depth -= 1
            if depth == 0:
                return (start, j + 1)
        elif c in '"\'':
            q = c
            j += 1
            while j < len(text) and text[j] != q:
                if text[j] == '\\':
                    j += 1
                j += 1
        j += 1
    return None


def extract_func(text, name):
    b = find_func(text, name)
    if not b:
        return None
    return text[b[0]:b[1]]


def func_in_new(new, name):
    b = find_func(new, name)
    if not b:
        return None
    return new[b[0]:b[1]]


def sim(a, b):
    return difflib.SequenceMatcher(None, a, b, autojunk=False).ratio()


def fuzzy_block(content, old, new):
    fl = content.split('\n')
    ol = old.split('\n')
    first = ol[0].strip()
    idxs = [i for i, l in enumerate(fl) if l.strip() == first]
    best = None
    for i in idxs:
        for span in (len(ol), len(ol) + 1, len(ol) - 1, len(ol) + 3, len(ol) - 3, len(ol) + 6):
            if span <= 0 or i + span > len(fl):
                continue
            seg = '\n'.join(fl[i:i + span])
            r = sim(seg, old)
            if best is None or r > best[0]:
                best = (r, i, span)
    if best is None or best[0] < 0.6:
        return False
    _, i, span = best
    fl[i:i + span] = new.split('\n')
    return True


def apply_edit(content, old, new):
    if old in content:
        return content.replace(old, new, 1)
    if new in content:
        return content
    names = re.findall(r'\b(\w+)::(\w+)\s*\(', old)
    for cls, fn in names:
        cur = extract_func(content, fn)
        rep = func_in_new(new, fn)
        if cur and rep:
            if sim(cur, rep) > 0.5:
                return content.replace(cur, rep, 1)
            return content.replace(cur, rep, 1)
    if fuzzy_block(content, old, new):
        return '\n'.join(content.split('\n'))
    return None


def build(name, relpath, base=None):
    events = json.load(open(os.path.join(ROOT, 'tools', '_events.json'), encoding='utf-8'))
    evs = [e for e in events if e[1].replace('\\', '/').endswith('/' + name)]

    if base is None:
        r = subprocess.run(['git', 'show', 'HEAD:' + relpath], cwd=REPO, capture_output=True)
        content = r.stdout.decode('utf-8', errors='replace').replace('\r\n', '\n')
    else:
        content = open(base, encoding='utf-8', newline='').read().replace('\r\n', '\n')

    applied = []
    missed = []
    for i, ev in enumerate(evs):
        if ev[0] != 'E':
            continue
        old = ev[2].replace('\r\n', '\n')
        new = ev[3].replace('\r\n', '\n')
        if not old.strip() or not new.strip():
            continue
        res = apply_edit(content, old, new)
        if res is None:
            missed.append(i)
        else:
            content = res
            applied.append(i)
    return content, applied, missed


if __name__ == '__main__':
    import sys
    target = sys.argv[1] if len(sys.argv) > 1 else 'IModule.cpp'
    rel = 'Project/QT/SmartGround/src/modules/' + target
    content, applied, missed = build(target, rel)
    print('applied', len(applied), 'missed', missed)
    with open(os.path.join(ROOT, 'src', 'modules', target), 'w', encoding='utf-8', newline='') as f:
        f.write(content)
