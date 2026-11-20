import json
import os
import re

root = r'd:\Program\Cpp\Project\QT\SmartGround'
edits = json.load(open(os.path.join(root, 'tools', '_hf_edits.json'), encoding='utf-8'))

base = None
for ln, name, inp in edits:
    if name == 'Write' and ln == 9556:
        base = (ln, inp.get('contents', ''))
text = base[1]
print('base from write', base[0], len(text))

applied = 0
for ln, name, inp in edits:
    if ln <= base[0] or name == 'Write':
        continue
    old = inp.get('old_string', '')
    new = inp.get('new_string', '')
    if not old:
        continue
    if old in text:
        text = text.replace(old, new, 1)
        applied += 1
    else:
        print('  MISS', ln, len(new), repr(old[:70]))

out = os.path.join(root, 'tools', '_hf_replay.cpp')
open(out, 'w', encoding='utf-8').write(text)
print('applied', applied, 'final len', len(text))
for k in ('HF_JELLY', 'HF_QUEUE_DROP', 'layoutComponentSym', 'subtreeLayout', 'm_compRank', 'HF_SORT_LIFT'):
    print(k, k in text)
print('funcs', sorted(set(re.findall(r'HuffmanTreeModule::(\w+)', text))))
