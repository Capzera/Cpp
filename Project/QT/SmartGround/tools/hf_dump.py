import json
import os

ROOT = r'd:\Program\Cpp\Project\QT\SmartGround'
calls = json.load(open(os.path.join(ROOT, 'tools', '_hf_calls.json'), encoding='utf-8'))
out = []
for i in range(79, 133):
    t, a = calls[i]
    b = os.path.basename(a['path'])
    out.append('=' * 70)
    out.append('CALL %d %s %s' % (i, t, b))
    if t == 'Write':
        out.append('--- CONTENTS ---')
        out.append(a.get('contents', ''))
    else:
        out.append('--- OLD ---')
        out.append(a.get('old_string', ''))
        out.append('--- NEW ---')
        out.append(a.get('new_string', ''))
p = os.path.join(ROOT, 'tools', '_hf_calls_79_132.txt')
open(p, 'w', encoding='utf-8', newline='\n').write('\n'.join(out))
print(p, len('\n'.join(out)))
