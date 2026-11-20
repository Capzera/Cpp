import difflib
import json
import os
import re
import subprocess

REPO = r'D:\Program\Cpp'
ROOT = r'd:\Program\Cpp\Project\QT\SmartGround'


def norm_lines(s):
    return [l.rstrip() for l in s.replace('\r\n', '\n').split('\n')]


def function_spans(text):
    spans = []
    for m in re.finditer(r'^[A-Za-z_][^\n;{}]*\bIModule::(\w+)\s*\([^;]*\)\s*(?:const\s*)?\{', text, re.M):
        i = text.find('{', m.start())
        depth = 0
        j = i
        while j < len(text):
            c = text[j]
            if c == '{':
                depth += 1
            elif c == '}':
                depth -= 1
                if depth == 0:
                    spans.append((m.group(1), m.start(), j + 1))
                    break
            elif c in '"\'':
                q = c
                j += 1
                while j < len(text) and text[j] != q:
                    if text[j] == '\\':
                        j += 1
                    j += 1
            j += 1
    return spans


def dedupe(text):
    """Remove duplicate IModule method definitions, keeping the LAST one."""
    changed = True
    while changed:
        changed = False
        spans = function_spans(text)
        seen = {}
        for name, s, e in spans:
            seen.setdefault(name, []).append((s, e))
        for name, lst in seen.items():
            if len(lst) < 2:
                continue
            keep = lst[-1]
            for s, e in reversed(lst[:-1]):
                text = text[:s] + text[e:]
                changed = True
            break
    return text


def apply_exact(content, old, new):
    if old in content:
        return content.replace(old, new, 1), 'exact'
    if new in content:
        return content, 'already'
    return None, 'miss'


def build(name, relpath):
    events = json.load(open(os.path.join(ROOT, 'tools', '_events.json'), encoding='utf-8'))
    evs = [e for e in events if e[1].replace('\\', '/').endswith('/' + name)]
    r = subprocess.run(['git', 'show', 'HEAD:' + relpath], cwd=REPO, capture_output=True)
    content = r.stdout.decode('utf-8', errors='replace').replace('\r\n', '\n')
    applied = 0
    missed = []
    for i, ev in enumerate(evs):
        if ev[0] != 'E':
            continue
        old = ev[2].replace('\r\n', '\n')
        new = ev[3].replace('\r\n', '\n')
        if not old.strip() or not new.strip():
            continue
        res, kind = apply_exact(content, old, new)
        if res is None:
            missed.append(i)
            continue
        content = res
        applied += 1
    content = dedupe(content)
    return content, applied, missed


if __name__ == '__main__':
    import sys
    target = sys.argv[1] if len(sys.argv) > 1 else 'IModule.cpp'
    rel = 'Project/QT/SmartGround/src/modules/' + target
    content, applied, missed = build(target, rel)
    print('exact applied', applied, 'missed', missed)
    with open(os.path.join(ROOT, 'src', 'modules', target), 'w', encoding='utf-8', newline='') as f:
        f.write(content)
    spans = function_spans(content)
    names = [n for n, _, _ in spans]
    dup = {n for n in names if names.count(n) > 1}
    print('dups after dedupe:', dup)
    print('functions:', len(spans))
