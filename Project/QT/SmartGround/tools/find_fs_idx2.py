import json
import re

path = r'C:\Users\Administrator\.cursor\projects\d-Program-Cpp-Project-QT-SmartGround\agent-transcripts\a82040b8-8d2a-4a53-9f11-951587575635\a82040b8-8d2a-4a53-9f11-951587575635.jsonl'
hits = []
with open(path, 'rb') as f:
    for ln, line in enumerate(f):
        s = line.decode('utf-8', errors='replace')
        if 'ForwardStarModule.cpp' not in s:
            continue
        hits.append((ln, s))
print('lines with FS:', len(hits))

pat = re.compile(r'addIndexRow|kFsTopY \+ kFsRowGap')
for ln, s in hits:
    for m in re.finditer(r'addIndexRow', s):
        seg = s[max(0, m.start() - 1500):m.start() + 2500]
        if 'void ForwardStarModule::addIndexRow' in seg:
            print('=== line', ln)
            i = seg.find('void ForwardStarModule::addIndexRow')
            print(seg[max(0, i - 300):i + 2000].replace('\\n', '\n'))
            raise SystemExit
