import os

root = r'd:\Program\Cpp\Project\QT\SmartGround\src\modules'
for f in sorted(os.listdir(root)):
    if not f.endswith(('.cpp', '.h')):
        continue
    t = open(os.path.join(root, f), 'rb').read().decode('utf-8', errors='replace')
    for i, l in enumerate(t.split('\n'), 1):
        if l.count('"') % 2 == 1:
            print(f + ':' + str(i) + ': ' + l.strip())
