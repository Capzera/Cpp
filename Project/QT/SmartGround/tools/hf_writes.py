import json
import os
import re

root = r'd:\Program\Cpp\Project\QT\SmartGround'
edits = json.load(open(os.path.join(root, 'tools', '_hf_edits.json'), encoding='utf-8'))

for ln, name, inp in edits:
    if name != 'Write':
        continue
    body = inp.get('contents', '')
    flags = []
    for k in ('HF_JELLY', 'HF_QUEUE_DROP', 'layoutComponentSym', 'subtreeLayout', 'HF_SORT_LIFT', 'm_compRank'):
        if k in body:
            flags.append(k)
    print(ln, len(body), flags)
