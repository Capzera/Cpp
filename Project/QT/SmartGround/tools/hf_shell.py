import json
import os

tr = r'C:\Users\Administrator\.cursor\projects\d-Program-Cpp-Project-QT-SmartGround\agent-transcripts\a82040b8-8d2a-4a53-9f11-951587575635\a82040b8-8d2a-4a53-9f11-951587575635.jsonl'

out = []
for line in open(tr, encoding='utf-8'):
    try:
        ev = json.loads(line)
    except Exception:
        continue
    msg = ev.get('message') or {}
    content = msg.get('content')
    if not isinstance(content, list):
        continue
    for blk in content:
        if not isinstance(blk, dict) or blk.get('type') != 'tool_use':
            continue
        name = blk.get('name')
        args = blk.get('input')
        if not isinstance(args, dict):
            args = {}
        if name in ('Shell', 'Bash'):
            cmd = args.get('command', '')
            if 'uffman' in cmd or 'hf_' in cmd:
                out.append((name, cmd))
        elif name in ('Write', 'StrReplace', 'Edit'):
            p = args.get('path', '')
            if 'hf_' in p or 'uffman' in p:
                out.append((name, 'FILE ' + p))

print(len(out))
for n, c in out:
    print('----', n)
    print(c[:400])
