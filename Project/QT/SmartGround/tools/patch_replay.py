import difflib
import json
import os
import re
import subprocess

REPO = r'D:\Program\Cpp'
ROOT = r'd:\Program\Cpp\Project\QT\SmartGround'


def build(name, relpath):
    events = json.load(open(os.path.join(ROOT, 'tools', '_events.json'), encoding='utf-8'))
    evs = [e for e in events if e[1].replace('\\', '/').endswith('/' + name)]

    r = subprocess.run(['git', 'show', 'HEAD:' + relpath], cwd=REPO, capture_output=True)
    content = r.stdout.decode('utf-8', errors='replace').replace('\r\n', '\n')

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
        if patch_by_lines(content, old, new):
            applied.append(i)
            continue
        missed.append(i)
    return content, applied, missed


def patch_by_lines(content, old, new):
    fl = content.split('\n')
    ol = [l.strip() for l in old.split('\n')]
    nl = [l.strip() for l in new.split('\n')]
    fs = [l.strip() for l in fl]
    sm = difflib.SequenceMatcher(None, fs, ol, autojunk=False)
    blocks = [b for b in sm.get_matching_blocks() if b.size > 0]
    if not blocks:
        return False
    best = max(blocks, key=lambda b: b.size)
    total = sum(b.size for b in blocks)
    if total < max(2, len(ol) // 3):
        return False
    fstart = best.a
    ostart = best.b
    size = best.size
    fend = fstart + size
    oend = ostart + size
    if oend < len(ol):
        tail_old = ol[oend:]
        tail_new = nl[len(nl) - len(tail_old):] if len(nl) >= len(tail_old) else []
        if tail_old and len(tail_new) == len(tail_old):
            fend += len(tail_old)
            nl = nl[:-len(tail_old)]
    if ostart > 0:
        head_old = ol[:ostart]
        head_new = nl[:len(head_old)] if len(nl) >= len(head_old) else []
        if len(head_old) == len(head_new):
            fstart -= ostart
            nl = head_new + nl[ostart:]
        else:
            return False
    if len(nl) < len(ol):
        return False
    fl[fstart:fend] = nl
    return True


if __name__ == '__main__':
    import sys
    target = sys.argv[1] if len(sys.argv) > 1 else 'IModule.cpp'
    rel = 'Project/QT/SmartGround/src/modules/' + target
    content, applied, missed = build(target, rel)
    print('applied', len(applied), 'missed', missed)
    with open(os.path.join(ROOT, 'src', 'modules', target), 'w', encoding='utf-8', newline='') as f:
        f.write(content)
