#include "BipartiteModule.h"
#include "GraphModuleUtils.h"
#include "GraphRenderHelper.h"
#include <QFont>
#include <algorithm>
#include "core/PointerGeometry.h"
#include "ui/UiFonts.h"

static const QColor kBpBlue = QColor(120, 175, 235);
static const QColor kBpOrange = QColor(245, 195, 145);
static const QColor kBpIdle = QColor(252, 252, 254);
static const QColor kBpConflict = QColor(240, 130, 120);
static const QColor kBpFocus = QColor(255, 214, 138);
static const QColor kBpQueue = QColor(198, 226, 246);
static const QColor kBpOddCycle = QColor(226, 87, 60);
static const QColor kBpEdgeDone = QColor(80, 175, 120);

BipartiteModule::BipartiteModule() {}

QString BipartiteModule::getModuleName() const { return "二分图判定"; }
QString BipartiteModule::getDefaultInput() const {
    return "6 6\n"
           "1 2\n"
           "1 3\n"
           "2 4\n"
           "3 4\n"
           "3 5\n"
           "4 6";
}
bool BipartiteModule::supportsGraphOptions() const { return true; }
bool BipartiteModule::supportsDirected() const { return false; }
bool BipartiteModule::supportsBidirectional() const { return false; }
GraphModuleUtils::GraphOptions BipartiteModule::getGraphOptions() const { return m_options; }
void BipartiteModule::setGraphOptions(const GraphModuleUtils::GraphOptions& options) { m_options = options; }
QString BipartiteModule::getInputHint() const {
    return "第一行输入两个正整数：顶点数 n、边数 m\n"
           "接下来 m 行，每行两个整数 u, v，表示一条无向边 u - v\n"
           "算法用 0/1 交替染色（蓝/橙两色），一旦发现相邻两点同色即判定不是二分图\n"
           "冲突时会沿 BFS 树回溯抽出那条奇环，直观说明「同色相邻」的本质原因";
}
std::map<int, QPointF> BipartiteModule::captureNodePositions() const {
    return GraphRenderHelper::captureNodePositions(m_nodes);
}
void BipartiteModule::restoreNodePositions(const std::map<int, QPointF>& positions) {
    GraphRenderHelper::restoreNodePositions(m_nodes, positions);
}

void BipartiteModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;
    GraphModuleUtils::GraphInput input;
    GraphModuleUtils::GraphOptions options = m_options;
    options.directed = false;
    m_valid = GraphModuleUtils::parse(inputData, false, options, input);
    m_mathText = createScriptText(scene, m_valid ? "二分图判定：相邻节点必须异色（2-染色）"
                                        : "输入格式：n m，随后输入 m 条无向边 u v");
    m_mathText->setFont(UiFonts::mono(15, true));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(150, 18);
    if (!m_valid) return;

    m_n = input.n;
    m_m = input.m;
    for (const auto& edge : input.edges) {
        m_edgeList.push_back({edge.u, edge.v});
        int id = ++m_edgeCount;
        m_to[id] = edge.v;
        m_next[id] = m_head[edge.u];
        m_head[edge.u] = id;
        id = ++m_edgeCount;
        m_to[id] = edge.u;
        m_next[id] = m_head[edge.v];
        m_head[edge.v] = id;
    }

    GraphRenderHelper::buildNodes(scene, m_nodes, m_n, QPointF(470, 210), m_n <= 8 ? 115 : 135, this);
    GraphRenderHelper::buildEdges(scene, m_nodes, m_edges, input.edges, options, input.weighted);

    const int startX = std::max(140, 470 - m_n * 30);
    GraphRenderHelper::addLabel(scene, "col", QPointF(30, 455));
    for (int i = 1; i <= m_n; ++i) {
        ArrayBlock* block = new ArrayBlock(i, 0, QString::number(i));
        block->setCustomValue("-1");
        block->setPos(startX + (i - 1) * 60, 470);
        scene->addItem(block);
        m_colorBlocks[i] = block;
    }

    buildQueueRow(m_n + 2);

    m_verdictText = createScriptText(scene, "");
    m_verdictText->setFont(UiFonts::cjk(14, true));
    m_verdictText->setDefaultTextColor(QColor(40, 140, 70));
    m_verdictText->setPos(150, 630);

    m_cycleText = createScriptText(scene, "");
    m_cycleText->setFont(UiFonts::mono(13, true));
    m_cycleText->setDefaultTextColor(kBpOddCycle.darker(115));
    m_cycleText->setPos(150, 666);

    m_cycleTrail = new PathTrail(kBpOddCycle);
    m_cycleTrail->setTrailWidth(4.0);
    m_cycleTrail->setHeadRadius(7.0);
    scene->addItem(m_cycleTrail);
    m_cycleTrail->clearPoints();
}

