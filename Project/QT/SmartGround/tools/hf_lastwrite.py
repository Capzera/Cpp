import json
import os

root = r'd:\Program\Cpp\Project\QT\SmartGround'
edits = json.load(open(os.path.join(root, 'tools', '_hf_edits.json'), encoding='utf-8'))

writes = [(ln, inp) for ln, name, inp in edits if name == 'Write']
writes.sort(key=lambda x: x[0])
ln, inp = writes[-1]
body = inp.get('contents', '')
print('last Write at line', ln, 'len', len(body))
print('JELLY', 'HF_JELLY' in body, 'QUEUE', 'HF_QUEUE_DROP' in body,
      'sym', 'layoutComponentSym' in body, 'sub', 'subtreeLayout' in body)
print('functions:', sorted(set(__import__('re').findall(r'HuffmanTreeModule::(\w+)', body))))
open(os.path.join(root, 'tools', '_hf_write.cpp'), 'w', encoding='utf-8').write(body)
