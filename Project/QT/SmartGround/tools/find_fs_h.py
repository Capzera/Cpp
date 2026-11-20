import re

path = r'C:\Users\Administrator\.cursor\projects\d-Program-Cpp-Project-QT-SmartGround\agent-transcripts\a82040b8-8d2a-4a53-9f11-951587575635\a82040b8-8d2a-4a53-9f11-951587575635.jsonl'
best = None
with open(path, 'rb') as f:
    for ln, line in enumerate(f):
        s = line.decode('utf-8', errors='replace')
        for m in re.finditer(r'"(?:new_string|contents)":"((?:[^"\\]|\\.){2500,})"', s):
            txt = m.group(1)
            if 'm_arraysBottomY' in txt and 'layoutArrays' in txt:
                if best is None or ln > best[0]:
                    best = (ln, txt)
if best:
    ln, txt = best
    print('LINE', ln, 'LEN', len(txt))
    out = txt.encode('utf-8').decode('unicode_escape')
    try:
        out = out.encode('latin-1').decode('utf-8')
    except Exception:
        pass
    open(r'd:\Program\Cpp\Project\QT\SmartGround\tools\_fs_h.cpp', 'w', encoding='utf-8').write(out)
    print(out[:300])
else:
    print('none')
