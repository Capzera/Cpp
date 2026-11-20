import difflib
import json
import os
import re
import subprocess
import sys

REPO = r'D:\Program\Cpp'
ROOT = r'd:\Program\Cpp\Project\QT\SmartGround'


def norm(s):
    return [l.rstrip() for l in s.replace('\r\n', '\n').split('\n')]


def locate(cl, target):
    t = [l.strip() for l in target]
    c = [l.strip() for l in cl]
    if not t:
        return None
    sm = difflib.SequenceMatcher(None, c, t, autojunk=False)
    blocks = [b for b in sm.get_matching_blocks() if b.size > 0]
    if blocks:
        best = max(blocks, key=lambda b: b.size)
        total = sum(b.size for b in blocks)
        if total >= max(1, len(t) * 0.6):
            start = best.a
            end = best.a + best.size
            pre = best.b
            post = best.b + best.size
            while start > 0 and pre > 0 and c[start - 1] == t[pre - 1]:
                start -= 1
                pre -= 1
            while end < len(c) and post < len(t) and c[end] == t[post]:
                end += 1
                post += 1
            return (start, end)
    n = len(t)
    for i in range(0, max(0, len(c) - n + 1)):
        if c[i:i + n] == t:
            return (i, i + n)
    return None


def apply_hunks(cl, old, new):
    ol = norm(old)
    nl = norm(new)
    sm = difflib.SequenceMatcher(None, ol, nl, autojunk=False)
    changes = []
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == 'equal':
            continue
        changes.append((ol[i1:i2], nl[j1:j2]))
    result = list(cl)
    applied = 0
    skipped = 0
    for oldseg, newseg in changes:
        loc = locate(result, oldseg)
        if loc is not None:
            s, e = loc
            result[s:e] = newseg
            applied += 1
            continue
        skipped += 1
    return result, applied, skipped, len(changes)


def main():
    target = sys.argv[1] if len(sys.argv) > 1 else 'IModule.cpp'
    rel = 'Project/QT/SmartGround/src/modules/' + target
    path = os.path.join(ROOT, 'src', 'modules', target)
    content = open(path, encoding='utf-8', newline='').read().replace('\r\n', '\n')
    events = json.load(open(os.path.join(ROOT, 'tools', '_events.json'), encoding='utf-8'))
    evs = [e for e in events if e[1].replace('\\', '/').endswith('/' + target)]
    total_applied = total_skipped = total_changes = 0
    misses = []
    for i, ev in enumerate(evs):
        if ev[0] != 'E':
            continue
        old = ev[2].replace('\r\n', '\n')
        new = ev[3].replace('\r\n', '\n')
        if not old.strip() or not new.strip():
            continue
        if old in content:
            content = content.replace(old, new, 1)
            total_applied += 1
            continue
        if new in content:
            total_skipped += 1
            continue
        cl = content.split('\n')
        result, a, s, nch = apply_hunks(cl, old, new)
        total_applied += a
        total_skipped += s
        total_changes += nch
        if a == 0 and nch > 0:
            misses.append(i)
        content = '\n'.join(result)
    open(path, 'w', encoding='utf-8', newline='').write(content)
    print('applied', total_applied, 'skipped', total_skipped, 'hunks', total_changes)
    print('edits with no applied hunk:', misses)


if __name__ == '__main__':
    main()
