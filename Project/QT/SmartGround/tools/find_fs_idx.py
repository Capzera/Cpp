import json
import re

path = r'C:\Users\Administrator\.cursor\projects\d-Program-Cpp-Project-QT-SmartGround\agent-transcripts\a82040b8-8d2a-4a53-9f11-951587575635\a82040b8-8d2a-4a53-9f11-951587575635.jsonl'
out = []
with open(path, 'rb') as f:
    for line in f:
        s = line.decode('utf-8', errors='replace')
        if 'addIndexRow' in s:
            for m in re.finditer(r'addIndexRow', s):
                seg = s[max(0, m.start() - 900):m.start() + 900]
                if 'void ForwardStarModule::addIndexRow' in seg:
                    out.append(seg)
print('hits', len(out))
if out:
    s = out[-1]
    i = s.find('void ForwardStarModule::addIndexRow')
    print(s[max(0, i - 1500):i + 2500].replace('\\n', '\n'))
