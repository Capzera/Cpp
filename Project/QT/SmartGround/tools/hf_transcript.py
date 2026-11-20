import json
import re

path = r'C:\Users\Administrator\.cursor\projects\d-Program-Cpp-Project-QT-SmartGround\agent-transcripts\a82040b8-8d2a-4a53-9f11-951587575635\a82040b8-8d2a-4a53-9f11-951587575635.jsonl'

found = []
with open(path, 'rb') as f:
    for ln, line in enumerate(f):
        s = line.decode('utf-8', errors='replace')
        if 'HuffmanTreeModule.cpp' not in s:
            continue
        try:
            obj = json.loads(s)
        except Exception:
            continue
        stack = [obj]
        while stack:
            o = stack.pop()
            if isinstance(o, dict):
                if o.get('type') == 'tool_use' and o.get('name') in ('Write', 'StrReplace', 'Edit'):
                    inp = o.get('input', {})
                    fp = inp.get('path', '')
                    if fp.endswith('HuffmanTreeModule.cpp'):
                        found.append((ln, o.get('name'), inp))
                stack.extend(o.values())
            elif isinstance(o, list):
                stack.extend(o)

print('edits found:', len(found))
for ln, name, inp in found:
    body = inp.get('contents') or inp.get('new_string') or ''
    print('line', ln, name, 'len', len(body),
          'JELLY' if 'HF_JELLY' in body else '',
          'QUEUE' if 'HF_QUEUE_DROP' in body else '',
          'subtreeLayout' if 'subtreeLayout' in body else '',
          'layoutComponentSym' if 'layoutComponentSym' in body else '')

with open(r'd:\Program\Cpp\Project\QT\SmartGround\tools\_hf_edits.json', 'w', encoding='utf-8') as out:
    json.dump(found, out, ensure_ascii=False)
