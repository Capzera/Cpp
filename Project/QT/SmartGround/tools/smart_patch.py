import difflib
import json
import os
import re
import subprocess

REPO = r'D:\Program\Cpp'
ROOT = r'd:\Program\Cpp\Project\QT\SmartGround'


def norm_lines(s):
    return [l.rstrip() for l in s.replace('\r\n', '\n').split('\n')]


def hunks(old_lines, new_lines):
    out = []
    for tag, i1, i2, j1, j2 in difflib.SequenceMatcher(None, old_lines, new_lines, autojunk=False).get_opcodes():
        if tag == 'equal':
            continue
        out.append((i1, i2, j1, j2))
    return out


def apply_edit(content, old, new):
    if old in content:
        return content.replace(old, new, 1), 'exact'
    if new in content:
        return content, 'already-exact'

    ol = norm_lines(old)
    nl = norm_lines(new)
    cl = content.split('\n')
    cl_stripped = [l.strip() for l in cl]
    ol_stripped = [l.strip() for l in ol]

    # locate the old block: try each candidate start from matching first line
    first = ol_stripped[0]
    cands = [i for i, l in enumerate(cl_stripped) if l == first]
    if not cands and len(first) > 12:
        cands = [i for i, l in enumerate(cl_stripped) if l.startswith(first[:40])]
    best = None
    for i in cands:
        for span in (len(ol), len(ol) + 1, len(ol) - 1, len(ol) + 4, len(ol) - 4,
                     len(ol) + 10, len(ol) - 10):
            if span <= 0 or i + span > len(cl_stripped):
                continue
            seg = cl_stripped[i:i + span]
            r = difflib.SequenceMatcher(None, seg, ol_stripped, autojunk=False).ratio()
            if best is None or r > best[0]:
                best = (r, i, span)
    if best is None:
        return None, 'nofind'
    r_old, i, span = best
    seg = cl_stripped[i:i + span]
    r_new = difflib.SequenceMatcher(None, seg, [l.strip() for l in nl], autojunk=False).ratio()
    if r_new >= 0.9 and r_new >= r_old:
        return content, 'already-fuzzy'

    # check each hunk: if added lines already present in region -> already applied
    region = cl_stripped[i:i + span]
    all_present = True
    for (i1, i2, j1, j2) in hunks(ol_stripped, [l.strip() for l in nl]):
        added = [l.strip() for l in nl[j1:j2] if l.strip()]
        if not added:
            continue
        for a in added:
            if a not in region:
                all_present = False
                break
        if not all_present:
            break
    if all_present:
        return content, 'already-hunk'

    if r_old < 0.45:
        return None, 'lowsim'
    cl[i:i + span] = new.replace('\r\n', '\n').split('\n')
    return '\n'.join(cl), 'fuzzy'


def build(name, relpath):
    events = json.load(open(os.path.join(ROOT, 'tools', '_events.json'), encoding='utf-8'))
    evs = [e for e in events if e[1].replace('\\', '/').endswith('/' + name)]
    r = subprocess.run(['git', 'show', 'HEAD:' + relpath], cwd=REPO, capture_output=True)
    content = r.stdout.decode('utf-8', errors='replace').replace('\r\n', '\n')
    stats = {}
    missed = []
    for i, ev in enumerate(evs):
        if ev[0] != 'E':
            continue
        old = ev[2].replace('\r\n', '\n')
        new = ev[3].replace('\r\n', '\n')
        if not old.strip() or not new.strip():
            continue
        res, kind = apply_edit(content, old, new)
        if res is None:
            missed.append((i, kind))
            continue
        content = res
        stats[kind] = stats.get(kind, 0) + 1
    return content, stats, missed


if __name__ == '__main__':
    import sys
    target = sys.argv[1] if len(sys.argv) > 1 else 'IModule.cpp'
    rel = 'Project/QT/SmartGround/src/modules/' + target
    content, stats, missed = build(target, rel)
    print(stats)
    print('missed', missed)
    with open(os.path.join(ROOT, 'src', 'modules', target), 'w', encoding='utf-8', newline='') as f:
        f.write(content)
