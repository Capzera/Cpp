import json
import os
import re

root = r'd:\Program\Cpp\Project\QT\SmartGround'
edits = json.load(open(os.path.join(root, 'tools', '_hf_edits.json'), encoding='utf-8'))

# The six-stage rewrite (HF_JELLY etc.) was produced by scripted rebuild passes
# rather than Write; look for it inside tools/_edits.json
raw = json.load(open(os.path.join(root, 'tools', '_edits.json'), encoding='utf-8'))
hits = []
if isinstance(raw, dict):
    items = list(raw.items())
else:
    items = list(enumerate(raw))
for k, v in items:
    s = json.dumps(v, ensure_ascii=False)
    if 'HF_JELLY' in s and 'layoutComponentSym' in s:
        hits.append((k, len(s)))
print('candidates in _edits.json:', len(hits))
for k, n in hits[:20]:
    print(' ', k, n)
