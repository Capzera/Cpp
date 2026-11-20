import difflib
import json
import os
import re
import subprocess
import sys

REPO = r'D:\Program\Cpp'
ROOT = r'd:\Program\Cpp\Project\QT\SmartGround'
MODS = os.path.join(ROOT, 'src', 'modules')

SIG = re.compile(r'^\s*(?:[\w:<>,\*&\[\]\s]+\s+)?[\w:~]+\s+[\w:~]+::\w+\s*\(')


def norm(s):
    return [l.rstrip() for l in s.replace('\r\n', '\n').split('\n')]


def find_defs(text, cls=None):
    pat = r'^[^\n;{}]*\b' + (re.escape(cls) + '::' if cls else '') + r'(\w+)\s*\('
    res = []
    for m in re.finditer(pat, text, re.M):
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


def grab(text, name, cls=None):
    for n, s, e in find_defs(text, cls):
        if n == name:
            return text[s:e]
    return None


def ratio(a, b):
    return difflib.SequenceMatcher(None, a, b, autojunk=False).ratio()


def apply_one(content, old, new):
    """Return (content|None, kind)."""
    if old in content:
        return content.replace(old, new, 1), 'exact'
    if new in content:
        return content, 'already'
    ol = norm(old)
    nl = norm(new)

    # case 1: OLD starts a whole function definition -> func-level replace
    if SIG.match(old.lstrip()) and ol[0].strip():
        cls = None
        m = re.search(r'\b(\w+)::(\w+)\s*\(', old)
        if m:
            cls, fn = m.group(1), m.group(2)
            cur = grab(content, fn, cls)
            rep = grab(new, fn, cls)
            if cur and rep:
                if ratio(norm(cur), norm(rep)) > 0.95:
                    return content, 'already'
                return content.replace(cur, rep, 1), 'func'

    # case 2: locate OLD region by first-line anchor, fuzzy replace
    first = ol[0].strip()
    cl = content.split('\n')
    cls_strip = [l.strip() for l in cl]
    cands = [i for i, l in enumerate(cls_strip) if l == first]
    if not cands and len(first) > 12:
        cands = [i for i, l in enumerate(cls_strip) if l.startswith(first[:40])]
    best = None
    for i in cands:
        for span in (len(ol), len(ol) + 1, len(ol) - 1, len(ol) + 3, len(ol) - 3,
                     len(ol) + 8, len(ol) - 8):
            if span <= 0 or i + span > len(cls_strip):
                continue
            r = difflib.SequenceMatcher(None, cls_strip[i:i + span], ol, autojunk=False).ratio()
            if best is None or r > best[0]:
                best = (r, i, span)
    if best is None:
        return None, 'nofind'
    r_old, i, span = best
    r_new = difflib.SequenceMatcher(None, cls_strip[i:i + span], [l.strip() for l in nl],
                                    autojunk=False).ratio()
    if r_new >= 0.9 and r_new >= r_old:
        return content, 'already'
    if r_old < 0.45:
        return None, 'lowsim'
    cl[i:i + span] = new.replace('\r\n', '\n').split('\n')
    return '\n'.join(cl), 'fuzzy'


def main():
    events = json.load(open(os.path.join(ROOT, 'tools', '_events.json'), encoding='utf-8'))
    headnames = set(x.strip().split('/')[-1] for x in
                    open(os.path.join(ROOT, 'tools', '_head_mods.txt'), encoding='utf-8-sig') if x.strip())
    names = sorted(n for n in os.listdir(MODS) if n.endswith('.cpp') or n.endswith('.h'))
    stats = {}
    report = []
    for name in names:
        rel = 'Project/QT/SmartGround/src/modules/' + name
        evs = [e for e in events if e[1].replace('\\', '/').endswith('/' + name)]
        r = subprocess.run(['git', 'show', 'HEAD:' + rel], cwd=REPO, capture_output=True)
        content = None
        if r.returncode == 0 and r.stdout and name in headnames:
            content = r.stdout.decode('utf-8-sig', errors='replace').replace('\r\n', '\n')
        if content is None:
            last_w = -1
            for i, ev in enumerate(evs):
                if ev[0] == 'W':
                    last_w = i
            if last_w < 0:
                report.append((name, 'NOBASE'))
                continue
            content = evs[last_w][2].replace('\r\n', '\n')
            evs = evs[last_w + 1:]
        miss = []
        for i, ev in enumerate(evs):
            if ev[0] != 'E':
                continue
            old = ev[2].replace('\r\n', '\n')
            new = ev[3].replace('\r\n', '\n')
            if not old.strip() or not new.strip():
                continue
            res, kind = apply_one(content, old, new)
            if res is None:
                miss.append(i)
                continue
            content = res
            stats[kind] = stats.get(kind, 0) + 1
        open(os.path.join(MODS, name), 'w', encoding='utf-8', newline='').write(content)
        if miss:
            report.append((name, miss))
    print(stats)
    for n, m in report:
        print(n, m)


if __name__ == '__main__':
    main()
