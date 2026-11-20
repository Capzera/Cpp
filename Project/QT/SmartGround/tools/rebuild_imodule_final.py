import difflib
import json
import os
import re
import subprocess
import sys

REPO = r'D:\Program\Cpp'
ROOT = r'd:\Program\Cpp\Project\QT\SmartGround'
PATH = os.path.join(ROOT, 'src', 'modules', 'IModule.cpp')

SIG = re.compile(r'^\s*(?:[\w:<>,\*&\[\]\s]+\s+)?[\w:~]+\s+[\w:~]+::\w+\s*\(')


def find_defs(text):
    res = []
    for m in re.finditer(r'^[^\n;{}]*\bIModule::(\w+)\s*\(', text, re.M):
        name = m.group(1)
        ls = text.rfind('\n', 0, m.start()) + 1
        i = text.find('{', m.end())
        if i < 0:
            continue
        seg = text[m.end():i]
        if seg.count('(') - seg.count(')') != -1 or ';' in seg:
            continue
        depth = 0
        j = i
        ok = False
        while j < len(text):
            c = text[j]
            if c == '{':
                depth += 1
            elif c == '}':
                depth -= 1
                if depth == 0:
                    res.append((name, ls, j + 1))
                    ok = True
                    break
            elif c in '"\'':
                q = c
                j += 1
                while j < len(text) and text[j] != q:
                    if text[j] == '\\':
                        j += 1
                    j += 1
            j += 1
        if not ok:
            continue
    return res


def grab(text, name):
    for n, s, e in find_defs(text):
        if n == name:
            return text[s:e]
    return None


def main():
    r = subprocess.run(['git', 'show', 'HEAD:Project/QT/SmartGround/src/modules/IModule.cpp'],
                       cwd=REPO, capture_output=True)
    content = r.stdout.decode('utf-8-sig', errors='replace').replace('\r\n', '\n')

    events = json.load(open(os.path.join(ROOT, 'tools', '_events.json'), encoding='utf-8'))
    evs = [e for e in events if e[1].replace('\\', '/').endswith('/IModule.cpp')]

    stats = {'exact': 0, 'already': 0, 'func': 0, 'skip': 0}
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
            stats['exact'] += 1
            continue
        if new in content:
            stats['already'] += 1
            continue
        if SIG.match(old.lstrip()):
            m = re.search(r'\bIModule::(\w+)\s*\(', old)
            if m:
                fn = m.group(1)
                cur = grab(content, fn)
                rep = grab(new, fn)
                if cur and rep:
                    if cur != rep:
                        content = content.replace(cur, rep, 1)
                    stats['func'] += 1
                    continue
        stats['skip'] += 1
        miss.append(i)
    open(PATH, 'w', encoding='utf-8', newline='').write(content)
    print(stats)
    print('miss', miss)


if __name__ == '__main__':
    main()
