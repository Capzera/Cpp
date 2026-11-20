import difflib
import json
import os
import re
import subprocess

REPO = r'D:\Program\Cpp'
ROOT = r'd:\Program\Cpp\Project\QT\SmartGround'


def norm(s):
    return [l.strip() for l in s.replace('\r\n', '\n').split('\n')]


def find_region(cl, ol):
    first = ol[0]
    cands = [i for i, l in enumerate(cl) if l == first]
    if not cands and len(first) > 12:
        cands = [i for i, l in enumerate(cl) if l.startswith(first[:40])]
    best = None
    for i in cands:
        for span in (len(ol), len(ol) + 1, len(ol) - 1, len(ol) + 3, len(ol) - 3,
                     len(ol) + 6, len(ol) - 6, len(ol) + 12, len(ol) - 12):
            if span <= 0 or i + span > len(cl):
                continue
            seg = cl[i:i + span]
            r = difflib.SequenceMatcher(None, seg, ol, autojunk=False).ratio()
            if best is None or r > best[0]:
                best = (r, i, span)
    return best


def apply_fuzzy(content, old, new):
    cl = content.split('\n')
    ol = norm(old)
    nl = norm(new)
    loc = find_region(cl, ol)
    if not loc:
        return None, 'nofind'
    r_old, i, span = loc
    seg = cl[i:i + span]
    r_new = difflib.SequenceMatcher(None, seg, nl, autojunk=False).ratio()
    if r_new >= 0.92 and r_new >= r_old:
        return content, 'already'
    if r_old < 0.5:
        return None, 'lowsim %.2f' % r_old
    # verify the new block's "signature" lines (non-trivial) exist in seg
    newlines = [l for l in nl if len(l) > 12]
    missing = [l for l in newlines if l not in seg]
    if missing and len(missing) > max(2, len(newlines) // 3):
        return None, 'verify'
    cl[i:i + span] = new.replace('\r\n', '\n').split('\n')
    return '\n'.join(cl), 'fuzzy'


def main():
    import sys
    target = sys.argv[1] if len(sys.argv) > 1 else 'IModule.cpp'
    rel = 'Project/QT/SmartGround/src/modules/' + target
    path = os.path.join(ROOT, 'src', 'modules', target)
    content = open(path, encoding='utf-8', newline='').read().replace('\r\n', '\n')
    events = json.load(open(os.path.join(ROOT, 'tools', '_events.json'), encoding='utf-8'))
    evs = [e for e in events if e[1].replace('\\', '/').endswith('/' + name_of(target))]
    stats = {}
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
            stats['exact'] = stats.get('exact', 0) + 1
            continue
        if new in content:
            stats['already'] = stats.get('already', 0) + 1
            continue
        res, kind = apply_fuzzy(content, old, new)
        if res is None:
            missed.append((i, kind))
            continue
        content = res
        stats['fuzzy'] = stats.get('fuzzy', 0) + 1
    open(path, 'w', encoding='utf-8', newline='').write(content)
    print(stats)
    print('missed', missed)


def name_of(t):
    return t


if __name__ == '__main__':
    main()
