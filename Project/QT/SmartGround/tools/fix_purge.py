import re

path = 'src/modules/IModule.cpp'
with open(path, encoding='utf-8', newline='') as f:
    text = f.read()

old = ('void IModule::purgeTransientItems() {\n'
       '    for (auto& item : m_transientItems) {\n'
       '        if (!item) continue;\n'
       '        QGraphicsItem* g = item.data();\n'
       '        if (!g) continue;\n'
       '        if (g->scene()) g->scene()->removeItem(g);\n'
       '        delete g;\n'
       '    }\n'
       '    m_transientItems.clear();\n'
       '}\n\n')

assert old in text
text = text.replace(old, '', 1)

with open(path, 'w', encoding='utf-8', newline='') as f:
    f.write(text)

import collections
defs = re.findall(r'IModule::(\w+)\s*\(', text)
print({k: v for k, v in collections.Counter(defs).items() if v > 1})
print('len', len(text))