void BipartiteModule::buildQueueRow(int capacity) {
    m_queueCap = capacity;
    const double rowY = 560.0;
    const double startX = 200.0;
    GraphRenderHelper::addLabel(m_scene, "q[] 队列", QPointF(30, rowY + 4));
    for (int i = 0; i < capacity; ++i) {
        ArrayBlock* block = new ArrayBlock(i, 0, QString::number(i));
        block->setCellSize(46, 34);
        block->setShowLabel(false);
        block->showValue = false;
        block->color = QColor(250, 250, 252);
        block->setPos(startX + i * 46, rowY);
        m_scene->addItem(block);
        m_queueBlocks[i] = block;
    }

    m_queueHeadPtr = new PointerBlock("head", QColor(210, 80, 55));
    m_queueHeadPtr->setOpacity(0);
    m_queueHeadPtr->setDirection(PointerBlock::Up);
    m_queueHeadPtr->setShaftLength(28);
    m_queueHeadPtr->setAnchorOffset(PointerGeometry::belowCell(46, 34));
    m_queueHeadPtr->setLabelAtTail(9.0);
    m_scene->addItem(m_queueHeadPtr);

    m_queueTailPtr = new PointerBlock("tail", QColor(60, 140, 220));
    m_queueTailPtr->setOpacity(0);
    m_queueTailPtr->setDirection(PointerBlock::Up);
    m_queueTailPtr->setShaftLength(28);
    m_queueTailPtr->setAnchorOffset(PointerGeometry::belowCell(46, 34));
    m_queueTailPtr->setLabelAtTail(9.0);
    m_scene->addItem(m_queueTailPtr);
}

void BipartiteModule::clearQueueRow() {
    for (auto& kv : m_queueBlocks) {
        kv.second->showValue = false;
        kv.second->color = QColor(250, 250, 252);
        kv.second->update();
    }
    if (m_queueHeadPtr) m_queueHeadPtr->setOpacity(0);
    if (m_queueTailPtr) m_queueTailPtr->setOpacity(0);
}

void BipartiteModule::setQueueSlot(int index, int value, bool active) {
    if (index < 0 || index >= static_cast<int>(m_queueBlocks.size())) return;
    ArrayBlock* block = m_queueBlocks[index];
    if (active) {
        block->setCustomValue(QString::number(value));
    } else {
        block->showValue = false;
    }
    block->color = active ? kBpQueue : QColor(250, 250, 252);
    block->update();
}

void BipartiteModule::setQueuePointer(int index, const QString& label, PointerBlock* pointer) {
    if (!pointer) return;
    if (index < 0 || index >= static_cast<int>(m_queueBlocks.size())) {
        pointer->setOpacity(0);
        return;
    }
    pointer->setLabel(label);
    pointer->setOpacity(1);
    retargetPointerTo(pointer, m_queueBlocks[index], PointerGeometry::belowCell(46, 34));
}

QColor BipartiteModule::colorFor(int color) const {
    return color == 0 ? kBpBlue : kBpOrange;
}

