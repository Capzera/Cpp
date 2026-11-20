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


def func_spans(text):
    spans = []
    for m in re.finditer(r'^[^\n;{}]*\bIModule::(\w+)\s*\([^;]*\)\s*(?:const\s*)?\{', text, re.M):
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


def grab(text, name):
    for n, s, e in func_spans(text):
        if n == name:
            return text[s:e]
    return None


def dedupe_keep_last(text):
    while True:
        spans = func_spans(text)
        seen = {}
        for name, s, e in spans:
            seen.setdefault(name, []).append((s, e))
        dups = {k: v for k, v in seen.items() if len(v) > 1}
        if not dups:
            return text
        name, lst = sorted(dups.items())[0]
        keep = lst[-1]
        for s, e in reversed(lst[:-1]):
            text = text[:s] + text[e:]


def ratio(a, b):
    return difflib.SequenceMatcher(None, a, b, autojunk=False).ratio()


def edit_by_func(content, old, new):
    on = re.findall(r'\bIModule::(\w+)\s*\(', old)
    nn = re.findall(r'\bIModule::(\w+)\s*\(', new)
    if len(on) != 1 or len(nn) != 1 or on[0] != nn[0]:
        return None
    name = on[0]
    cur = grab(content, name)
    rep = grab(new, name)
    if not cur or not rep:
        return None
    if ratio(norm(cur), norm(rep)) > 0.95:
        return content, 'already'
    return content.replace(cur, rep, 1), 'func'


def main():
    target = sys.argv[1] if len(sys.argv) > 1 else 'IModule.cpp'
    rel = 'Project/QT/SmartGround/src/modules/' + target
    path = os.path.join(ROOT, 'src', 'modules', target)
    content = open(path, encoding='utf-8', newline='').read().replace('\r\n', '\n')
    events = json.load(open(os.path.join(ROOT, 'tools', '_events.json'), encoding='utf-8'))
    evs = [e for e in events if e[1].replace('\\', '/').endswith('/' + target)]
    stats = {}
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
            stats['exact'] = stats.get('exact', 0) + 1
            continue
        if new in content:
            stats['already'] = stats.get('already', 0) + 1
            continue
        res = edit_by_func(content, old, new)
        if res is not None:
            content, kind = res
            stats[kind] = stats.get(kind, 0) + 1
            continue
        misses.append(i)
    content = dedupe_keep_last(content)
    open(path, 'w', encoding='utf-8', newline='').write(content)
    print(stats)
    print('misses', misses)
    spans = func_spans(content)
    names = [n for n, _, _ in spans]
    dup = sorted({n for n in names if names.count(n) > 1})
    print('remaining dups:', dup)
    print('funcs:', len(spans))


if __name__ == '__main__':
    main()
