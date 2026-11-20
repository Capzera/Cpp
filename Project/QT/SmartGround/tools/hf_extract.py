import json
import os
import re

root = r'd:\Program\Cpp\Project\QT\SmartGround'
tr = r'C:\Users\Administrator\.cursor\projects\d-Program-Cpp-Project-QT-SmartGround\agent-transcripts\a82040b8-8d2a-4a53-9f11-951587575635\a82040b8-8d2a-4a53-9f11-951587575635.jsonl'

calls = []
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
            continue
        if name in ('Write', 'StrReplace', 'Edit'):
            p = args.get('path', '')
            if 'Huffman' in p:
                calls.append((name, args))

print('huffman calls', len(calls))
for i, (t, a) in enumerate(calls):
    print(i, t, os.path.basename(a['path']), len(a.get('contents', '') or a.get('new_string', '') or a.get('old_string', '')))
json.dump(calls, open(os.path.join(root, 'tools', '_hf_calls.json'), 'w', encoding='utf-8'), ensure_ascii=False)
