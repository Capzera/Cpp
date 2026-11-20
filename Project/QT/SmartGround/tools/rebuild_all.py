import json
import os
import re
import subprocess
import sys

REPO = r'D:\Program\Cpp'
ROOT = r'd:\Program\Cpp\Project\QT\SmartGround'
MODS = os.path.join(ROOT, 'src', 'modules')
REC = os.path.join(ROOT, 'tools', '_recovered')


def head_content(rel):
    r = subprocess.run(['git', 'show', 'HEAD:' + rel], cwd=REPO, capture_output=True)
    if r.returncode != 0 or not r.stdout:
        return None
    return r.stdout.decode('utf-8-sig', errors='replace').replace('\r\n', '\n')


def apply_events(content, evs):
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
    return content, ok, miss


def main():
    events = json.load(open(os.path.join(ROOT, 'tools', '_events.json'), encoding='utf-8'))
    headnames = set(x.strip().split('/')[-1] for x in
                    open(os.path.join(ROOT, 'tools', '_head_mods.txt'), encoding='utf-8-sig') if x.strip())
    names = sorted(n for n in os.listdir(MODS) if n.endswith('.cpp') or n.endswith('.h'))
    total_miss = 0
    for name in names:
        rel = 'Project/QT/SmartGround/src/modules/' + name
        evs = [e for e in events if e[1].replace('\\', '/').endswith('/' + name)]
        base = None
        if name in headnames:
            base = head_content(rel)
        if base is None:
            last_w = -1
            for i, ev in enumerate(evs):
                if ev[0] == 'W':
                    last_w = i
            if last_w < 0:
                print(name, 'SKIP no base')
                continue
            base = evs[last_w][2].replace('\r\n', '\n')
            evs = evs[last_w + 1:]
        content, ok, miss = apply_events(base, evs)
        total_miss += len(miss)
        open(os.path.join(MODS, name), 'w', encoding='utf-8', newline='').write(content)
        if miss:
            print(name, 'ok', ok, 'MISS', miss)
    print('total misses', total_miss)


if __name__ == '__main__':
    main()
