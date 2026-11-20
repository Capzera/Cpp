import json
import collections
import os

P = r'C:\Users\Administrator\.cursor\projects\d-Program-Cpp-Project-QT-SmartGround\agent-transcripts\a82040b8-8d2a-4a53-9f11-951587575635\a82040b8-8d2a-4a53-9f11-951587575635.jsonl'

writes = collections.defaultdict(list)
reads = []
edits = []

for line in open(P, encoding='utf-8'):
    try:
        d = json.loads(line)
    except Exception:
        continue
    msg = d.get('message', {})
    c = msg.get('content')
    if not isinstance(c, list):
        continue
    for part in c:
        if not isinstance(part, dict) or part.get('type') != 'tool_use':
            continue
        name = part.get('name')
        inp = part.get('input', {}) or {}
        fp = str(inp.get('path', '')).replace('\\', '/')
        if name == 'Write':
            writes[fp].append(inp.get('contents', ''))
        elif name == 'Read':
            reads.append((fp, inp.get('offset'), inp.get('limit')))
        elif name == 'StrReplace':
            edits.append((fp, inp.get('old_string', ''), inp.get('new_string', '')))

print('write files:', len(writes))
print('edits:', len(edits))

out = {}
for fp, lst in writes.items():
    if 'src/modules' not in fp or not fp.endswith('.cpp'):
        continue
    out[fp] = lst[-1]

os.makedirs('tools/_recovered', exist_ok=True)
for fp, content in out.items():
    dest = os.path.join('tools/_recovered', os.path.basename(fp))
    with open(dest, 'w', encoding='utf-8', newline='') as f:
        f.write(content)
print('saved', len(out))

with open('tools/_edits.json', 'w', encoding='utf-8') as f:
    json.dump(edits, f, ensure_ascii=False)

with open('tools/_reads.json', 'w', encoding='utf-8') as f:
    json.dump(reads, f, ensure_ascii=False)
