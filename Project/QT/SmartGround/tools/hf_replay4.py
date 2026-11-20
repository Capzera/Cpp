import json
import os
import re

ROOT = r'd:\Program\Cpp\Project\QT\SmartGround'
calls = json.load(open(os.path.join(ROOT, 'tools', '_hf_calls.json'), encoding='utf-8'))


def loose_replace(content, old, new):
    if not old:
        return None
    if old in content:
        return content.replace(old, new, 1)
    pat = re.escape(old)
    pat = pat.replace(r'\ ', r'[ \t]+')
    pat = pat.replace('\n', r'\s*\n\s*')
    try:
        m = re.search(pat, content)
    except re.error:
        return None
    if not m:
        return None
    return content[:m.start()] + new + content[m.end():]


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
    r = loose_replace(state[key], old, new)
    if r is None:
        print('MISS', key.split('\\')[-1], repr(old[:70]))
        continue
    state[key] = r

for k in order:
    if 'HuffmanTreeModule' not in k:
        continue
    out = os.path.join(ROOT, 'tools', '_replay2_' + os.path.basename(k))
    open(out, 'w', encoding='utf-8', newline='').write(state[k])
    print('wrote', out, len(state[k]))
