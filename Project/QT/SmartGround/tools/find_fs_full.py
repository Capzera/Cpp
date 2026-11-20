import re

path = r'C:\Users\Administrator\.cursor\projects\d-Program-Cpp-Project-QT-SmartGround\agent-transcripts\a82040b8-8d2a-4a53-9f11-951587575635\a82040b8-8d2a-4a53-9f11-951587575635.jsonl'
best = None
with open(path, 'rb') as f:
    for ln, line in enumerate(f):
        s = line.decode('utf-8', errors='replace')
        if 'ForwardStarModule.cpp' not in s:
            continue
        # look for a long new_string / contents containing layoutArrays body
        for m in re.finditer(r'"(?:new_string|contents)":"((?:[^"\\]|\\.){6000,})"', s):
            txt = m.group(1)
            if 'layoutArrays' in txt and 'kFsHeadX' not in txt:
                if best is None or len(txt) > len(best[1]):
                    best = (ln, txt)
if best:
    ln, txt = best
    print('LINE', ln, 'LEN', len(txt))
    out = txt.encode('utf-8').decode('unicode_escape')
    open(r'd:\Program\Cpp\Project\QT\SmartGround\tools\_fs_full.cpp', 'w', encoding='utf-8').write(out)
    print(out[:600])
else:
    print('none')
