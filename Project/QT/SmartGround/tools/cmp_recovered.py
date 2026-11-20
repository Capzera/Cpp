import os
import subprocess
import sys

REPO = r'D:\Program\Cpp'
ROOT = r'd:\Program\Cpp\Project\QT\SmartGround'
REC = ROOT + r'\tools\_recovered'
MOD = ROOT + r'\src\modules'

for f in sorted(os.listdir(REC)):
    if not f.endswith(('.cpp', '.h')):
        continue
    rp = os.path.join(REC, f)
    mp = os.path.join(MOD, f)
    if not os.path.exists(mp):
        print('MISSING in modules:', f)
        continue
    a = open(rp, 'rb').read()
    b = open(mp, 'rb').read()
    if a == b:
        continue
    # is it tracked?
    r = subprocess.run(['git', 'ls-files', '--error-unmatch', 'Project/QT/SmartGround/src/modules/' + f],
                       cwd=REPO, capture_output=True)
    tracked = r.returncode == 0
    print(('TRACKED ' if tracked else 'untracked'), f, len(b), '->', len(a))
