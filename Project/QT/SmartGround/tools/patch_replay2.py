import difflib
import json
import os
import re
import subprocess

REPO = r'D:\Program\Cpp'
ROOT = r'd:\Program\Cpp\Project\QT\SmartGround'


def sim_ratio(a_lines, b_lines):
    sm = difflib.SequenceMatcher(None, a_lines, b_lines, autojunk=False)
    total = sum(b.size for b in sm.get_matching_blocks())
    return total / max(1, len(b_lines))


def fuzzy_patch(content, old, new):
    fl = content.split('\n')
    ol = old.split('\n')
    first = ol[0].strip()
    idxs = [i for i, l in enumerate(fl) if l.strip() == first]
    if not idxs:
        return False
    best = None
    for i in idxs:
        for span in (len(ol), len(ol) + 2, len(ol) - 2, len(ol) + 5, len(ol) - 5):
            if span <= 0:
                continue
            seg = fl[i:i + span]
            r = sim_ratio(seg, ol)
            if best is None or r > best[0]:
                best = (r, i, span)
    if best is None or best[0] < 0.55:
        return False
    _, i, span = best
    fl[i:i + span] = new.split('\n')
    return True


def build(name, relpath, base=None):
    events = json.load(open(os.path.join(ROOT, 'tools', '_events.json'), encoding='utf-8'))
    evs = [e for e in events if e[1].replace('\\', '/').endswith('/' + name)]

    if base is None:
        r = subprocess.run(['git', 'show', 'HEAD:' + relpath], cwd=REPO, capture_output=True)
        content = r.stdout.decode('utf-8', errors='replace').replace('\r\n', '\n')
    else:
        content = open(base, encoding='utf-8', newline='').read().replace('\r\n', '\n')

    applied = []
    missed = []
    for i, ev in enumerate(evs):
        if ev[0] != 'E':
            continue
        old = ev[2].replace('\r\n', '\n')
        new = ev[3].replace('\r\n', '\n')
        if not old.strip() or not new.strip():
            continue
        if old in content:
            content = content.replace(old, new, 1)
            applied.append(i)
            continue
        if new in content:
            applied.append(i)
            continue
        if fuzzy_patch(content, old, new):
            applied.append(i)
            continue
        missed.append(i)
    return content, applied, missed


if __name__ == '__main__':
    import sys
    target = sys.argv[1] if len(sys.argv) > 1 else 'IModule.cpp'
    rel = 'Project/QT/SmartGround/src/modules/' + target
    content, applied, missed = build(target, rel)
    print('applied', len(applied), 'missed', missed)
    with open(os.path.join(ROOT, 'src', 'modules', target), 'w', encoding='utf-8', newline='') as f:
        f.write(content)
