import json
import os

root = r'd:\Program\Cpp\Project\QT\SmartGround'
calls = json.load(open(os.path.join(root, 'tools', '_hf_calls.json'), encoding='utf-8'))

state = {}
for path in ('src/modules/HuffmanTreeModule.h', 'src/modules/HuffmanTreeModule.cpp'):
    state[path] = ''

misses = []
for i, (t, a) in enumerate(calls):
    p = a['path'].replace('/', '\\')
    for k in list(state):
        if k.endswith(os.path.basename(p)):
            p = k
    if t == 'Write':
        state[p] = a.get('contents', '')
        continue
    if t == 'Edit':
        old = a.get('old_string', '')
        new = a.get('new_string', '')
    else:
        old = a.get('old_string', '')
        new = a.get('new_string', '')
    if not old:
        continue
    if old in state[p]:
        state[p] = state[p].replace(old, new, 1)
    else:
        misses.append((i, t, p, old[:70]))
        state[p] += '\n' + new

for p, txt in state.items():
    out = os.path.join(root, 'tools', '_replay_' + os.path.basename(p))
    open(out, 'w', encoding='utf-8', newline='').write(txt)
    print(p, len(txt))

print('misses', len(misses))
for m in misses:
    print('  ', m)
