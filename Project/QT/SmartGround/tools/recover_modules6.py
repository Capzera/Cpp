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
    r = subprocess.run(['git', 'show', 'HEAD:' + rel], cwd=REPO, capture_output=True)
    if r.returncode != 0:
        return None
    return r.stdout.decode('utf-8', errors='replace').replace('\r\n', '\n')


def loose_replace(content, old, new):
    if not old:
        return None
    if old in content:
        return content.replace(old, new, 1)
    ol = old.split('\n')
    nl = new.split('\n')
    cl = content.split('\n')
    n = len(ol)
    for i in range(len(cl) - n + 1):
        if cl[i].strip() != ol[0].strip():
            continue
        ok = True
        for j in range(1, n):
            if cl[i + j].strip() != ol[j].strip():
                ok = False
                break
        if ok:
            return '\n'.join(cl[:i] + nl + cl[i + n:])
    return None


def apply_edits(content, tail, stats):
    for ev in tail:
        if ev[0] != 'E':
            continue
        old, new = ev[2].replace('\r\n', '\n'), ev[3].replace('\r\n', '\n')
        if not old:
            continue
        if new and new in content:
            stats['skip'] += 1
            continue
        r = loose_replace(content, old, new)
        if r is None:
            stats['miss'] += 1
            continue
        content = r
        stats['ok'] += 1
    return content


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

    stats = collections.Counter()
    if last_w >= 0:
        content = evs[last_w][2].replace('\r\n', '\n')
        origin = 'write'
        tail = evs[last_w + 1:]
    else:
        content = base
        origin = 'git-base'
        tail = evs

    if content is not None:
        content = apply_edits(content, tail, stats)

    if content is None:
        report.append((name, 'NONE', 0, dict(stats)))
        continue

    with open(path, 'w', encoding='utf-8', newline='') as f:
        f.write(content)
    report.append((name, origin, len(content), dict(stats)))

for r in report:
    print(r)
