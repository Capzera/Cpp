#include "SegmentTreeEditBase.h"
#include "ui/UiTheme.h"
#include <QFont>

void SegmentTreeEditBase::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;
    QString error;
    std::vector<SegmentTreeCommon::Operation> ops;
    m_valid = SegmentTreeCommon::parseOps(inputData, m_arr, m_mod, ops, error);
    m_ops = ops;
    buildTreeScene("", error);
}

void SegmentTreeEditBase::buildTreeScene(const QString& title, const QString& subtitle) {
    m_mathText = createScriptText(m_scene, title);
    m_mathText->setFont(QFont("Consolas", 14, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(50, 22);
    if (!m_valid) {
        setScriptText(m_mathText, "输入错误：" + subtitle);
        return;
    }

    m_n = static_cast<int>(m_arr.size()) - 1;
    SegmentTreeCommon::Layout layout = SegmentTreeCommon::computeLayout(m_n, 3);
    m_nodeIds = layout.slotIds;
    m_slotCount = m_nodeIds.empty() ? 0 : m_nodeIds.back();
    m_tr.assign(m_slotCount + 1, 0);
    m_add.assign(m_slotCount + 1, 0);
    m_mul.assign(m_slotCount + 1, 1);

    const double cellW = 58.0;
    const double arrStartX = 150.0;
    for (int i = 1; i <= m_n; ++i) {
        ArrayBlock* block = new ArrayBlock(i, m_arr[i], QString::number(i));
        block->setCellSize(cellW, 40.0);
        block->setPos(arrStartX + (i - 1) * cellW, m_arrY);
        block->color = QColor(255, 250, 230);
        m_scene->addItem(block);
        m_arrays["a"][i] = block;
        registerArraySlot(block, "a", i);
    }
    QGraphicsTextItem* aTitle = m_scene->addText("a");
    aTitle->setFont(QFont("Consolas", 13, QFont::Bold));
    aTitle->setDefaultTextColor(QColor(40, 70, 110));
    aTitle->setPos(arrStartX - 40, m_arrY - 2);

    m_nodes = SegmentTreeCommon::buildTopology(1, 1, m_n, QPointF(430, m_nodeY), 200.0, layout.rowGap);
    for (const auto& info : m_nodes) {
        GraphNode* item = new GraphNode(info.node);
        item->setPos(info.pos);
        item->setSubLabel(QString("[%1,%2]").arg(info.l).arg(info.r));
        item->color = QColor(250, 250, 252);
        m_scene->addItem(item);
        m_treeNodes[info.node] = item;
        m_nodeIntervalL[info.node] = info.l;
        m_nodeIntervalR[info.node] = info.r;
    }
    for (const auto& info : m_nodes) {
        if (info.l == info.r) continue;
        GraphNode* self = m_treeNodes[info.node];
        GraphNode* left = m_treeNodes[SegmentTreeCommon::ls(info.node)];
        GraphNode* right = m_treeNodes[SegmentTreeCommon::rs(info.node)];
        if (!self || !left || !right) continue;
        GraphEdge* e1 = new GraphEdge(self, left, false, false);
        m_scene->addItem(e1);
        self->addEdge(e1);
        left->addEdge(e1);
        m_treeEdges[{info.node, SegmentTreeCommon::ls(info.node)}] = e1;
        GraphEdge* e2 = new GraphEdge(self, right, false, false);
        m_scene->addItem(e2);
        self->addEdge(e2);
        right->addEdge(e2);
        m_treeEdges[{info.node, SegmentTreeCommon::rs(info.node)}] = e2;
    }

    const QStringList rows = QStringList() << "tr" << "add" << "mul";
    double y = layout.firstRowY;
    for (const QString& row : rows) {
        m_rowY[row] = y;
        int slotIndex = 0;
        for (int nodeId : m_nodeIds) {
            ArrayBlock* blk = new ArrayBlock(nodeId, 0, QString::number(nodeId));
            blk->showValue = false;
            blk->setCellSize(m_slotW, 40.0);
            blk->setPos(m_slotStartX + slotIndex * m_slotW, y);
            blk->color = QColor(228, 231, 236);
            m_scene->addItem(blk);
            m_arrays[row][nodeId] = blk;
            registerArraySlot(blk, row, nodeId);
            ++slotIndex;
        }
        QGraphicsTextItem* title = m_scene->addText(row);
        title->setFont(QFont("Consolas", 13, QFont::Bold));
        title->setDefaultTextColor(QColor(40, 70, 110));
        title->setPos(m_slotStartX - 76, y);
        y += layout.rowStep;
    }

    m_ptrNode = new PointerBlock("node", QColor(238, 120, 108));
    m_ptrNode->setOpacity(0);
    m_scene->addItem(m_ptrNode);
}

void SegmentTreeEditBase::resetTreeScene() {
    for (auto& kv : m_treeNodes) {
        kv.second->setHighlightColor(Qt::transparent);
        kv.second->color = QColor(250, 250, 252);
        kv.second->setValueText(QString());
        kv.second->setTag(QString(), QColor());
    }
    for (auto& row : m_arrays) {
        for (auto& kv : row.second) {
            kv.second->showValue = false;
            kv.second->value = 0;
            kv.second->color = QColor(228, 231, 236);
            kv.second->update();
        }
    }
    if (m_ptrNode) m_ptrNode->setOpacity(0);
    clearDescentTrail();
}

void SegmentTreeEditBase::ensureDescentTrail(QColor color) {
    if (m_descentTrail) return;
    m_descentTrail = new PathTrail(color);
    m_descentTrail->setTrailWidth(4.5);
    m_descentTrail->setHeadRadius(7.0);
    m_scene->addItem(m_descentTrail);
    m_descentPoints.clear();
}

void SegmentTreeEditBase::clearDescentTrail() {
    if (!m_descentTrail) return;
    m_descentTrail->clearPoints();
    m_descentTrail->setProgress(0.0);
    m_descentPoints.clear();
}

void SegmentTreeEditBase::appendDescent(int node, QColor color) {
    if (node <= 0) return;
    auto it = m_treeNodes.find(node);
    if (it == m_treeNodes.end()) return;
    ensureDescentTrail(color);
    m_descentPoints.append(it->second->pos());
    if (m_descentPoints.size() >= 2) {
        playPathTrail(m_descentTrail, m_descentPoints, color, 420);
    } else {
        m_descentTrail->setPoints(m_descentPoints);
        m_descentTrail->setColor(color);
        m_descentTrail->setProgress(1.0);
    }
}

void SegmentTreeEditBase::freezeDescentTrail(QColor color) {
    if (!m_descentTrail) return;
    m_descentTrail->setColor(color);
    m_descentTrail->setProgress(1.0);
}

void SegmentTreeEditBase::highlightNode(int node, const QColor& color, const QColor& fill) {
    GraphNode* item = m_treeNodes[node];
    if (!item) return;
    item->setHighlightColor(color);
    item->color = fill;
}

void SegmentTreeEditBase::clearHighlights() {
    for (auto& kv : m_treeNodes) {
        kv.second->setHighlightColor(Qt::transparent);
        kv.second->color = QColor(250, 250, 252);
    }
}

void SegmentTreeEditBase::setNodeValue(int node, const QString& text) {
    GraphNode* item = m_treeNodes[node];
    if (item) item->setValueText(text);
}

void SegmentTreeEditBase::setNodeTag(int node, const QString& text, const QColor& color) {
    GraphNode* item = m_treeNodes[node];
    if (item) item->setTag(text, color);
}

void SegmentTreeEditBase::clearNodeTag(int node) {
    GraphNode* item = m_treeNodes[node];
    if (item) item->setTag(QString(), QColor());
}

void SegmentTreeEditBase::setArrayValue(const QString& row, int slot, int value, const QColor& color) {
    ArrayBlock* blk = arrayBlock(row, slot);
    if (!blk) return;
    blk->value = value;
    blk->showValue = true;
    blk->color = color;
    blk->update();
}

void SegmentTreeEditBase::showArrayValue(const QString& row, int slot, bool show) {
    ArrayBlock* blk = arrayBlock(row, slot);
    if (!blk) return;
    blk->showValue = show;
    blk->update();
}

void SegmentTreeEditBase::setArrayColor(const QString& row, int slot, const QColor& color) {
    ArrayBlock* blk = arrayBlock(row, slot);
    if (!blk) return;
    blk->color = color;
    blk->update();
}

ArrayBlock* SegmentTreeEditBase::arrayBlock(const QString& row, int slot) {
    auto it = m_arrays.find(row);
    if (it == m_arrays.end()) return nullptr;
    auto sit = it->second.find(slot);
    if (sit == it->second.end()) return nullptr;
    return sit->second;
}

QPointF SegmentTreeEditBase::nodePos(int node) const {
    auto it = m_treeNodes.find(node);
    if (it == m_treeNodes.end()) return QPointF();
    return it->second->pos();
}

GraphNode* SegmentTreeEditBase::nodeItem(int node) const {
    auto it = m_treeNodes.find(node);
    if (it == m_treeNodes.end()) return nullptr;
    return it->second;
}

void SegmentTreeEditBase::movePointerTo(int node) {
    if (!m_ptrNode) return;
    GraphNode* item = nodeItem(node);
    if (!item) return;
    m_ptrNode->setOpacity(1);
    m_ptrNode->setAnchorNode(item, false);
    m_ptrNode->setAnchorOffset(QPointF(0, 46));
    playPointerMove(m_ptrNode, item->pos() + QPointF(0, 46));
}

void SegmentTreeEditBase::setFormula(const QString& text) {
    if (m_mathText) setScriptText(m_mathText, text);
}

void SegmentTreeEditBase::onLiveDataChanged(const QString& row, int index, int value) {
    if (row == "a") {
        if (index >= 1 && index < static_cast<int>(m_arr.size())) m_arr[index] = value;
        return;
    }
    if (row == "tr") {
        if (index >= 0 && index < static_cast<int>(m_tr.size())) m_tr[index] = value;
        return;
    }
    if (row == "add") {
        if (index >= 0 && index < static_cast<int>(m_add.size())) m_add[index] = value;
        return;
    }
    if (row == "mul") {
        if (index >= 0 && index < static_cast<int>(m_mul.size())) m_mul[index] = value;
    }
}

void SegmentTreeEditBase::reset() {
    m_frames.clear();
    m_treeNodes.clear();
    m_arrays.clear();
    m_nodeIntervalL.clear();
    m_nodeIntervalR.clear();
    m_nodes.clear();
    m_ptrNode = nullptr;
    m_mathText = nullptr;
    m_descentTrail = nullptr;
    m_descentPoints.clear();
    m_arr.clear();
    m_tr.clear();
    m_add.clear();
    m_mul.clear();
    m_nodeIds.clear();
    m_n = 0;
    m_mod = 100;
    m_slotCount = 0;
    m_valid = false;
    m_rowY.clear();
}

void SegmentTreeEditBase::releaseSceneItems() {
    m_treeNodes.clear();
    m_treeEdges.clear();
    m_arrays.clear();
    m_nodeIntervalL.clear();
    m_nodeIntervalR.clear();
    m_nodes.clear();
    m_descentTrail = nullptr;
    m_descentPoints.clear();
    m_ptrNode = nullptr;
    m_mathText = nullptr;
}
