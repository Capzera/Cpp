import json
import os
import subprocess
import collections

REPO = r'D:\Program\Cpp'
ROOT = r'd:\Program\Cpp\Project\QT\SmartGround'
MODS = os.path.join(ROOT, 'src', 'modules')

events = json.load(open(os.path.join(ROOT, 'tools', '_events.json'), encoding='utf-8'))

per_file = collections.defaultdict(list)
for ev in events:
    fp = ev[1].replace('\\', '/')
    if fp.endswith('.cpp') and '/src/modules/' in fp:
        per_file[fp].append(ev)


def git_show(rel):
    r = subprocess.run(['git', 'show', 'HEAD:' + rel], cwd=REPO,
                       capture_output=True, text=True, encoding='utf-8', errors='replace')
    if r.returncode != 0:
        return None
    return r.stdout.replace('\r\n', '\n')


report = []
for name in sorted(os.listdir(MODS)):
    if not name.endswith('.cpp'):
        continue
    path = os.path.join(MODS, name)
    rel = 'Project/QT/SmartGround/src/modules/' + name

    key = None
    for k in per_file:
        if k.endswith('/' + name):
            key = k
            break

    evs = per_file.get(key, [])
    base = git_show(rel)

    content = None
    started = False
    for ev in evs:
        if ev[0] == 'W':
            content = ev[2].replace('\r\n', '\n')
            started = True
        elif ev[0] == 'E':
            if content is None:
                if base is None:
                    continue
                content = base
                started = True
            old, new = ev[2].replace('\r\n', '\n'), ev[3].replace('\r\n', '\n')
            if not old:
                continue
            if old in content:
                content = content.replace(old, new, 1)

    if content is None:
        content = base

    if content is None:
        report.append((name, 'NONE'))
        continue

    with open(path, 'w', encoding='utf-8', newline='') as f:
        f.write(content)
    report.append((name, 'gitbase' if base else 'writeonly', len(content)))

for r in report:
    print(r)
