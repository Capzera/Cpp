import difflib
import json
import os
import re
import subprocess
import sys
import collections

REPO = r'D:\Program\Cpp'
ROOT = r'd:\Program\Cpp\Project\QT\SmartGround'
MODS = os.path.join(ROOT, 'src', 'modules')


def find_defs(text, cls=None):
    res = []
    pat = r'\b' + (cls + '::' if cls else '') + r'(\w+)\s*\('
    for m in re.finditer(pat, text):
        name = m.group(1)
        line_start = text.rfind('\n', 0, m.start()) + 1
        i = text.find('{', m.end())
        if i < 0:
            continue
        seg = text[m.end():i]
        if seg.count('(') - seg.count(')') != -1:
            continue
        if ';' in seg:
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
                    res.append((name, line_start, j + 1))
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


def ratio(a, b):
    return difflib.SequenceMatcher(None, a, b, autojunk=False).ratio()


def rebuild(name, events):
    path = os.path.join(MODS, name)
    if not os.path.exists(path):
        return None
    rel = 'Project/QT/SmartGround/src/modules/' + name
    r = subprocess.run(['git', 'show', 'HEAD:' + rel], cwd=REPO, capture_output=True)
    if r.returncode != 0 or not r.stdout:
        content = None
    else:
        content = r.stdout.decode('utf-8-sig', errors='replace').replace('\r\n', '\n')

    evs = [e for e in events if e[1].replace('\\', '/').endswith('/' + name)]
    if content is None:
        last_w = -1
        for i, ev in enumerate(evs):
            if ev[0] == 'W':
                last_w = i
        if last_w < 0:
            return None
        content = evs[last_w][2].replace('\r\n', '\n')
        evs = evs[last_w + 1:]
    for ev in evs:
        if ev[0] != 'E':
            continue
        old = ev[2].replace('\r\n', '\n')
        new = ev[3].replace('\r\n', '\n')
        if not old.strip() or not new.strip():
            continue
        if old in content:
            content = content.replace(old, new, 1)
            continue
        if new in content:
            continue
        oldmap = collections.defaultdict(list)
        for n, s, e in find_defs(old):
            oldmap[n].append(old[s:e])
        newmap = collections.defaultdict(list)
        for n, s, e in find_defs(new):
            newmap[n].append(new[s:e])
        if not newmap:
            continue
        curmap = collections.defaultdict(list)
        for n, s, e in find_defs(content):
            curmap[n].append((s, e))
        repls = []
        for n, newsrcs in newmap.items():
            if n not in oldmap or n not in curmap:
                continue
            oldsrc = oldmap[n][-1]
            best = None
            for s, e in curmap[n]:
                rr = ratio(content[s:e], oldsrc)
                if best is None or rr > best[0]:
                    best = (rr, s, e)
            if best is None or best[0] < 0.55:
                continue
            newsrc = newsrcs[-1]
            if content[best[1]:best[2]] == newsrc:
                continue
            repls.append((best[1], best[2], newsrc))
        repls.sort(reverse=True)
        for s, e, src in repls:
            content = content[:s] + src + content[e:]
    return content


def main():
    events = json.load(open(os.path.join(ROOT, 'tools', '_events.json'), encoding='utf-8'))
    targets = sys.argv[1:] if len(sys.argv) > 1 else sorted(
        n for n in os.listdir(MODS) if n.endswith('.cpp'))
    for name in targets:
        res = rebuild(name, events)
        if res is None:
            print(name, 'NO-BASE')
            continue
        open(os.path.join(MODS, name), 'w', encoding='utf-8', newline='').write(res)
        print(name, 'ok', len(res.splitlines()))


if __name__ == '__main__':
    main()
