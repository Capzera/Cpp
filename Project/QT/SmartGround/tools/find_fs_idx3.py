import re

path = r'C:\Users\Administrator\.cursor\projects\d-Program-Cpp-Project-QT-SmartGround\agent-transcripts\a82040b8-8d2a-4a53-9f11-951587575635\a82040b8-8d2a-4a53-9f11-951587575635.jsonl'
terms = ['addIndexRow', 'indexRow', 'kFsIndex', 'layoutArrays']
counts = {t: 0 for t in terms}
samples = {t: [] for t in terms}
with open(path, 'rb') as f:
    for ln, line in enumerate(f):
        s = line.decode('utf-8', errors='replace')
        for t in terms:
            n = s.count(t)
            if n:
                counts[t] += n
                if len(samples[t]) < 4:
                    i = s.find(t)
                    samples[t].append((ln, s[max(0, i - 200):i + 400].replace('\\n', '\n')))
print(counts)
for t in terms:
    for ln, seg in samples[t]:
        print('=====', t, ln)
        print(seg[:600])
