import json
import os
import re

root = r'd:\Program\Cpp\Project\QT\SmartGround'
edits = json.load(open(os.path.join(root, 'tools', '_hf_edits.json'), encoding='utf-8'))

text = ''
for ln, name, inp in edits:
    if name == 'Write':
        text = inp.get('contents', '')
        print('WRITE at', ln, len(text))
    else:
        old = inp.get('old_string', '')
        new = inp.get('new_string', '')
        if not old:
            continue
        if old in text:
            text = text.replace(old, new, 1)
            print('  ok', ln, len(new))
        else:
            print('  MISS', ln, len(new), repr(old[:60]))

out = os.path.join(root, 'tools', '_hf_replay.cpp')
open(out, 'w', encoding='utf-8').write(text)
print('---- final len', len(text))
for k in ('HF_JELLY', 'HF_QUEUE_DROP', 'layoutComponentSym', 'subtreeLayout', 'm_compRank', 'HF_SORT_LIFT'):
    print(k, k in text)
print('funcs', sorted(set(re.findall(r'HuffmanTreeModule::(\w+)', text))))