void BipartiteModule::applyColor(int node, int color) {
    if (m_colorBlocks.count(node)) {
        m_colorBlocks[node]->setCustomValue(color == 0 ? "0" : "1");
        m_colorBlocks[node]->color = colorFor(color);
        m_colorBlocks[node]->update();
    }
    if (m_nodes.count(node)) {
        m_nodes[node]->color = colorFor(color);
        m_nodes[node]->setHighlightColor(colorFor(color));
        m_nodes[node]->update();
    }
}

std::vector<int> BipartiteModule::extractOddCycle(int u, int v) const {
    std::vector<int> chainU;
    int x = u;
    while (x > 0) { chainU.push_back(x); x = m_parent[x]; }
    std::vector<int> chainV;
    x = v;
    while (x > 0) { chainV.push_back(x); x = m_parent[x]; }
    if (chainU.empty() || chainV.empty()) return {};

    size_t common = 0;
    while (common < chainU.size() && common < chainV.size()
           && chainU[chainU.size() - 1 - common] == chainV[chainV.size() - 1 - common]) {
        ++common;
    }
    if (common == 0) return {};

    std::vector<int> cycle;
    for (size_t i = 0; i + common <= chainU.size(); ++i) cycle.push_back(chainU[i]);
    for (int i = static_cast<int>(chainV.size()) - static_cast<int>(common) - 1; i >= 0; --i) {
        cycle.push_back(chainV[i]);
    }
    return cycle;
}

std::vector<AnimFrame> BipartiteModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;

    std::fill(std::begin(m_color), std::end(m_color), -1);
    std::fill(std::begin(m_parent), std::end(m_parent), 0);
    std::fill(std::begin(m_depth), std::end(m_depth), 0);
    bool ok = true;
    std::vector<int> cycle;

    m_frames.push_back({ActionType::BP_RESET, 0, 0, 0, 0, {},
                        QString("初始化颜色数组，全部未染色"),
                        QString("col[i] = -1")});

    for (int s = 1; s <= m_n && ok; ++s) {
        if (m_color[s] != -1) continue;
        m_color[s] = 0;
        m_parent[s] = 0;
        m_depth[s] = 0;
        m_frames.push_back({ActionType::BP_COLOR, s, 0, 0, 0, {},
                            QString("从孤立分量起点 %1 开始，染成蓝色（0）").arg(s),
                            QString("col[%1] = 0（蓝）").arg(s)});
        int queue[101]{};
        int head = 0;
        int tail = 0;
        queue[tail++] = s;
        m_frames.push_back({ActionType::BP_ENQUEUE, s, 0, tail - 1, 0, {},
                            QString("起点 %1 入队").arg(s),
                            QString("q[tail++] = %1").arg(s)});
        while (head < tail && ok) {
            const int u = queue[head];
            m_frames.push_back({ActionType::BP_DEQUEUE, u, head, tail - 1, 0, {},
                                QString("队头出队：处理节点 %1").arg(u),
                                QString("q[head++] = %1").arg(u)});
            ++head;
            for (int edge = m_head[u]; edge; edge = m_next[edge]) {
                const int v = m_to[edge];
                if (m_color[v] == -1) {
                    m_color[v] = m_color[u] ^ 1;
                    m_parent[v] = u;
                    m_depth[v] = m_depth[u] + 1;
                    m_frames.push_back({ActionType::BP_COLOR, v, u, m_color[v], 0, {},
                                        QString("节点 %1 与 %2 相邻，染成%3色（%4）")
                                            .arg(v).arg(u).arg(m_color[v] == 0 ? "蓝" : "橙").arg(m_color[v]),
                                        QString("col[%1] = col[%2] ^ 1 = %3").arg(v).arg(u).arg(m_color[v])});
                    queue[tail++] = v;
                    m_frames.push_back({ActionType::BP_ENQUEUE, v, head, tail - 1, 0, {},
                                        QString("节点 %1 入队，等待扩展").arg(v),
                                        QString("q[tail++] = %1").arg(v)});
                } else {
                    const bool same = (m_color[u] == m_color[v]);
                    m_frames.push_back({ActionType::BP_CHECK, u, v, m_color[u], m_color[v], {},
                                        QString("检查边 %1—%2：两端分别是%3色和%4色")
                                            .arg(u).arg(v)
                                            .arg(m_color[u] == 0 ? "蓝" : "橙")
                                            .arg(m_color[v] == 0 ? "蓝" : "橙"),
                                        QString("col[%1] = %2，col[%3] = %4")
                                            .arg(u).arg(m_color[u]).arg(v).arg(m_color[v])});
                    if (same) {
                        ok = false;
                        cycle = extractOddCycle(u, v);
                        m_frames.push_back({ActionType::BP_CONFLICT, u, v, m_color[u], 0, {},
                                            QString("冲突！边 %1—%2 两端同色，图不是二分图").arg(u).arg(v),
                                            QString("col[%1] == col[%2] = %3 ⇒ 存在奇环").arg(u).arg(v).arg(m_color[u])});
                        break;
                    }
                }
            }
        }
    }

    int blue = 0;
    int orange = 0;
    for (int i = 1; i <= m_n; ++i) {
        if (m_color[i] == 0) ++blue;
        else if (m_color[i] == 1) ++orange;
    }

    m_frames.push_back({ActionType::BP_QUEUE_CLEAR, 0, 0, 0, 0, {},
                        QString("BFS 结束，队列已清空（head == tail）"),
                        QString("队列清空，释放全部槽位")});

    if (!ok && !cycle.empty()) {
        m_frames.push_back({ActionType::BP_ODD_CYCLE, static_cast<int>(cycle.size()), 0, 0, 0, cycle,
                            QString("沿 BFS 树回溯，抽出长度 %1 的奇环：%2")
                                .arg(cycle.size())
                                .arg([&cycle] {
                                    QStringList ids;
                                    for (int id : cycle) ids << QString::number(id);
                                    return ids.join(" → ");
                                }()),
                            QString("奇环长度 = %1（奇数）⇒ 不是二分图").arg(cycle.size())});
    }

    m_frames.push_back({ActionType::BP_FINISH, ok ? 1 : 0, m_n, blue, orange, {},
                        ok ? QString("染色无冲突，该图是二分图：%1 个蓝点、%2 个橙点").arg(blue).arg(orange)
                            : QString("存在同色相邻点，该图不是二分图"),
                        ok ? QString("两部划分：蓝 {%1}，橙 {%2}")
                                 .arg(blueText()).arg(orangeText())
                            : QString("存在奇数长度回路")});
    return m_frames;
}

