#include "SegmentTreeBuildModule.h"
#include "ui/UiTheme.h"
#include <QFont>
#include <QRegularExpression>

static const QColor kSegRed = QColor(238, 120, 108);
static const QColor kSegOrange = QColor(250, 196, 110);
static const QColor kSegGreen = QColor(140, 205, 150);
static const QColor kSegBlue = QColor(150, 185, 235);
static const QColor kSegGray = QColor(228, 231, 236);
static const QColor kSegPale = QColor(250, 250, 252);

SegmentTreeBuildModule::SegmentTreeBuildModule() {}

QString SegmentTreeBuildModule::getModuleName() const { return "线段树 - 建树"; }
QString SegmentTreeBuildModule::getDefaultInput() const { return "4\n1 2 3 4"; }
QString SegmentTreeBuildModule::getInputHint() const {
    return "第一行输入 n：元素个数 n\n"
           "接下来一行输入 n 个整数，即原数组 a[1..n]\n"
           "本模块演示 build(1, 1, n, a)：自顶向下递归建树，自底向上 push_up 合并区间和\n"
           "约定 tr[] 存区间和，lz[] 存区间加法懒标记（建树阶段全为 0）";
}

void SegmentTreeBuildModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;
    QString error;
    m_valid = SegmentTreeCommon::parseInput(inputData, m_arr, error);
    m_mathText = createScriptText(scene, m_valid ? "build(1, 1, n, a)：自顶向下递归建树，自底向上 push_up 合并"
                                        : ("输入错误：" + error));
    m_mathText->setFont(QFont("Consolas", 14, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(50, 22);
    if (!m_valid) return;

    m_n = static_cast<int>(m_arr.size()) - 1;
    SegmentTreeCommon::Layout layout = SegmentTreeCommon::computeLayout(m_n, 2);
    m_nodeIds = layout.slotIds;
    m_slotCount = m_nodeIds.empty() ? 0 : m_nodeIds.back();
    m_trY = layout.firstRowY;
    m_lzY = layout.firstRowY + layout.rowStep;    m_tr.assign(m_slotCount + 1, 0);
    m_lz.assign(m_slotCount + 1, 0);

    const double cellW = 58.0;
    const double arrStartX = 150.0;
    for (int i = 1; i <= m_n; ++i) {
        ArrayBlock* block = new ArrayBlock(i, m_arr[i], QString::number(i));
        block->setCellSize(cellW, 40.0);
        block->setPos(arrStartX + (i - 1) * cellW, m_arrY);
        block->color = QColor(255, 250, 230);
        scene->addItem(block);
        m_aBlocks.push_back(block);
        registerArraySlot(block, "a", i);
    }
    QGraphicsTextItem* aTitle = scene->addText("a");
    aTitle->setFont(QFont("Consolas", 13, QFont::Bold));
    aTitle->setDefaultTextColor(QColor(40, 70, 110));
    aTitle->setPos(arrStartX - 40, m_arrY - 2);

    m_nodes = SegmentTreeCommon::buildTopology(1, 1, m_n, QPointF(430, m_nodeY), 200.0, layout.rowGap);
    for (const auto& info : m_nodes) {
        GraphNode* item = new GraphNode(info.node);
        item->setPos(info.pos);
        item->setSubLabel(QString("[%1,%2]").arg(info.l).arg(info.r));
        item->color = kSegPale;
        scene->addItem(item);
        m_treeNodes[info.node] = item;
    }
    for (const auto& info : m_nodes) {
        if (info.l == info.r) continue;
        GraphNode* self = m_treeNodes[info.node];
        GraphNode* left = m_treeNodes[SegmentTreeCommon::ls(info.node)];
        GraphNode* right = m_treeNodes[SegmentTreeCommon::rs(info.node)];
        if (!self || !left || !right) continue;
        GraphEdge* e1 = new GraphEdge(self, left, false, false);
        scene->addItem(e1);
        self->addEdge(e1);
        left->addEdge(e1);
        m_treeEdges[{info.node, SegmentTreeCommon::ls(info.node)}] = e1;
        GraphEdge* e2 = new GraphEdge(self, right, false, false);
        scene->addItem(e2);
        self->addEdge(e2);
        right->addEdge(e2);
        m_treeEdges[{info.node, SegmentTreeCommon::rs(info.node)}] = e2;
    }

    const double slotW = layout.slotW;
    const double slotStartX = layout.slotStartX;
    int slotIndex = 0;
    for (int nodeId : m_nodeIds) {
        ArrayBlock* tr = new ArrayBlock(nodeId, 0, QString::number(nodeId));
        tr->showValue = false;
        tr->setCellSize(slotW, 40.0);
        tr->setPos(slotStartX + slotIndex * slotW, m_trY);
        tr->color = kSegGray;
        scene->addItem(tr);
        m_trBlocks[nodeId] = tr;
        registerArraySlot(tr, "tr", nodeId);
        ArrayBlock* lz = new ArrayBlock(nodeId, 0, QString::number(nodeId));
        lz->showValue = false;
        lz->setCellSize(slotW, 40.0);
        lz->setPos(slotStartX + slotIndex * slotW, m_lzY);
        lz->color = kSegGray;
        scene->addItem(lz);
        m_lzBlocks[nodeId] = lz;
        registerArraySlot(lz, "lz", nodeId);
        ++slotIndex;
    }
    QGraphicsTextItem* trTitle = scene->addText("tr");
    trTitle->setFont(QFont("Consolas", 13, QFont::Bold));
    trTitle->setDefaultTextColor(QColor(40, 70, 110));
    trTitle->setPos(slotStartX - 42, m_trY);
    QGraphicsTextItem* lzTitle = scene->addText("lz");
    lzTitle->setFont(QFont("Consolas", 13, QFont::Bold));
    lzTitle->setDefaultTextColor(QColor(40, 70, 110));
    lzTitle->setPos(slotStartX - 42, m_lzY);

    m_ptrNode = new PointerBlock("node", kSegRed);
    m_ptrNode->setOpacity(0);
    scene->addItem(m_ptrNode);
}

GraphNode* SegmentTreeBuildModule::treeNode(int nodeId) const {
    auto it = m_treeNodes.find(nodeId);
    if (it == m_treeNodes.end()) return nullptr;
    return it->second;
}

ArrayBlock* SegmentTreeBuildModule::lzBlock(int nodeId) const {
    auto it = m_lzBlocks.find(nodeId);
    if (it == m_lzBlocks.end()) return nullptr;
    return it->second;
}

ArrayBlock* SegmentTreeBuildModule::trBlock(int nodeId) const {
    auto it = m_trBlocks.find(nodeId);
    if (it == m_trBlocks.end()) return nullptr;
    return it->second;
}

void SegmentTreeBuildModule::movePointerTo(int node) {
    if (!m_ptrNode) return;
    GraphNode* item = treeNode(node);
    if (!item) return;
    m_ptrNode->setOpacity(1);
    m_ptrNode->setAnchorNode(item, false);
    m_ptrNode->setAnchorOffset(QPointF(0, 46));
    playPointerMove(m_ptrNode, item->pos() + QPointF(0, 46));
}

void SegmentTreeBuildModule::ensureDescentTrail(QColor color) {
    if (m_descentTrail) return;
    m_descentTrail = new PathTrail(color);
    m_descentTrail->setTrailWidth(4.5);
    m_descentTrail->setHeadRadius(7.0);
    m_scene->addItem(m_descentTrail);
    m_descentPoints.clear();
}

void SegmentTreeBuildModule::clearDescentTrail() {
    if (!m_descentTrail) return;
    m_descentTrail->clearPoints();
    m_descentTrail->setProgress(0.0);
    m_descentPoints.clear();
}

void SegmentTreeBuildModule::appendDescent(int node, QColor color) {
    if (node <= 0) return;
    GraphNode* item = treeNode(node);
    if (!item) return;
    ensureDescentTrail(color);
    m_descentPoints.append(item->pos());
    if (m_descentPoints.size() >= 2) {
        playPathTrail(m_descentTrail, m_descentPoints, color, 420);
    } else {
        m_descentTrail->setPoints(m_descentPoints);
        m_descentTrail->setColor(color);
        m_descentTrail->setProgress(1.0);
    }
}

void SegmentTreeBuildModule::freezeDescentTrail(QColor color) {
    if (!m_descentTrail) return;
    m_descentTrail->setColor(color);
    m_descentTrail->setProgress(1.0);
}

std::vector<AnimFrame> SegmentTreeBuildModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;
    for (int i = 1; i <= m_n; ++i) m_arr[i] = liveValue("a", i, m_arr[i]);
    m_tr.assign(m_slotCount + 1, 0);
    m_lz.assign(m_slotCount + 1, 0);
    m_frames.push_back({ActionType::SEG_RESET, 0, 0, 0, 0, {}, "准备建树，所有节点尚未赋值", "build(1, 1, n, a)"});

    struct Task { int node, l, r; };
    std::vector<Task> stack;
    stack.push_back({1, 1, m_n});
    std::vector<Task> visitOrder;
    while (!stack.empty()) {
        Task t = stack.back();
        stack.pop_back();
        visitOrder.push_back(t);
        m_lz[t.node] = 0;
        m_frames.push_back({ActionType::SEG_INIT_LZ, t.node, 0, 0, 0, {},
                            QString("进入 build(node=%1, l=%2, r=%3)，本节点暂无懒标记").arg(t.node).arg(t.l).arg(t.r),
                            QString("lz[%1] = 0").arg(t.node)});
        if (t.l == t.r) {
            m_tr[t.node] = m_arr[t.l];
            m_frames.push_back({ActionType::SEG_LEAF, t.node, t.l, static_cast<int>(m_tr[t.node]), 0, {},
                                QString("到达叶子 node=%1，取 a[%2] 的值").arg(t.node).arg(t.l),
                                QString("tr[%1] = a[%2] = %3").arg(t.node).arg(t.l).arg(m_tr[t.node])});
            continue;
        }
        int mid = (t.l + t.r) >> 1;
        m_frames.push_back({ActionType::SEG_DESCEND, t.node, mid, SegmentTreeCommon::ls(t.node), SegmentTreeCommon::rs(t.node), {},
                            QString("node=%1 的区间 [%2,%3] 被中点 %4 一分为二").arg(t.node).arg(t.l).arg(t.r).arg(mid),
                            QString("mid = (%1+%2)>>1 = %3　ls = %1<<1 = %4　rs = %1<<1|1 = %5")
                                .arg(t.node).arg(t.r).arg(mid).arg(SegmentTreeCommon::ls(t.node)).arg(SegmentTreeCommon::rs(t.node))});
        stack.push_back({SegmentTreeCommon::rs(t.node), mid + 1, t.r});
        stack.push_back({SegmentTreeCommon::ls(t.node), t.l, mid});
    }

    for (int i = static_cast<int>(visitOrder.size()) - 1; i >= 0; --i) {
        int node = visitOrder[i].node;
        if (visitOrder[i].l == visitOrder[i].r) continue;
        int lc = SegmentTreeCommon::ls(node);
        int rc = SegmentTreeCommon::rs(node);
        if (lc > m_slotCount || rc > m_slotCount) continue;
        m_tr[node] = m_tr[lc] + m_tr[rc];
        m_frames.push_back({ActionType::SEG_PUSHUP, node, lc, static_cast<int>(m_tr[lc]), static_cast<int>(m_tr[rc]), {lc, rc},
                            QString("node=%1 汇总两个孩子的区间和").arg(node),
                            QString("tr[%1] = tr[%2] + tr[%3] = %4").arg(node).arg(lc).arg(rc).arg(m_tr[node])});
    }

    m_frames.push_back({ActionType::SEG_DONE, 1, 0, 0, 0, {}, "建树完成，根节点保存整个数组的和", "build 完成"});
    return m_frames;
}

