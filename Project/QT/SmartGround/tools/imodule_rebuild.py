import json
import os
import subprocess
import collections

REPO = r'D:\Program\Cpp'
ROOT = r'd:\Program\Cpp\Project\QT\SmartGround'

events = json.load(open(os.path.join(ROOT, 'tools', '_events.json'), encoding='utf-8'))

name = 'IModule.cpp'
evs = [e for e in events if e[1].replace('\\', '/').endswith('/' + name)]

r = subprocess.run(['git', 'show', 'HEAD:Project/QT/SmartGround/src/modules/' + name],
                   cwd=REPO, capture_output=True)
content = r.stdout.decode('utf-8', errors='replace').replace('\r\n', '\n')

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
    elif new in content:
        applied.append(i)
    else:
        missed.append(i)

print('applied', len(applied))
print('missed', missed)
with open(os.path.join(ROOT, 'src', 'modules', name), 'w', encoding='utf-8', newline='') as f:
    f.write(content)
print('len', len(content))
