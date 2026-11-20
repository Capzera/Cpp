import json
import re
import sys

path = r'C:\Users\Administrator\.cursor\projects\d-Program-Cpp-Project-QT-SmartGround\agent-transcripts\a82040b8-8d2a-4a53-9f11-951587575635\a82040b8-8d2a-4a53-9f11-951587575635.jsonl'
pat = re.compile(r'kFsHeadX\s*=\s*([0-9.]+)|kFsToX\s*=\s*([0-9.]+)|kFsNxtX\s*=\s*([0-9.]+)|kFsWeightX\s*=\s*([0-9.]+)')
found = {}
with open(path, 'rb') as f:
    for line in f:
        try:
            obj = json.loads(line.decode('utf-8', errors='replace'))
        except Exception:
            continue
        s = json.dumps(obj, ensure_ascii=False)
        for m in pat.finditer(s):
            for name in ('kFsHeadX', 'kFsToX', 'kFsNxtX', 'kFsWeightX'):
                mm = re.search(name + r'\s*=\s*([0-9.]+)', s)
                if mm:
                    found.setdefault(name, set()).add(mm.group(1))
print(found)

# also search for addIndexRow definition
with open(path, 'rb') as f:
    for line in f:
        s = line.decode('utf-8', errors='replace')
        if 'addIndexRow' in s and 'void ForwardStarModule::addIndexRow' in s:
            i = s.find('void ForwardStarModule::addIndexRow')
            print(repr(s[i:i+700]))
            print('---')
