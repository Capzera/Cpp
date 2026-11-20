import json
import re

path = r'C:\Users\Administrator\.cursor\d-Program-Cpp-Project-QT-SmartGround'
path = r'C:\Users\Administrator\.cursor\projects\d-Program-Cpp-Project-QT-SmartGround\agent-transcripts\a82040b8-8d2a-4a53-9f11-951587575635\a82040b8-8d2a-4a53-9f11-951587575635.jsonl'
edits = []
with open(path, 'rb') as f:
    for ln, line in enumerate(f):
        s = line.decode('utf-8', errors='replace')
        if 'ForwardStarModule.cpp' not in s:
            continue
        for m in re.finditer(r'\{"role":"assistant".*?"name":"(StrReplace|Write)".*?"input":(\{.*?\})\}\}\]', s):
            try:
                inp = json.loads(m.group(2))
            except Exception:
                continue
            edits.append((ln, m.group(1), inp))
print('edits', len(edits))
for ln, name, inp in edits:
    print('---', ln, name, inp.get('path'))
    if name == 'StrReplace':
        print('OLD:', repr(inp.get('old_string'))[:1500])
        print('NEW:', repr(inp.get('new_string'))[:1500])
