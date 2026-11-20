import json
import os
import re

TRANS = r'C:\Users\Administrator\.cursor\projects\d-Program-Cpp-Project-QT-SmartGround\agent-transcripts\a82040b8-8d2a-4a53-9f11-951587575635\a82040b8-8d2a-4a53-9f11-951587575635.jsonl'
OUT = r'd:\Program\Cpp\Project\QT\SmartGround\tools\_reads_raw.json'


def walk(obj):
    if isinstance(obj, dict):
        for k, v in obj.items():
            yield k, v
            yield from walk(v)
    elif isinstance(obj, list):
        for v in obj:
            yield from walk(v)


snaps = {}
n = 0
for line in open(TRANS, encoding='utf-8'):
    try:
        d = json.loads(line)
    except Exception:
        continue
    for k, v in walk(d):
        if not isinstance(v, str):
            continue
        if 'IModule.cpp' not in v:
            continue
        n += 1
        if len(v) > 2000:
            snaps.setdefault(len(v), v)

print('candidate strings', n, 'snapshots', len(snaps))
json.dump({str(k): v for k, v in snaps.items()}, open(OUT, 'w', encoding='utf-8'))
for k in sorted(snaps):
    print(k, repr(snaps[k][:120]))
