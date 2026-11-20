import os
import re
import sys

ROOT = r'd:\Program\Cpp\Project\QT\SmartGround\src\modules'

NARRATIVE = [
    'm_noteText', 'm_mathText', 'm_resultText', 'm_scanText', 'm_heapText',
    'm_windowText', 'm_probeText', 'm_stateText', 'm_relationText',
    'm_depthText', 'm_verdictText', 'm_accText', 'm_stepText', 'm_wordText',
    'm_gapText',
]

ASSIGN = re.compile(
    r'\b((?:m_|this->m_)(?:noteText|mathText|resultText|scanText|heapText|windowText|probeText|stateText|'
    r'relationText|depthText|verdictText|accText|stepText|wordText|gapText))\s*=\s*'
    r'((?:m_scene|this->m_scene|scene)\s*->\s*addText\s*\()')
PLAIN = re.compile(
    r'\b((?:m_|this->m_)(?:noteText|mathText|resultText|scanText|heapText|windowText|probeText|stateText|'
    r'relationText|depthText|verdictText|accText|stepText|wordText|gapText))\s*->\s*setPlainText\s*\(')
SCENE = re.compile(r'((?:m_scene|this->m_scene|scene))\s*->\s*addText\s*\(')


def match_paren(text, start):
    depth = 0
    i = start
    while i < len(text):
        c = text[i]
        if c == '"':
            i += 1
            while i < len(text):
                if text[i] == '\\':
                    i += 2
                    continue
                if text[i] == '"':
                    break
                i += 1
        elif c == '(':
            depth += 1
        elif c == ')':
            depth -= 1
            if depth == 0:
                return i
        i += 1
    return -1


def transform(text):
    changed = 0
    for _ in range(500):
        m = ASSIGN.search(text)
        if not m:
            break
        open_idx = text.find('(', m.start(2))
        close_idx = match_paren(text, open_idx)
        if close_idx < 0:
            break
        arg = text[open_idx + 1:close_idx]
        sc = SCENE.search(text[m.start(2):close_idx + 1]).group(1)
        text = text[:m.start(2)] + 'createScriptText(' + sc + ', ' + arg + ')' + text[close_idx + 1:]
        changed += 1

    for _ in range(800):
        m = PLAIN.search(text)
        if not m:
            break
        open_idx = text.find('(', m.end() - 1)
        close_idx = match_paren(text, open_idx)
        if close_idx < 0:
            break
        arg = text[open_idx + 1:close_idx]
        text = text[:m.start()] + 'setScriptText(' + m.group(1) + ', ' + arg + ')' + text[close_idx + 1:]
        changed += 1

    return text, changed


def main():
    total = 0
    for f in sorted(os.listdir(ROOT)):
        if not f.endswith('.cpp'):
            continue
        p = os.path.join(ROOT, f)
        text = open(p, 'rb').read().decode('utf-8', errors='replace')
        orig = text
        text, n = transform(text)
        if n:
            open(p, 'wb').write(text.encode('utf-8'))
            total += n
            print('updated %-42s %d' % (f, n))
    print('total', total)


if __name__ == '__main__':
    main()
