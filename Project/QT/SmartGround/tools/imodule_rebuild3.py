import json
import os
import re
import subprocess

REPO = r'D:\Program\Cpp'
ROOT = r'd:\Program\Cpp\Project\QT\SmartGround'
NAME = 'IModule.cpp'

events = json.load(open(os.path.join(ROOT, 'tools', '_events.json'), encoding='utf-8'))
evs = [e for e in events if e[1].replace('\\', '/').endswith('/' + NAME)]

r = subprocess.run(['git', 'show', 'HEAD:Project/QT/SmartGround/src/modules/' + NAME],
                   cwd=REPO, capture_output=True)
content = r.stdout.decode('utf-8', errors='replace').replace('\r\n', '\n')


def norm(s):
    return re.sub(r'\s+', ' ', s).strip()


def loose_find(text, old):
    no = norm(old)
    if len(no) < 40:
        return None
    toks = no.split(' ')
    pattern = re.escape(toks[0]) + r'\s+' + r'\s+'.join(re.escape(t) for t in toks[1:])
    m = re.search(pattern, text)
    return m


def func_bounds(text, name):
    m = re.search(r'^[^\n]*\bIModule::' + re.escape(name) + r'\s*\(', text, re.M)
    if not m:
        return None
    i = text.find('{', m.start())
    if i < 0:
        return None
    depth = 0
    while i < len(text):
        c = text[i]
        if c == '{':
            depth += 1
        elif c == '}':
            depth -= 1
            if depth == 0:
                return (m.start(), i + 1)
        elif c in '"\'':
            q = c
            i += 1
            while i < len(text) and text[i] != q:
                if text[i] == '\\':
                    i += 1
                i += 1
        i += 1
    return None


applied = []
missed = []
for i, ev in enumerate(evs):
    if ev[0] != 'E':
        continue
    old, new = ev[2].replace('\r\n', '\n'), ev[3].replace('\r\n', '\n')
    if not old or not new:
        continue
    if old in content:
        content = content.replace(old, new, 1)
        applied.append(i)
        continue
    if new in content:
        applied.append(i)
        continue
    m = loose_find(content, old)
    if m:
        content = content[:m.start()] + new + content[m.end():]
        applied.append(i)
        continue
    fname = None
    fm = re.search(r'IModule::(\w+)\s*\(', old)
    if fm:
        fname = fm.group(1)
    fb = func_bounds(content, fname) if fname else None
    if fb:
        seg = content[fb[0]:fb[1]]
        nm = re.search(r'IModule::(\w+)\s*\(', new)
        if nm:
            nf = func_bounds(new, nm.group(1))
            if nf:
                content = content[:fb[0]] + new[nf[0]:nf[1]] + content[fb[1]:]
                applied.append(i)
                continue
    missed.append(i)

print('applied', len(applied))
print('missed', missed)
with open(os.path.join(ROOT, 'src', 'modules', NAME), 'w', encoding='utf-8', newline='') as f:
    f.write(content)
print('len', len(content))
