import json
import os
import re

ROOT = r'd:\Program\Cpp\Project\QT\SmartGround'
calls = json.load(open(os.path.join(ROOT, 'tools', '_hf_calls.json'), encoding='utf-8'))

TOKEN = re.compile(r'\S+')


def tokens_of(s):
    out = []
    for m in TOKEN.finditer(s):
        out.append((m.group(0), m.start(), m.end()))
    return out


def fuzzy_replace(content, old, new):
    if not old:
        return None
    if old in content:
        return content.replace(old, new, 1)
    ot = tokens_of(old)
    if not ot:
        return None
    ct = tokens_of(content)
    key = [t[0] for t in ot]
    ckey = [t[0] for t in ct]
    n = len(key)
    first = key[0]
    for i in range(len(ckey)):
        if ckey[i] != first:
            continue
        if ckey[i:i + n] == key:
            start = ct[i][1]
            end = ct[i + n - 1][2]
            return content[:start] + new + content[end:]
    return None


state = {}
order = []
for t, a in calls:
    p = a['path'].replace('/', '\\')
    key = None
    for k in state:
        if k.endswith(os.path.basename(p)):
            key = k
            break
    if key is None:
        key = p
        state[key] = ''
        order.append(key)
    if t == 'Write':
        state[key] = a.get('contents', '')
        continue
    old = a.get('old_string', '')
    new = a.get('new_string', '')
    if not old:
        continue
    r = fuzzy_replace(state[key], old, new)
    if r is None:
        print('MISS', key.split('\\')[-1], repr(old[:70]))
        continue
    state[key] = r

for k in order:
    if 'HuffmanTreeModule' not in k:
        continue
    out = os.path.join(ROOT, 'tools', '_replay_' + os.path.basename(k))
    open(out, 'w', encoding='utf-8', newline='').write(state[k])
    print('wrote', out, len(state[k]))