void SegmentTreeBuildModule::renderFrame(const AnimFrame& frame) {
    if (!m_mathText) return;
    setScriptText(m_mathText, frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    if (frame.type == ActionType::SEG_RESET) {
        for (auto& kv : m_treeNodes) {
            kv.second->setHighlightColor(Qt::transparent);
            kv.second->color = kSegPale;
            kv.second->setValueText(QString());
            kv.second->update();
        }
        for (auto& kv : m_trBlocks) {
            kv.second->showValue = false;
            kv.second->value = 0;
            kv.second->color = kSegGray;
            kv.second->update();
        }
        for (auto& kv : m_lzBlocks) {
            kv.second->showValue = false;
            kv.second->value = 0;
            kv.second->color = kSegGray;
            kv.second->update();
        }
        if (m_ptrNode) m_ptrNode->setOpacity(0);
    } else if (frame.type == ActionType::SEG_INIT_LZ) {
        ArrayBlock* blk = lzBlock(frame.u);
        if (blk) {
            blk->value = 0;
            blk->showValue = true;
            blk->color = kSegBlue;
            blk->update();
        }
        GraphNode* node = treeNode(frame.u);
        if (node) {
            node->setHighlightColor(kSegRed);
            node->color = QColor(255, 240, 238);
            movePointerTo(frame.u);
        }
    } else if (frame.type == ActionType::SEG_DESCEND) {
        GraphNode* node = treeNode(frame.u);
        GraphNode* left = treeNode(frame.val1);
        GraphNode* right = treeNode(frame.val2);
        if (node) {
            node->setHighlightColor(kSegOrange);
            node->color = QColor(255, 248, 232);
        }
        auto edgeIt = m_treeEdges.find({frame.u, frame.val1});
        if (edgeIt != m_treeEdges.end()) playEdgeTravel(edgeIt->second, kSegOrange, 420);
        appendDescent(frame.u, kSegOrange);
        appendDescent(frame.val1, kSegOrange);
        if (left) {
            left->setHighlightColor(kSegOrange);
            left->color = QColor(255, 248, 232);
            left->update();
            playNodeRipple(left, kSegOrange, 34.0, 480);
        }
        if (right) {
            right->color = kSegPale;
            right->setHighlightColor(Qt::transparent);
            right->update();
        }
        movePointerTo(frame.u);
    } else if (frame.type == ActionType::SEG_LEAF) {
        ArrayBlock* blk = trBlock(frame.u);
        GraphNode* node = treeNode(frame.u);
        if (blk && node) {
            blk->value = frame.val1;
            blk->showValue = true;
            blk->color = kSegGreen;
            blk->update();
            node->setValueText(QString("tr=%1").arg(frame.val1));
            node->color = QColor(240, 252, 242);
            node->setHighlightColor(kSegGreen);
            playCellLanding(blk, kSegGreen, 420);
            playNodeRipple(node, kSegGreen, 42.0, 560);
        }
        movePointerTo(frame.u);
    } else if (frame.type == ActionType::SEG_PUSHUP) {
        ArrayBlock* blk = trBlock(frame.u);
        GraphNode* node = treeNode(frame.u);
        int rc = frame.group.size() > 1 ? frame.group[1] : 0;
        ArrayBlock* lb = trBlock(frame.v);
        ArrayBlock* rb = trBlock(rc);
        if (blk && node) {
            QPointF sa = lb ? lb->pos() + QPointF(30, 0) : blk->pos();
            QPointF sb = rb ? rb->pos() + QPointF(30, 0) : blk->pos();
            playDualValueFlight(sa, sb, blk, frame.val1, frame.val2, kSegGreen);
            int sum = frame.val1 + frame.val2;
            node->setValueText(QString("tr=%1").arg(sum));
            node->color = QColor(240, 252, 242);
            node->setHighlightColor(kSegGreen);
            playNodeRipple(node, kSegGreen, 46.0, 600);
        }
        movePointerTo(frame.u);
    } else if (frame.type == ActionType::SEG_DONE) {
        for (auto& kv : m_treeNodes) {
            kv.second->setHighlightColor(Qt::transparent);
            kv.second->color = QColor(240, 252, 242);
            playNodePop(kv.second, 1.10, 260);
        }
        for (auto& kv : m_trBlocks) {
            if (kv.second->showValue) {
                kv.second->color = kSegGreen;
                kv.second->update();
            }
        }
        if (m_ptrNode) m_ptrNode->setOpacity(0);
    }
}

void SegmentTreeBuildModule::onLiveDataChanged(const QString& row, int index, int value) {
    if (row == "a") {
        if (index >= 1 && index < static_cast<int>(m_arr.size())) m_arr[index] = value;
        return;
    }
    if (row == "tr") {
        if (index >= 0 && index < static_cast<int>(m_tr.size())) m_tr[index] = value;
        return;
    }
    if (row == "lz") {
        if (index >= 0 && index < static_cast<int>(m_lz.size())) m_lz[index] = value;
    }
}

void SegmentTreeBuildModule::reset() {
    m_frames.clear();
    m_treeNodes.clear();
    m_trBlocks.clear();
    m_lzBlocks.clear();
    m_aCells.clear();
    m_aBlocks.clear();
    m_ptrNode = nullptr;
    m_mathText = nullptr;
    m_arr.clear();
    m_tr.clear();
    m_lz.clear();
    m_nodes.clear();
    m_nodeIds.clear();
    m_n = 0;
    m_slotCount = 0;
    m_valid = false;
}

void SegmentTreeBuildModule::releaseSceneItems() {
    m_treeNodes.clear();
    m_trBlocks.clear();
    m_lzBlocks.clear();
    m_aCells.clear();
    m_aBlocks.clear();
    m_descentTrail = nullptr;
    m_descentPoints.clear();
    m_ptrNode = nullptr;
    m_mathText = nullptr;
}
