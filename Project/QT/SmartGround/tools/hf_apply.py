import json
import os

root = r'd:\Program\Cpp\Project\QT\SmartGround'
edits = json.load(open(os.path.join(root, 'tools', '_hf_edits.json'), encoding='utf-8'))

target = os.path.join(root, 'src', 'modules', 'HuffmanTreeModule.cpp')
text = open(target, 'rb').read().decode('utf-8', errors='replace')

applied = 0
skipped = 0
for ln, name, inp in edits:
    if name == 'Write':
        continue
    old = inp.get('old_string', '')
    new = inp.get('new_string', '')
    if old and old in text:
        text = text.replace(old, new, 1)
        applied += 1
    else:
        skipped += 1

open(target, 'wb').write(text.encode('utf-8'))
print('applied', applied, 'skipped', skipped)
print('JELLY', 'HF_JELLY' in text, 'QUEUE', 'HF_QUEUE_DROP' in text,
      'sym', 'layoutComponentSym' in text, 'sub', 'subtreeLayout' in text)
