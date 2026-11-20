import json
import os

TRANS = r'C:\Users\Administrator\.cursor\projects\d-Program-Cpp-Project-QT-SmartGround\agent-transcripts\a82040b8-8d2a-4a53-9f11-951587575635\a82040b8-8d2a-4a53-9f11-951587575635.jsonl'
OUT = r'd:\Program\Cpp\Project\QT\SmartGround\tools\_read_base.json'


def iter_msgs():
    for line in open(TRANS, encoding='utf-8'):
        line = line.strip()
        if not line:
            continue
        try:
            d = json.loads(line)
        except Exception:
            continue
        msg = d.get('message')
        if isinstance(msg, dict):
            yield msg


def main():
    pending = []
    bases = {}
    count = 0
    for msg in iter_msgs():
        content = msg.get('content')
        if not isinstance(content, list):
            continue
        results = [b for b in content if isinstance(b, dict) and b.get('type') == 'tool_result']
        uses = [b for b in content if isinstance(b, dict) and b.get('type') == 'tool_use']
        for b in uses:
            pending.append(b)
        for b in results:
            if not pending:
                continue
            call = pending.pop(0)
            name = call.get('name')
            if name != 'Read':
                continue
            inp = call.get('input') or {}
            path = inp.get('path') or ''
            if not path:
                continue
            txt = b.get('content')
            if isinstance(txt, list):
                txt = '\n'.join(x.get('text', '') for x in txt if isinstance(x, dict))
            if not isinstance(txt, str) or len(txt) < 100:
                continue
            key = path.replace('\\', '/').lower()
            if key not in bases:
                bases[key] = txt
                count += 1
    json.dump(bases, open(OUT, 'w', encoding='utf-8'), ensure_ascii=False)
    print('bases', count)
    for k in sorted(bases):
        print(k, len(bases[k]))


if __name__ == '__main__':
    main()
