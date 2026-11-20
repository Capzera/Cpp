import os
import re

ROOT = r'd:\Program\Cpp\Project\QT\SmartGround'
MODS = os.path.join(ROOT, 'src', 'modules')

DECL = re.compile(r'^[ \t]*(?:virtual\s+|static\s+|inline\s+)*'
                  r'(?:[\w:<>,\*&\[\]\s]+\s+)?\b(\w+)\s*\(([^;{}]*)\)'
                  r'\s*(?:const\s*)?(?:noexcept\s*)?(?:override\s*)?'
                  r'(?:=\s*[^;{]+)?\s*(;|\{\s*\}|[^;{]*)\s*$')


def sig_key(name, args):
    args = re.sub(r'\s+', '', args)
    args = re.sub(r'=default|=0', '', args)
    return name + '(' + args + ')'


def dedupe_header(path):
    lines = open(path, encoding='utf-8', newline='').read().replace('\r\n', '\n').split('\n')
    out = []
    seen = set()
    changed = False
    for line in lines:
        s = line.strip()
        if s.startswith('//') or not s:
            out.append(line)
            continue
        m = DECL.match(line)
        if m:
            key = sig_key(m.group(1), m.group(2))
            if key in seen:
                changed = True
                continue
            seen.add(key)
        out.append(line)
    if changed:
        open(path, 'w', encoding='utf-8', newline='').write('\n'.join(out))
    return changed


def dedupe_cpp(path):
    text = open(path, encoding='utf-8', newline='').read().replace('\r\n', '\n')
    cls = os.path.basename(path).replace('.cpp', '')
    pat = re.compile(r'^([^\n;{}]*\b' + re.escape(cls) + r'::(\w+)\s*\([^;]*\)\s*(?:const\s*)?\{)', re.M)
    spans = []
    for m in pat.finditer(text):
        i = text.find('{', m.end() - 1)
        if i < 0:
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
                    spans.append((m.group(2), m.start(), j + 1))
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
    groups = {}
    for name, s, e in spans:
        groups.setdefault(name, []).append((s, e))
    changed = False
    for name, lst in groups.items():
        if len(lst) < 2:
            continue
        keep = lst[-1]
        for s, e in reversed(lst[:-1]):
            text = text[:s] + text[e:]
            changed = True
    if changed:
        open(path, 'w', encoding='utf-8', newline='').write(text)
    return changed


def main():
    for name in sorted(os.listdir(MODS)):
        p = os.path.join(MODS, name)
        if name.endswith('.h'):
            if dedupe_header(p):
                print('dedup header', name)
        elif name.endswith('.cpp'):
            if dedupe_cpp(p):
                print('dedup cpp', name)


if __name__ == '__main__':
    main()
