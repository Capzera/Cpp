import json
import re

path = r'C:\Users\Administrator\.cursor\projects\d-Program-Cpp-Project-QT-SmartGround\agent-transcripts\a82040b8-8d2a-4a53-9f11-951587575635\a82040b8-8d2a-4a53-9f11-951587575635.jsonl'
pat = re.compile(r'kFsHeadX\s*=\s*[0-9.]+;\s*\\nstatic const double kFsToX\s*=\s*[0-9.]+;\s*\\nstatic const double kFsNxtX\s*=\s*[0-9.]+;\s*\\nstatic const double kFsWeightX\s*=\s*[0-9.]+;')
best = None
with open(path, 'rb') as f:
    for line in f:
        s = line.decode('utf-8', errors='replace')
        for m in pat.finditer(s):
            best = m.group(0)
        if 'kFsHeadX' in s:
            i = s.find('kFsHeadX')
            seg = s[max(0, i - 400):i + 600]
            if 'kFsWeightX' in seg:
                print('SNIP:', seg.replace('\\n', '\n')[:1000])
                print('======')
