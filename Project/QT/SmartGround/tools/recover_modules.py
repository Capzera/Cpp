import json
import os
import re
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


def norm(s):
    return s.replace('\r\n', '\n')


report = []
for name in sorted(os.listdir(MODS)):
    if not name.endswith('.cpp'):
        continue
    path = os.path.join(MODS, name)
    with open(path, encoding='utf-8', newline='') as f:
        cur = f.read()
    if cur.startswith('#include') or cur.lstrip().startswith('#include'):
        continue

    cur_n = norm(cur)
    rel = 'Project/QT/SmartGround/src/modules/' + name
    base = git_show(rel)

    key = None
    for k in per_file:
        if k.endswith('/' + name):
            key = k
            break

    source = None
    origin = None
    if key:
        evs = per_file[key]
        content = None
        for ev in evs:
            if ev[0] == 'W':
                content = ev[2]
            else:
                if content is None:
                    continue
                old, new = ev[2], ev[3]
                if old and old in content:
                    content = content.replace(old, new, 1)
        if content:
            source = norm(content)
            origin = 'transcript'

    if source is None and base is not None:
        idx = base.find(cur_n[:400])
        if idx >= 0:
            source = base[:idx] + cur_n
            origin = 'git+cur'
    if source is None and base is not None:
        source = base
        origin = 'git-only'

    if source is None:
        report.append((name, 'UNRECOVERABLE', len(cur_n)))
        continue

    report.append((name, origin, len(source)))

    with open(path, 'w', encoding='utf-8', newline='') as f:
        f.write(source)

for r in report:
    print(r)
