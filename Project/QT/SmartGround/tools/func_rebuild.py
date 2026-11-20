import difflib
import json
import os
import re
import subprocess
import sys
import collections

REPO = r'D:\Program\Cpp'
ROOT = r'd:\Program\Cpp\Project\QT\SmartGround'


def find_defs(text):
    res = []
    for m in re.finditer(r'\bIModule::(\w+)\s*\(', text):
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


def defs_map(text):
    out = collections.OrderedDict()
    for name, s, e in find_defs(text):
        out.setdefault(name, []).append(text[s:e])
    return out


def ratio(a, b):
    return difflib.SequenceMatcher(None, a, b, autojunk=False).ratio()


def main():
    target = sys.argv[1] if len(sys.argv) > 1 else 'IModule.cpp'
    rel = 'Project/QT/SmartGround/src/modules/' + target
    path = os.path.join(ROOT, 'src', 'modules', target)

    r = subprocess.run(['git', 'show', 'HEAD:' + rel], cwd=REPO, capture_output=True)
    content = r.stdout.decode('utf-8-sig', errors='replace').replace('\r\n', '\n')

    events = json.load(open(os.path.join(ROOT, 'tools', '_events.json'), encoding='utf-8'))
    evs = [e for e in events if e[1].replace('\\', '/').endswith('/' + target)]

    stats = collections.Counter()
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

        oldmap = defs_map(old)
        newmap = defs_map(new)
        if not newmap:
            stats['skip-nodefs'] += 1
            continue
        cur = find_defs(content)
        curmap = collections.defaultdict(list)
        for name, s, e in cur:
            curmap[name].append((s, e))

        repls = []
        for name, newsrcs in newmap.items():
            if name not in oldmap:
                continue
            oldsrc = oldmap[name][-1]
            if name not in curmap:
                continue
            cands = curmap[name]
            best = None
            for s, e in cands:
                r_ = ratio(content[s:e], oldsrc)
                if best is None or r_ > best[0]:
                    best = (r_, s, e)
            if best is None or best[0] < 0.55:
                continue
            newsrc = newsrcs[-1]
            if content[best[1]:best[2]] == newsrc:
                continue
            repls.append((best[1], best[2], newsrc))
        if not repls:
            stats['skip-nomatch'] += 1
            continue
        repls.sort(reverse=True)
        for s, e, src in repls:
            content = content[:s] + src + content[e:]
        stats['func'] += len(repls)

    open(path, 'w', encoding='utf-8', newline='').write(content)
    print(stats)
    cur = find_defs(content)
    c = collections.Counter(n for n, _, _ in cur)
    print('funcs:', len(cur), 'dups:', [k for k, v in c.items() if v > 1])


if __name__ == '__main__':
    main()
