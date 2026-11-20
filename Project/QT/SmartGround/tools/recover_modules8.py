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
    r = subprocess.run(['git', 'show', 'HEAD:' + rel], cwd=REPO, capture_output=True)
    if r.returncode != 0:
        return None
    return r.stdout.decode('utf-8', errors='replace').replace('\r\n', '\n')


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

    last_w = -1
    for i, ev in enumerate(evs):
        if ev[0] == 'W':
            last_w = i

    if last_w >= 0:
        content = evs[last_w][2].replace('\r\n', '\n')
        tail = evs[last_w + 1:]
        origin = 'write'
    else:
        content = base
        tail = evs
        origin = 'git-base'

    stats = collections.Counter()
    if content is not None:
        for ev in tail:
            if ev[0] != 'E':
                continue
            old, new = ev[2].replace('\r\n', '\n'), ev[3].replace('\r\n', '\n')
            if not old or not new:
                continue
            if new and new in content:
                stats['already'] += 1
                continue
            if old in content:
                content = content.replace(old, new, 1)
                stats['ok'] += 1
            else:
                stats['miss'] += 1

    with open(path, 'w', encoding='utf-8', newline='') as f:
        f.write(content or '')
    report.append((name, origin, len(content or ''), dict(stats)))

for r in report:
    print(r)
