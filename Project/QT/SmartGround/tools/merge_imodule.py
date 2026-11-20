import re
import subprocess
import sys

REPO = r'D:\Program\Cpp'
ROOT = r'd:\Program\Cpp\Project\QT\SmartGround'
CPP = ROOT + r'\src\modules\IModule.cpp'
CUR = ROOT + r'\tools\_im_cur.cpp'


def find_defs(t):
    res = []
    for m in re.finditer(r'^[^\n;{}]*\bIModule::(\w+)\s*\(', t, re.M):
        name = m.group(1)
        ls = t.rfind('\n', 0, m.start()) + 1
        i = t.find('{', m.end())
        if i < 0:
            continue
        seg = t[m.end():i]
        if seg.count('(') - seg.count(')') != -1 or ';' in seg:
            continue
        depth = 0
        j = i
        ok = False
        while j < len(t):
            c = t[j]
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
                while j < len(t) and t[j] != q:
                    if t[j] == '\\':
                        j += 1
                    j += 1
            j += 1
        if not ok:
            continue
    return res


def all_defs(t, name):
    return [t[s:e] for n, s, e in find_defs(t) if n == name]


def sig(text):
    m = re.search(r'IModule::\w+\s*\(([^)]*)\)', text)
    if not m:
        return ''
    return re.sub(r'\s+', ' ', m.group(1)).strip()


def main():
    head = subprocess.run(['git', 'show', 'HEAD:Project/QT/SmartGround/src/modules/IModule.cpp'],
                           cwd=REPO, capture_output=True).stdout.decode('utf-8-sig', errors='replace')
    head = head.replace('\r\n', '\n')
    cur = open(CUR, 'rb').read().decode('utf-8', errors='replace').replace('\r\n', '\n')

    head_names = {}
    for n, s, e in find_defs(head):
        head_names.setdefault(n, []).append((s, e))

    cur_names = {}
    for n, s, e in find_defs(cur):
        cur_names.setdefault(n, []).append((s, e))

    added = []
    for name, lst in cur_names.items():
        if name not in head_names:
            added.append((name, cur[lst[-1][0]:lst[-1][1]]))
        else:
            hlist = head_names[name]
            hset = {sig(head[s:e]) for s, e in hlist}
            cset = {sig(cur[s:e]) for s, e in lst}
            if cset - hset:
                print('DIFF SIG', name, 'cur-only:', cset - hset, 'head:', hset)

    print('to add:', len(added))
    for n, _ in added:
        print('  ', n)

    with open(sys.argv[1] if len(sys.argv) > 1 else CPP, 'wb') as f:
        f.write(head.encode('utf-8'))
    for n, body in added:
        with open(sys.argv[1] if len(sys.argv) > 1 else CPP, 'ab') as f:
            f.write(('\n' + body).encode('utf-8'))


if __name__ == '__main__':
    main()
