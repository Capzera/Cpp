import os
import re

ROOT = r'd:\Program\Cpp\Project\QT\SmartGround\src\modules'


def read(p):
    return open(p, 'rb').read().decode('utf-8', errors='replace')


def write(p, t):
    open(p, 'wb').write(t.encode('utf-8'))


def sub_once(text, old, new, label):
    if old not in text:
        print('  MISS', label)
        return text, False
    if text.count(old) > 1:
        print('  AMBIGUOUS', label, text.count(old))
        return text, False
    print('  ok', label)
    return text.replace(old, new), True


P = os.path.join(ROOT, 'TopologicalSortModule.cpp')
t = read(P)
t, _ = sub_once(t, '''std::map<int, QPointF> TopologicalSortModule::captureNodePositions() const {
    return GraphRenderHelper::captureNodePositions(m_nodes);
}
void TopologicalSortModule::restoreNodePositodule::getInputHint() const {
    return "第一行输入两个正整数：顶点数 n、边数 m\\n"
           "接下来 m 行，每行两个整数 u, v，表示一条有向边 u -> v\\n"
           "若任意一行写了三个数
}
void TopologicalSortModule::restoreNodePositions''',
             '''QString TopologicalSortModule::getInputHint() const {
    return "第一行输入两个正整数：顶点数 n、边数 m\\n"
           "接下来 m 行，每行两个整数 u, v，表示一条有向边 u -> v\\n"
           "若任意一行写了三个数 u v w，则整张图按带权图处理";
}
std::map<int, QPointF> TopologicalSortModule::captureNodePositions() const {
    return GraphRenderHelper::captureNodePositions(m_nodes);
}
void TopologicalSortModule::restoreNodePositions''', 'topo hint')
write(P, t)

P = os.path.join(ROOT, 'DijkstraNaiveModule.cpp')
t = read(P)
t, _ = sub_once(t, '''std::map<int, QPointF> DijkstraNaiveModule::captureNodePositions() const {
    return GraphRenderHelper::captureNodePositions(m_nodes);
}
void DijkstraNaiveModule::restoreNodePositodule::getInputHint() const {
    return "第一行输入三个正整数：顶点数 n、边数 m、源点 src\\n"
           "接下来 m 行，每行三个整数 u, v, w，表示一条权值为 w 的边\\n"
           "权
}
void DijkstraNaiveModule::restoreNodePositions''',
             '''QString DijkstraNaiveModule::getInputHint() const {
    return "第一行输入三个正整数：顶点数 n、边数 m、源点 src\\n"
           "接下来 m 行，每行三个整数 u, v, w，表示一条权值为 w 的边\\n"
           "权值必须非负，否则本模块拒绝该输入";
}
std::map<int, QPointF> DijkstraNaiveModule::captureNodePositions() const {
    return GraphRenderHelper::captureNodePositions(m_nodes);
}
void DijkstraNaiveModule::restoreNodePositions''', 'dijkstra hint')
write(P, t)

P = os.path.join(ROOT, 'KruskalModule.cpp')
t = read(P)
t, _ = sub_once(t, '''std::map<int, QPointF> KruskalModule::captureNodePositions() const {
    return GraphRenderHelper::captureNodePositions(m_nodes);
}
void KruskalModule::restoreNodePositions(constInputHint() const {
    return "第一行输入两个正整数：顶点数 n、边数 m\\n"
           "接下来 m 行，每行三个整数 u, v, w，表示一条无向边 u - v，权值为 w\\n"
           "本模块固定
}
void KruskalModule::restoreNodePositions''',
             '''QString KruskalModule::getInputHint() const {
    return "第一行输入两个正整数：顶点数 n、边数 m\\n"
           "接下来 m 行，每行三个整数 u, v, w，表示一条无向边 u - v，权值为 w\\n"
           "本模块固定按无向图处理，可勾选双向箭头显示";
}
std::map<int, QPointF> KruskalModule::captureNodePositions() const {
    return GraphRenderHelper::captureNodePositions(m_nodes);
}
void KruskalModule::restoreNodePositions''', 'kruskal hint')
write(P, t)

P = os.path.join(ROOT, 'BFSModule.cpp')
t = read(P)
t, _ = sub_once(t, '''std::map<int, QPointF> BFSModule::captureNodePositions() const {
    return GraphRenderHelper::captureNodePositions(m_nodes);
}
void BFSModule::restoreNodePositions(const std::int() const {
    return "第一行输入三个正整数：顶点数 n、边数 m、起点 src\\n"
           "接下来 m 行，每行两个整数 u, v，表示一条边 u - v\\n"
           "若任意一行写了三个数
}
void BFSModule::restoreNodePositions''',
             '''QString BFSModule::getInputHint() const {
    return "第一行输入三个正整数：顶点数 n、边数 m、起点 src\\n"
           "接下来 m 行，每行两个整数 u, v，表示一条边 u - v\\n"
           "若任意一行写了三个数 u v w，则整张图按带权图处理";
}
std::map<int, QPointF> BFSModule::captureNodePositions() const {
    return GraphRenderHelper::captureNodePositions(m_nodes);
}
void BFSModule::restoreNodePositions''', 'bfs hint')
write(P, t)

P = os.path.join(ROOT, 'SegmentTreeEditBase.cpp')
t = read(P)
t, _ = sub_once(t, '''void SegmentTreeEditBase::setFormula(const QString& text) {
    if (m_mathText) m_mathText->setPlainText(text);
}


    if (row == "tr") {''',
             '''void SegmentTreeEditBase::setFormula(const QString& text) {
    if (m_mathText) m_mathText->setPlainText(text);
}

void SegmentTreeEditBase::onLiveDataChanged(const QString& row, int index, int value) {
    if (row == "a") {
        if (index >= 1 && index < static_cast<int>(m_arr.size())) m_arr[index] = value;
        return;
    }
    if (row == "tr") {''', 'seg edit base live')
write(P, t)

P = os.path.join(ROOT, 'TrieModule.cpp')
t = read(P)
t, _ = sub_once(t, 'm_indexPointer->setAnchorNode(m_charCells[pos]);',
                'm_indexPointer->setAnchorItem(m_charCells[pos]);', 'trie anchor')
write(P, t)

print('done')
