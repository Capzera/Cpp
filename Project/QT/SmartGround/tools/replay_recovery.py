import json
import os
import re
import collections

P = r'C:\Users\Administrator\.cursor\projects\d-Program-Cpp-Project-QT-SmartGround\agent-transcripts\a82040b8-8d2a-4a53-9f11-951587575635\a82040b8-8d2a-4a53-9f11-951587575635.jsonl'

events = []

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
            events.append(('W', fp, inp.get('contents', '')))
        elif name == 'StrReplace':
            events.append(('E', fp, inp.get('old_string', ''), inp.get('new_string', '')))

json.dump(events, open('tools/_events.json', 'w', encoding='utf-8'), ensure_ascii=False)
print('events', len(events))

targets = collections.defaultdict(list)
for ev in events:
    fp = ev[1]
    if 'src/modules' in fp and fp.endswith('.cpp'):
        targets[fp].append(ev)

print('module cpp files in transcript:', len(targets))

outdir = 'tools/_recovered'
os.makedirs(outdir, exist_ok=True)
state = {}
for fp, evs in targets.items():
    content = None
    for ev in evs:
        if ev[0] == 'W':
            content = ev[2]
        else:
            if content is None:
                continue
            old, new = ev[2], ev[3]
            if old and old in content:
                content = content.replace(old, new, 1)
    if content:
        state[fp] = content

for fp, content in state.items():
    dest = os.path.join(outdir, os.path.basename(fp))
    with open(dest, 'w', encoding='utf-8', newline='') as f:
        f.write(content)

print('replayed files:', len(state))
for fp in sorted(state):
    print('  ', os.path.basename(fp), len(state[fp]))
