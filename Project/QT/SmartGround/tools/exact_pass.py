import json
import os
import re
import subprocess

REPO = r'D:\Program\Cpp'
ROOT = r'd:\Program\Cpp\Project\QT\SmartGround'


def exact_pass(name, relpath):
    events = json.load(open(os.path.join(ROOT, 'tools', '_events.json'), encoding='utf-8'))
    evs = [e for e in events if e[1].replace('\\', '/').endswith('/' + name)]
    r = subprocess.run(['git', 'show', 'HEAD:' + relpath], cwd=REPO, capture_output=True)
    content = r.stdout.decode('utf-8', errors='replace').replace('\r\n', '\n')
    exact = []
    nonexact = []
    for i, ev in enumerate(evs):
        if ev[0] != 'E':
            continue
        old = ev[2].replace('\r\n', '\n')
        new = ev[3].replace('\r\n', '\n')
        if not old.strip() or not new.strip():
            continue
        if old in content:
            content = content.replace(old, new, 1)
            exact.append(i)
        elif new in content:
            exact.append(i)
        else:
            nonexact.append(i)
    return content, exact, nonexact


if __name__ == '__main__':
    import sys
    target = sys.argv[1] if len(sys.argv) > 1 else 'IModule.cpp'
    rel = 'Project/QT/SmartGround/src/modules/' + target
    content, exact, nonexact = exact_pass(target, rel)
    print('exact', len(exact), 'nonexact', nonexact)
    with open(os.path.join(ROOT, 'src', 'modules', target), 'w', encoding='utf-8', newline='') as f:
        f.write(content)
