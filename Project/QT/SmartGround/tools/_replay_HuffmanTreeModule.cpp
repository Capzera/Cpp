#include "HuffmanTreeModule.h"
#include <QFont>
#include <QRegularExpression>
#include <algorithm>
#include <functional>
#include "ui/UiFonts.h"

static const QColor kHfLeaf = QColor(206, 226, 250);
static const QColor kHfSorted = QColor(174, 208, 244);
static const QColor kHfPick = QColor(255, 205, 130);
static const QColor kHfMerge = QColor(196, 230, 200);
static const QColor kHfRoot = QColor(255, 214, 168);
static const QColor kHfEdge = QColor(152, 168, 192);
static const QColor kHfEdgeNew = QColor(224, 138, 40);
static const QColor kHfEmpty = QColor(243, 245, 249);
static const QColor kHfIdle = QColor(250, 250, 253);
static const QColor kHfCode = QColor(212, 234, 250);
static const QColor kHfCodeHot = QColor(255, 214, 150);
static const QColor kHfWpl = QColor(255, 226, 175);
static const QColor kHfText = QColor(20, 30, 45);
static const QColor kHfPath = QColor(214, 96, 32);

HuffmanTreeModule::HuffmanTreeModule() {}

QString HuffmanTreeModule::getModuleName() const { return "哈夫曼树"; }

QString HuffmanTreeModule::getDefaultInput() const { return "7\n45 13 20 16 9 12 5"; }

QString HuffmanTreeModule::getInputHint() const {
    return "第一行输入一个正整数 n，表示叶子个数（2 <= n <= 8）\n"
           "第二行输入 n 个非负整数，表示每个叶子的权值\n"
           "先对权值数组升序排序，再每次取两个最小权值合并成父节点，父节点权值 = 两者之和";
}

HuffmanNodeData* HuffmanTreeModule::nodeAt(int id) {
    if (id < 0 || id >= static_cast<int>(m_nodes.size())) return nullptr;
    return &m_nodes[id];
}

const HuffmanNodeData* HuffmanTreeModule::nodeAt(int id) const {
    if (id < 0 || id >= static_cast<int>(m_nodes.size())) return nullptr;
    return &m_nodes[id];
}

bool HuffmanTreeModule::subtreeHas(int nodeId, int leafId) const {
    const HuffmanNodeData* node = nodeAt(nodeId);
    if (!node) return false;
    if (node->original) return node->id == leafId;
    return subtreeHas(node->left, leafId) || subtreeHas(node->right, leafId);
}

void HuffmanTreeModule::countLeaves(int id, int& leaves) const {
    const HuffmanNodeData* node = nodeAt(id);
    if (!node) return;
    if (node->original) { ++leaves; return; }
    countLeaves(node->left, leaves);
    countLeaves(node->right, leaves);
}
