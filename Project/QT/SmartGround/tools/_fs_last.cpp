#include "ForwardStarModule.h"
#include "GraphRenderHelper.h"
#include <QFont>
#include <algorithm>

static const QColor kFsPale = QColor(252, 252, 254);
static const QColor kFsEdge = QColor(70, 165, 195);
static const QColor kFsNext = QColor(160, 110, 220);
static const QColor kFsCur = QColor(246, 176, 120);
static const QColor kFsHead = QColor(255, 220, 160);

static const double kFsCellW = 58.0;
static const double kFsCellH = 36.0;
static const double kFsHeadX = 40.0;
static const double kFsToX = 190.0;
static const double kFsNxtX = 340.0;
static const double kFsWeightX = 490.0;
static const double kFsTopY = 96.0;
static const double kFsRowGap = 42.0;

ForwardStarModule::ForwardStarModule() {}

QString ForwardStarModule::getModuleName() const { return "链式前向星"; }
QString ForwardStarModule::getDefaultInput() const { return "5 6 1\n1 2 3\n1 3 5\n2 4 2\n3 4 1\n4 5 4\n2 5 6"; }

QString ForwardStarModule::getInputHint() const {
    return "第一行输入三个正整数：顶点数 n、边数 m、源点 src\n"
           "接下来 m 行，每行三个整数 u, v, w，表示一条权值为 w 的边\n"
           "add(u, v, w) 会把新边插到 u 的链表头部：to[++tot]=v，nxt[tot]=head[u]，head[u]=tot";
}

std::map<int, QPointF> ForwardStarModule::captureNodePositions() const {
    return GraphRenderHelper::captureNodePositions(m_nodes);
}

void ForwardStarModule::restoreNodePositions(const std::map<int, QPointF>& positions) {
    GraphRenderHelper::restoreNodePositions(m_nodes, positions);
}

void ForwardStarModule::layoutArrays(int nodeCount, int edgeCapacity) {
    auto addColumn = [&](const QString& title, double x, double y) {
        QGraphicsTextItem* label = m_scene->addText(title);
        label->setFont(QFont("Consolas", 12, QFont::Bold));
        label->setDefaultTextColor(QColor(40, 70, 110));
        label->setPos(x - 12, y - 28);
    };

    addColumn("head[u]", kFsHeadX, kFsTopY);
    addColumn("to[k]", kFsToX, kFsTopY);
    addColumn("nxt[k]", kFsNxtX, kFsTopY);
    addColumn("weight[k]", kFsWeightX, kFsTopY);

    for (int u = 1; u <= nodeCount; ++u) {
        ArrayBlock* block = new ArrayBlock(u, 0, QString::number(u));
        block->setCellSize(kFsCellW, kFsCellH);
        block->setShowLabel(false);
        block->setPos(kFsHeadX, kFsTopY + (u - 1) * kFsRowGap);
        block->color = kFsPale;
        m_scene->addItem(block);
        m_headBlocks[u] = block;
    }
    for (int k = 1; k <= edgeCapacity; ++k) {
        ArrayBlock* to = new ArrayBlock(k, 0, QString::number(k));
        to->setCellSize(kFsCellW, kFsCellH);
        to->setShowLabel(false);
        to->setPos(kFsToX, kFsTopY + (k - 1) * kFsRowGap);
        to->color = kFsPale;
        m_scene->addItem(to);
        m_toBlocks[k] = to;

        ArrayBlock* nxt = new ArrayBlock(k, 0, QString::number(k));
        nxt->setCellSize(kFsCellW, kFsCellH);
        nxt->setShowLabel(false);
        nxt->setPos(kFsNxtX, kFsTopY + (k - 1) * kFsRowGap);
        nxt->color = kFsPale;
        m_scene->addItem(nxt);
        m_nxtBlocks[k] = nxt;

        ArrayBlock* weight = new ArrayBlock(k, 0, QString::number(k));
        weight->setCellSize(kFsCellW, kFsCellH);
        weight->setShowLabel(false);
        weight->setPos(kFsWeightX, kFsTopY + (k - 1) * kFsRowGap);
        weight->color = kFsPale;
        m_scene->addItem(weight);
        m_weightBlocks[k] = weight;
    }
}

void ForwardStarModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;

    GraphModuleUtils::GraphInput input;
    m_valid = GraphModuleUtils::parse(inputData, true, m_options, input);
    m_mathText = scene->addText(m_valid ? "链式前向星：head / to / nxt / weight 四列静态数组还原有向图" : "输入格式：n m src，随后输入 m 条边 u v w");
    m_mathText->setFont(QFont("Consolas", 14, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(40, 18);
    if (!m_valid) return;

    m_n = input.n;
    m_m = input.m;
    m_src = input.src;
    m_capacity = std::max(1, m_m * (m_options.directed ? 1 : 2) + 1);

    m_edges = input.edges;
    layoutArrays(m_n, m_capacity);

    GraphRenderHelper::buildNodes(scene, m_nodes, m_n, QPointF(880, 300), m_n <= 8 ? 130 : 150, this);

    m_ptrK = new PointerBlock("k", QColor(210, 80, 55));
    m_ptrK->setDirection(PointerBlock::Right);
    m_ptrK->setOpacity(0);
    scene->addItem(m_ptrK);
    m_ptrHead = new PointerBlock("head[u]", QColor(45, 130, 210));
    m_ptrHead->setOpacity(0);
    scene->addItem(m_ptrHead);
}

std::vector<AnimFrame> ForwardStarModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;

    m_frames.push_back({ActionType::FS_RESET, 0, 0, 0, 0, {}, "初始化：tot = 0，head[] 全为 0", "head[1..n] = 0"});

    int tot = 0;
    std::vector<int> head(m_n + 1, 0);
    std::vector<int> to(m_capacity + 1, 0);
    std::vector<int> nxt(m_capacity + 1, 0);
    std::vector<int> weight(m_capacity + 1, 0);

    auto addEdge = [&](int u, int v, int w, int kind) {
        ++tot;
        to[tot] = v;
        weight[tot] = w;
        nxt[tot] = head[u];
        head[u] = tot;
        m_frames.push_back({ActionType::FS_ADD, tot, u, v, w, {kind},
                            QString("add(%1, %2, %3)：插入第 %4 条边").arg(u).arg(v).arg(w).arg(tot),
                            QString("to[%1] = %2　weight[%1] = %3").arg(tot).arg(v).arg(w)});
        m_frames.push_back({ActionType::FS_LINK, tot, v, v, w, {kind},
                            QString("to[%1] = %2，连向结点 %2").arg(tot).arg(v),
                            QString("to[%1] = %2").arg(tot).arg(v)});
        m_frames.push_back({ActionType::FS_HEAD, tot, u, 0, 0, {kind},
                            QString("nxt[%1] = head[%2]，head[%2] = %1（头插）").arg(tot).arg(u),
                            QString("nxt[%1] = head[%2]　head[%2] = %1").arg(tot).arg(u)});
    };

    for (size_t ei = 0; ei < m_edges.size(); ++ei) {
        const auto& edge = m_edges[ei];
        int w = liveValue("w", static_cast<int>(ei), edge.w);
        addEdge(edge.u, edge.v, w, 0);
        if (!m_options.directed) addEdge(edge.v, edge.u, w, 1);
    }

    m_frames.push_back({ActionType::FS_DONE, tot, 0, 0, 0, {}, QString("共插入 %1 条边").arg(tot), QString("tot = %1").arg(tot)});

    m_frames.push_back({ActionType::FS_TRAVERSE, m_src, 0, 0, 0, {}, QString("从结点 %1 出发遍历邻接表").arg(m_src), QString("for (i = head[%1]; i; i = nxt[i])").arg(m_src)});
    for (int u = 1; u <= m_n; ++u) {
        int i = head[u];
        if (i == 0) continue;
        m_frames.push_back({ActionType::FS_TRAVERSE, u, i, to[i], 0, {},
                            QString("head[%1] = %2，访问到结点 %3").arg(u).arg(i).arg(to[i]),
                            QString("i = head[%1] = %2　to[%2] = %3").arg(u).arg(i).arg(to[i])});
        int guard = 0;
        while (i != 0 && guard++ < 64) {
            i = nxt[i];
            if (i == 0) {
                m_frames.push_back({ActionType::FS_TRAVERSE, u, 0, 0, 0, {},
                                    QString("nxt 链走到 0，%1 的邻接表结束").arg(u),
                                    QString("i = 0，遍历结束")});
                break;
            }
            m_frames.push_back({ActionType::FS_TRAVERSE, u, i, to[i], 0, {},
                                QString("沿 nxt 跳到 %1，访问到结点 %2").arg(i).arg(to[i]),
                                QString("i = nxt[%1] = %2　to[%2] = %3").arg(i).arg(i).arg(to[i])});
        }
    }
    return m_frames;
}