QString BipartiteModule::blueText() const {
    QStringList ids;
    for (int i = 1; i <= m_n; ++i) if (m_color[i] == 0) ids << QString::number(i);
    return ids.join(", ");
}

QString BipartiteModule::orangeText() const {
    QStringList ids;
    for (int i = 1; i <= m_n; ++i) if (m_color[i] == 1) ids << QString::number(i);
    return ids.join(", ");
}

void BipartiteModule::renderFrame(const AnimFrame& frame) {
    if (m_mathText) {
        setScriptText(m_mathText, frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    }
    if (!m_valid) return;

    if (frame.type == ActionType::BP_RESET) {
        for (auto& kv : m_nodes) {
            kv.second->color = kBpIdle;
            kv.second->setHighlightColor(Qt::transparent);
            kv.second->update();
        }
        for (auto& kv : m_edges) {
            kv.second->setColor(Qt::black);
            kv.second->setPenWidth(2.0);
            kv.second->update();
        }
        for (auto& kv : m_colorBlocks) {
            kv.second->setCustomValue("-1");
            kv.second->color = Qt::white;
            kv.second->update();
        }
        clearQueueRow();
        if (m_cycleTrail) {
            m_cycleTrail->clearPoints();
            m_cycleTrail->setProgress(0.0);
        }
        if (m_cycleText) {
            setScriptText(m_cycleText, "");
            m_cycleText->setDefaultTextColor(kBpOddCycle.darker(115));
        }
        if (m_verdictText) {
            setScriptText(m_verdictText, "");
            m_verdictText->setDefaultTextColor(QColor(40, 140, 70));
        }
    } else if (frame.type == ActionType::BP_COLOR) {
        const int target = frame.u;
        const int color = frame.val1;
        applyColor(target, color);
        if (m_animated && m_nodes.count(target)) {
            playScalePulse(m_nodes[target], 1.22, 240);
        }
        GraphEdge* edge = GraphRenderHelper::findEdge(m_edges, frame.v, target);
        if (edge && frame.v > 0) {
            edge->setColor(kBpEdgeDone);
            edge->update();
            playEdgeDiscover(edge, m_nodes.count(target) ? m_nodes[target] : nullptr, kBpEdgeDone, 500);
        }
    } else if (frame.type == ActionType::BP_ENQUEUE) {
        const int value = frame.u;
        const int tailIndex = frame.val1;
        setQueueSlot(tailIndex, value, true);
        setQueuePointer(tailIndex, "tail", m_queueTailPtr);
        if (m_animated) playScalePulse(m_queueBlocks[tailIndex], 1.14, 200);
    } else if (frame.type == ActionType::BP_DEQUEUE) {
        const int value = frame.u;
        const int headIndex = frame.val1;
        const int tailIndex = frame.val2;
        setQueueSlot(headIndex, value, false);
        setQueuePointer(headIndex, "head", m_queueHeadPtr);
        setQueuePointer(tailIndex, "tail", m_queueTailPtr);
        if (m_nodes.count(value)) {
            m_nodes[value]->setHighlightColor(kBpFocus);
            m_nodes[value]->update();
            playNodeWake(m_nodes[value], kBpFocus, 32.0, 440);
        }
    } else if (frame.type == ActionType::BP_CHECK) {
        GraphEdge* edge = GraphRenderHelper::findEdge(m_edges, frame.u, frame.v);
        const bool same = (frame.val1 == frame.val2);
        if (edge) {
            edge->setColor(same ? kBpConflict : kBpEdgeDone);
            edge->update();
            if (same) playEdgeTravelFrom(edge, kBpConflict, frame.u, 440);
            else playEdgeTravelFrom(edge, kBpEdgeDone, frame.u, 400);
        }
        if (m_nodes.count(frame.u)) {
            m_nodes[frame.u]->setHighlightColor(kBpFocus);
            m_nodes[frame.u]->update();
            playNodeWake(m_nodes[frame.u], kBpFocus, 30.0, 440);
        }
        if (m_nodes.count(frame.v)) {
            m_nodes[frame.v]->setHighlightColor(kBpFocus);
            m_nodes[frame.v]->update();
            playNodeWake(m_nodes[frame.v], kBpFocus, 30.0, 440);
        }
    } else if (frame.type == ActionType::BP_CONFLICT) {
        if (m_nodes.count(frame.u)) {
            m_nodes[frame.u]->color = kBpConflict;
            m_nodes[frame.u]->setHighlightColor(kBpConflict);
            m_nodes[frame.u]->update();
            if (m_animated) {
                playScalePulse(m_nodes[frame.u], 1.3, 300);
                playNodeRipple(m_nodes[frame.u], kBpConflict, 52.0, 640);
            }
        }
        if (m_nodes.count(frame.v)) {
            m_nodes[frame.v]->color = kBpConflict;
            m_nodes[frame.v]->setHighlightColor(kBpConflict);
            m_nodes[frame.v]->update();
            if (m_animated) {
                playScalePulse(m_nodes[frame.v], 1.3, 300);
                playNodeRipple(m_nodes[frame.v], kBpConflict, 52.0, 640);
            }
        }
        GraphEdge* edge = GraphRenderHelper::findEdge(m_edges, frame.u, frame.v);
        if (edge) {
            edge->setColor(kBpConflict);
            edge->setPenWidth(5.0);
            edge->update();
            playEdgeTravelFrom(edge, kBpConflict, frame.u, 520);
        }
        if (m_verdictText) {
            setScriptText(m_verdictText, QString("冲突：%1 与 %2 同色，非二分图").arg(frame.u).arg(frame.v));
            m_verdictText->setDefaultTextColor(kBpConflict.darker(115));
        }
    } else if (frame.type == ActionType::BP_ODD_CYCLE) {
        const std::vector<int>& cycle = frame.group;
        if (cycle.empty()) return;
        QVector<QPointF> points;
        for (int id : cycle) {
            if (!m_nodes.count(id)) continue;
            points.append(m_nodes[id]->pos());
        }
        if (!m_cycleTrail) return;
        if (points.size() >= 2) {
            playPathTrail(m_cycleTrail, points, kBpOddCycle, 700);
        }
        for (int id : cycle) {
            if (!m_nodes.count(id)) continue;
            m_nodes[id]->color = kBpOddCycle;
            m_nodes[id]->setHighlightColor(kBpOddCycle);
            m_nodes[id]->update();
            if (m_animated) playNodePop(m_nodes[id], 1.16, 280);
        }
        for (size_t i = 0; i < cycle.size(); ++i) {
            const int a = cycle[i];
            const int b = cycle[(i + 1) % cycle.size()];
            GraphEdge* edge = GraphRenderHelper::findEdge(m_edges, a, b);
            if (!edge) continue;
            edge->setColor(kBpOddCycle);
            edge->setPenWidth(4.5);
            edge->update();
        }
        if (m_cycleText) {
            setScriptText(m_cycleText, frame.explanation);
            m_cycleText->setDefaultTextColor(kBpOddCycle.darker(115));
        }
    } else if (frame.type == ActionType::BP_QUEUE_CLEAR) {
        clearQueueRow();
        if (m_animated) {
            for (auto& kv : m_queueBlocks) {
                playCellRipple(kv.second, kBpQueue, 0.8, 420);
            }
        }
    } else if (frame.type == ActionType::BP_FINISH) {
        if (m_verdictText) {
            const bool ok = frame.u == 1;
            setScriptText(m_verdictText, ok ? QString("结论：该图是二分图")
                                          : QString("结论：该图不是二分图"));
            m_verdictText->setDefaultTextColor(ok ? QColor(40, 140, 70) : kBpConflict.darker(115));
        }
        for (auto& kv : m_nodes) {
            if (kv.second->highlightColor == kBpConflict) continue;
            kv.second->setHighlightColor(Qt::transparent);
            kv.second->update();
            playNodePop(kv.second, 1.12, 280);
        }
    }
}

void BipartiteModule::reset() {
    m_frames.clear();
    m_nodes.clear();
    m_edges.clear();
    m_colorBlocks.clear();
    m_queueBlocks.clear();
    m_edgeList.clear();
    m_mathText = nullptr;
    m_verdictText = nullptr;
    m_cycleText = nullptr;
    m_cycleTrail = nullptr;
    m_queueHeadPtr = nullptr;
    m_queueTailPtr = nullptr;
    m_n = 0;
    m_m = 0;
    m_edgeCount = 0;
    m_queueCap = 0;
    m_valid = false;
    std::fill(std::begin(m_head), std::end(m_head), 0);
    std::fill(std::begin(m_to), std::end(m_to), 0);
    std::fill(std::begin(m_next), std::end(m_next), 0);
    std::fill(std::begin(m_color), std::end(m_color), -1);
    std::fill(std::begin(m_parent), std::end(m_parent), 0);
    std::fill(std::begin(m_depth), std::end(m_depth), 0);
}

void BipartiteModule::releaseSceneItems() {
    m_nodes.clear();
    m_edges.clear();
    m_colorBlocks.clear();
    m_queueBlocks.clear();
    m_mathText = nullptr;
    m_verdictText = nullptr;
    m_cycleText = nullptr;
    m_cycleTrail = nullptr;
    m_queueHeadPtr = nullptr;
    m_queueTailPtr = nullptr;
}
