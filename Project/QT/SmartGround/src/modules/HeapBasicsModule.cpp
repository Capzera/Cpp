#include "HeapBasicsModule.h"
#include <QRegularExpression>
#include <QPropertyAnimation>
#include <QFont>
#include <cmath>
HeapBasicsModule::HeapBasicsModule() : m_mathText(nullptr), m_arrayStartX(0), m_treeRootX(0), n(0) {}
QString HeapBasicsModule::getModuleName() const {
    return "堆基础映射";
}
QString HeapBasicsModule::getDefaultInput() const {
    return "5\n10 5 8 2 9";
}
QString HeapBasicsModule::getInputHint() const {
    return "第一行输入一个正整数 n，表示堆中元素的个数\n"
           "第二行输入 n 个整数，即完全二叉树按层序展开后的数组存储\n"
           "约定 heap[1] 为根节点，heap[2i] 为左子，heap[2i+1] 为右子";
}
void HeapBasicsModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    m_arrayBlocks.clear();
    m_treeNodes.clear();
    m_treeEdges.clear();
    m_heap.clear();
    m_frames.clear();
    QStringList items = inputData.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    if (items.isEmpty()) return;
    n = items[0].toInt();
    if (n < 0) n = 0;
    if (n > items.size() - 1) n = items.size() - 1;
    if (n <= 0) return;
    for (int i = 1; i <= n; ++i) m_heap.push_back(items[i].toInt());
    m_arrayStartX = 200 - (n * 60) / 2;
    m_treeRootX = 400;
    m_mathText = createScriptText(m_scene, "");
    m_mathText->setFont(QFont("Consolas", 14, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(180, 50, 50));
    m_mathText->setPos(50, 50);
    QGraphicsTextItem* arrLabel = m_scene->addText("Array View (Index 0~N-1)");
    arrLabel->setFont(QFont("Arial", 11, QFont::Bold));
    arrLabel->setPos(40, 168);
    for (int i = 0; i < n; ++i) {
        ArrayBlock* block = new ArrayBlock(i + 1, m_heap[i], QString::number(i));
        m_scene->addItem(block);
        block->setPos(m_arrayStartX + i * 60, 250);
        m_arrayBlocks[i + 1] = block;
    }
    QGraphicsTextItem* treeLabel = m_scene->addText("Binary Tree Structure");
    treeLabel->setFont(QFont("Arial", 11, QFont::Bold));
    treeLabel->setPos(430, 168);
    std::function<void(int, int, int)> buildTree = [&](int idx, int x, int y) {
        if (idx > n) return;
        GraphNode* node = new GraphNode(m_heap[idx - 1], m_heap[idx - 1]);
        m_scene->addItem(node);
        node->setPos(x, y);
        m_treeNodes[idx] = node;
        int leftChild = idx * 2;
        int rightChild = idx * 2 + 1;
        if (leftChild <= n) {
            buildTree(leftChild, x - 100 / (1 + (int)std::log2(leftChild)), y + 80);
            GraphEdge* edge = new GraphEdge(node, m_treeNodes[leftChild], true);
            m_scene->addItem(edge);
            node->addEdge(edge);
            m_treeNodes[leftChild]->addEdge(edge);
            m_treeEdges[ {idx, leftChild}] = edge;
        }
        if (rightChild <= n) {
            buildTree(rightChild, x + 100 / (1 + (int)std::log2(rightChild)), y + 80);
            GraphEdge* edge = new GraphEdge(node, m_treeNodes[rightChild], true);
            m_scene->addItem(edge);
            node->addEdge(edge);
            m_treeNodes[rightChild]->addEdge(edge);
            m_treeEdges[ {idx, rightChild}] = edge;
        }
    };
    if (n > 0) buildTree(1, m_treeRootX, 280);
}
std::vector<AnimFrame> HeapBasicsModule::runAlgorithm() {
    m_frames.clear();
    m_frames.push_back({ActionType::HP_HIGHLIGHT_RELATION, 1, 0, 0, 0, {}, "演示堆的数组-树映射关系", "heap[1] = 根节点, heap[2i] = 左子, heap[2i+1] = 右子"});
    for (int i = 1; i <= n; ++i) {
        m_frames.push_back({ActionType::HP_HIGHLIGHT_RELATION, i, 0, 0, 0, {}, QString("索引 %1: 值=%2").arg(i).arg(m_heap[i - 1])});
        if (i > 1) {
            int parent = i / 2;
            m_frames.push_back({ActionType::HP_HIGHLIGHT_RELATION, parent, i, 0, 0, {}, QString("父节点关系: heap[%1] -> heap[%2]").arg(parent).arg(i)});
        }
    }
    m_frames.push_back({ActionType::HP_SYNC_ARRAY_TREE, 0, 0, 0, 0, {}, "数组与树结构完全对应"});
    return m_frames;
}
void HeapBasicsModule::renderFrame(const AnimFrame& step) {
    if (!step.mathText.isEmpty()) setScriptText(m_mathText, step.mathText);
    if (step.type == ActionType::HP_HIGHLIGHT_RELATION) {
        if (m_arrayBlocks.count(step.u)) {
            m_arrayBlocks[step.u]->color = QColor(255, 200, 100);
            playScalePulse(m_arrayBlocks[step.u], 1.15, 200);
            playCellRipple(m_arrayBlocks[step.u], QColor(255, 190, 90), 0.9, 460);
            m_arrayBlocks[step.u]->update();
        }
        if (m_treeNodes.count(step.u)) {
            m_treeNodes[step.u]->highlightColor = QColor(255, 200, 100);
            playScalePulse(m_treeNodes[step.u], 1.15, 200);
            playNodeRipple(m_treeNodes[step.u], QColor(255, 190, 90), 34.0, 480);
            m_treeNodes[step.u]->update();
        }
        if (step.v > 0) {
            if (m_arrayBlocks.count(step.v)) {
                m_arrayBlocks[step.v]->color = QColor(100, 200, 255);
                m_arrayBlocks[step.v]->update();
                playNodePop(m_arrayBlocks[step.v], 1.12, 240);
            }
            if (m_treeNodes.count(step.v)) {
                m_treeNodes[step.v]->highlightColor = QColor(100, 200, 255);
                m_treeNodes[step.v]->update();
                playNodePop(m_treeNodes[step.v], 1.12, 240);
            }
            if (m_treeEdges.count({step.u, step.v})) {
                m_treeEdges[ {step.u, step.v}]->color = QColor(100, 200, 255);
                m_treeEdges[ {step.u, step.v}]->update();
                playEdgeTravel(m_treeEdges[ {step.u, step.v}], QColor(100, 200, 255), 460);
            }
        }
    } else if (step.type == ActionType::HP_SYNC_ARRAY_TREE) {
        for (auto& p : m_arrayBlocks) {
            p.second->color = Qt::white;
            p.second->update();
        }
        for (auto& p : m_treeNodes) {
            p.second->highlightColor = Qt::white;
            p.second->update();
        }
        for (auto& p : m_treeEdges) {
            p.second->color = Qt::black;
            p.second->update();
        }
    }
}
void HeapBasicsModule::reset() {
    m_arrayBlocks.clear();
    m_treeNodes.clear();
    m_treeEdges.clear();
    m_heap.clear();
    m_frames.clear();
}
void HeapBasicsModule::releaseSceneItems() {
    m_arrayBlocks.clear();
    m_treeNodes.clear();
    m_treeEdges.clear();
    m_mathText = nullptr;
}
