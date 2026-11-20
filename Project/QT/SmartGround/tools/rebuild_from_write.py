import json
import os
import re
import sys

ROOT = r'd:\Program\Cpp\Project\QT\SmartGround'
TARGET = sys.argv[1] if len(sys.argv) > 1 else 'IModule.h'
WRITE_INDEX = int(sys.argv[2]) if len(sys.argv) > 2 else 5
PATH = os.path.join(ROOT, 'src', 'modules', TARGET)


def main():
    events = json.load(open(os.path.join(ROOT, 'tools', '_events.json'), encoding='utf-8'))
    evs = [e for e in events if e[1].replace('\\', '/').endswith('/' + TARGET)]
    content = evs[WRITE_INDEX][2].replace('\r\n', '\n')
    rest = evs[WRITE_INDEX + 1:]
    ok = 0
    miss = []
    for i, ev in enumerate(rest):
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
    open(PATH, 'w', encoding='utf-8', newline='').write(content)
    print(TARGET, 'ok', ok, 'miss', miss)


if __name__ == '__main__':
    main()
