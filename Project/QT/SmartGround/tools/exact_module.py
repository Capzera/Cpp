import json
import os
import re
import subprocess
import sys

REPO = r'D:\Program\Cpp'
ROOT = r'd:\Program\Cpp\Project\QT\SmartGround'
MODS = os.path.join(ROOT, 'src', 'modules')


def main():
    name = sys.argv[1]
    path = os.path.join(MODS, name)
    rel = 'Project/QT/SmartGround/src/modules/' + name
    r = subprocess.run(['git', 'show', 'HEAD:' + rel], cwd=REPO, capture_output=True)
    if r.returncode == 0 and r.stdout:
        content = r.stdout.decode('utf-8-sig', errors='replace').replace('\r\n', '\n')
    else:
        content = None
    events = json.load(open(os.path.join(ROOT, 'tools', '_events.json'), encoding='utf-8'))
    evs = [e for e in events if e[1].replace('\\', '/').endswith('/' + name)]
    if content is None:
        last_w = -1
        for i, ev in enumerate(evs):
            if ev[0] == 'W':
                last_w = i
        content = evs[last_w][2].replace('\r\n', '\n')
        evs = evs[last_w + 1:]
    ok = 0
    miss = []
    for i, ev in enumerate(evs):
        if ev[0] != 'E':
            continue
        old = ev[2].replace('\r\n', '\n')
        new = ev[3].replace('\r\n', '\n')
        if not old.strip() or not new.strip():
            continue
        if old in content:
            content = content.replace(old, new, 1)
            ok += 1
        elif new in content:
            ok += 1
        else:
            miss.append(i)
    open(path, 'w', encoding='utf-8', newline='').write(content)
    print(name, 'ok', ok, 'miss', miss)


if __name__ == '__main__':
    main()