void ForwardStarModule::highlightCell(ArrayBlock* block, const QColor& color) {
    if (!block) return;
    block->color = color;
    block->update();
    if (m_animated) playScalePulse(block, 1.18, 240);
}

void ForwardStarModule::renderFrame(const AnimFrame& frame) {
    if (m_mathText) {
        m_mathText->setPlainText(frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    }
    if (!m_valid) return;

    auto resetArrays = [&]() {
        for (auto& kv : m_headBlocks) { kv.second->color = kFsPale; kv.second->update(); }
        for (auto& kv : m_toBlocks) { kv.second->color = kFsPale; kv.second->update(); }
        for (auto& kv : m_nxtBlocks) { kv.second->color = kFsPale; kv.second->update(); }
        for (auto& kv : m_weightBlocks) { kv.second->color = kFsPale; kv.second->update(); }
    };

    auto movePtrK = [&](int k) {
        if (!m_ptrK) return;
        m_ptrK->setOpacity(1);
        QPointF target(kFsToX - 12.0, kFsTopY + (k - 1) * kFsRowGap + kFsCellH / 2.0);
        if (m_animated && m_ptrK->opacity() > 0.05) {
            playPointerMove(m_ptrK, target, 280, QEasingCurve::InOutCubic);
        } else {
            m_ptrK->setPos(target);
        }
    };

    if (frame.type == ActionType::FS_RESET) {
        resetArrays();
        for (auto& kv : m_toLines) { if (kv.second) { m_scene->removeItem(kv.second); delete kv.second; } }
        m_toLines.clear();
        for (auto& kv : m_nxtLines) { if (kv.second) { m_scene->removeItem(kv.second); delete kv.second; } }
        m_nxtLines.clear();
        for (auto& kv : m_nodes) { kv.second->color = Qt::white; kv.second->update(); }
        if (m_ptrK) m_ptrK->setOpacity(0);
        if (m_ptrHead) m_ptrHead->setOpacity(0);
    } else if (frame.type == ActionType::FS_ADD) {
        int k = frame.u;
        if (k > 0) {
            if (m_toBlocks.count(k)) {
                m_toBlocks[k]->value = frame.val1;
                m_toBlocks[k]->customValue.clear();
                m_toBlocks[k]->showValue = true;
                highlightCell(m_toBlocks[k], kFsCur);
            }
            if (m_weightBlocks.count(k)) {
                m_weightBlocks[k]->value = frame.val2;
                m_weightBlocks[k]->customValue.clear();
                m_weightBlocks[k]->showValue = true;
                highlightCell(m_weightBlocks[k], kFsCur);
            }
            movePtrK(k);
        }
    } else if (frame.type == ActionType::FS_LINK) {
        int k = frame.u;
        int v = frame.v;
        if (k > 0 && m_toBlocks.count(k) && m_nodes.count(v)) {
            ArrayBlock* toBlock = m_toBlocks[k];
            GraphNode* target = m_nodes[v];
            LinkLine* line = new LinkLine(toBlock->pos() + QPointF(toBlock->cellWidth(), toBlock->cellHeight() / 2.0),
                                          target->pos());
            line->setStyle(kFsEdge, 2.2, false, true);
            line->setBend(18.0);
            m_scene->addItem(line);
            m_toLines[k] = line;
            target->color = QColor(190, 232, 240);
            target->update();
        }
    } else if (frame.type == ActionType::FS_HEAD) {
        int k = frame.u;
        int u = frame.v;
        if (k > 0 && m_headBlocks.count(u) && m_nxtBlocks.count(k)) {
            m_headBlocks[u]->value = k;
            m_headBlocks[u]->customValue.clear();
            m_headBlocks[u]->showValue = true;
            highlightCell(m_headBlocks[u], kFsHead);
            m_nxtBlocks[k]->value = m_headBlocks[u]->value;
            m_nxtBlocks[k]->customValue.clear();
            m_nxtBlocks[k]->showValue = true;
            highlightCell(m_nxtBlocks[k], kFsNext);
            if (m_ptrHead) {
                m_ptrHead->setOpacity(1);
                m_ptrHead->setPos(kFsHeadX + kFsCellW / 2.0, kFsTopY + (u - 1) * kFsRowGap + kFsCellH / 2.0 - 34.0);
            }
            LinkLine* line = new LinkLine(m_nxtBlocks[k]->pos() + QPointF(0, m_nxtBlocks[k]->cellHeight() / 2.0),
                                          m_headBlocks[u]->pos() + QPointF(m_headBlocks[u]->cellWidth(), m_headBlocks[u]->cellHeight() / 2.0));
            line->setStyle(kFsNext, 1.8, true, true);
            line->setBend(-14.0);
            m_scene->addItem(line);
            m_nxtLines[k] = line;
        }
    } else if (frame.type == ActionType::FS_TRAVERSE) {
        int u = frame.u;
        int k = frame.v;
        for (auto& kv : m_headBlocks) {
            kv.second->color = kFsPale;
            kv.second->update();
        }
        for (auto& kv : m_toBlocks) { kv.second->color = kFsPale; kv.second->update(); }
        if (u > 0 && m_headBlocks.count(u)) {
            highlightCell(m_headBlocks[u], kFsHead);
        }
        if (k > 0) {
            if (m_toBlocks.count(k)) {
                highlightCell(m_toBlocks[k], kFsCur);
            }
            movePtrK(k);
            auto it = m_toLines.find(k);
            if (it != m_toLines.end() && it->second) {
                it->second->setStyle(kFsCur, 3.0, false, true);
            }
        }
    } else if (frame.type == ActionType::FS_DONE) {
        resetArrays();
        if (m_ptrK) m_ptrK->setOpacity(0);
        if (m_ptrHead) m_ptrHead->setOpacity(0);
    }
}

void ForwardStarModule::onLiveDataChanged(const QString& row, int index, int value) {
    Q_UNUSED(row);
    Q_UNUSED(index);
    Q_UNUSED(value);
}

void ForwardStarModule::reset() {
    m_frames.clear();
    m_nodes.clear();
    m_headBlocks.clear();
    m_toBlocks.clear();
    m_nxtBlocks.clear();
    m_weightBlocks.clear();
    m_toLines.clear();
    m_nxtLines.clear();
    m_edges.clear();
    m_mathText = nullptr;
    m_ptrK = nullptr;
    m_ptrHead = nullptr;
    m_n = 0;
    m_m = 0;
    m_src = 1;
    m_capacity = 0;
    m_valid = false;
}

void ForwardStarModule::releaseSceneItems() {
    for (auto& kv : m_toLines) { if (kv.second) { m_scene->removeItem(kv.second); delete kv.second; } }
    m_toLines.clear();
    for (auto& kv : m_nxtLines) { if (kv.second) { m_scene->removeItem(kv.second); delete kv.second; } }
    m_nxtLines.clear();
    m_nodes.clear();
    m_headBlocks.clear();
    m_toBlocks.clear();
    m_nxtBlocks.clear();
    m_weightBlocks.clear();
    m_mathText = nullptr;
    m_ptrK = nullptr;
    m_ptrHead = nullptr;
}
