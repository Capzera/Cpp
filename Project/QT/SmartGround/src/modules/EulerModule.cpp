#include "EulerModule.h"
#include "GraphModuleUtils.h"
#include "GraphRenderHelper.h"
#include <QFont>
#include <algorithm>
#include "ui/UiFonts.h"

static const QColor kEuIdle = QColor(252, 252, 254);
static const QColor kEuWalk = QColor(255, 205, 130);
static const QColor kEuPath = QColor(140, 205, 165);
static const QColor kEuDead = QColor(205, 210, 218);
static const QColor kEuBridge = QColor(158, 122, 214);
static const QColor kEuSplice = QColor(226, 96, 62);
static const QColor kEuOdd = QColor(232, 108, 96);

EulerModule::EulerModule() {}

QString EulerModule::getModuleName() const { return "欧拉路径"; }
QString EulerModule::getDefaultInput() const {
    return "5 6\n"
           "1 2\n"
           "1 3\n"
           "2 3\n"
           "3 4\n"
           "3 5\n"
           "4 5";
}
bool EulerModule::supportsGraphOptions() const { return true; }
bool EulerModule::supportsDirected() const { return true; }
bool EulerModule::supportsBidirectional() const { return false; }
GraphModuleUtils::GraphOptions EulerModule::getGraphOptions() const { return m_options; }
void EulerModule::setGraphOptions(const GraphModuleUtils::GraphOptions& options) { m_options = options; }
QString EulerModule::getInputHint() const {
    return "第一行输入两个正整数：顶点数 n、边数 m\n"
           "接下来 m 行，每行两个整数 u, v，表示一条边 u -> v\n"
           "在「图结构选项」里可切换有向 / 无向：\n"
           "  无向图用度数 deg[i]，奇度点为 0 或 2 个才存在欧拉路径\n"
           "  有向图用入度 in[i] 与出度 out[i]，两者相等才存在欧拉回路\n"
           "算法用 Hierholzer：走到死路就压栈，回溯时依次出栈得到欧拉路径";
}
std::map<int, QPointF> EulerModule::captureNodePositions() const {
    return GraphRenderHelper::captureNodePositions(m_nodes);
}
void EulerModule::restoreNodePositions(const std::map<int, QPointF>& positions) {
    GraphRenderHelper::restoreNodePositions(m_nodes, positions);
}

QString EulerModule::moduleSettingsTitle() const { return "欧拉路径选项"; }

QVector<ModuleField> EulerModule::moduleSettingsFields() const {
    QVector<ModuleField> fields;
    fields.append(ModuleField::makeToggle("bridge", "高亮延迟边", m_highlightBridges));
    return fields;
}

void EulerModule::applyModuleSetting(const QString& key, const ModuleField& field) {
    if (key == "bridge") m_highlightBridges = field.boolValue;
}

void EulerModule::buildStackColumn(std::vector<ArrayBlock*>& column, double x, double baseY, int capacity) {
    for (int i = 0; i < capacity; ++i) {
        ArrayBlock* block = new ArrayBlock(i, 0, QString::number(i));
        block->setCellSize(44, 32);
        block->setShowLabel(false);
        block->showValue = false;
        block->setPos(x, baseY - (i + 1) * 34.0);
        m_scene->addItem(block);
        column.push_back(block);
    }
}

void EulerModule::setStackSlot(std::vector<ArrayBlock*>& column, int index, int value, bool filled) {
    if (index < 0 || index >= static_cast<int>(column.size())) return;
    ArrayBlock* block = column[index];
    block->showValue = filled;
    block->value = value;
    block->update();
}

void EulerModule::clearStackColumn(std::vector<ArrayBlock*>& column) {
    for (ArrayBlock* block : column) {
        block->showValue = false;
        block->color = Qt::white;
        block->update();
    }
}

void EulerModule::clearTrail() {
    if (m_trail) {
        m_trail->clearPoints();
        m_trail->setProgress(0.0);
    }
}

void EulerModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;
    GraphModuleUtils::GraphInput input;
    GraphModuleUtils::GraphOptions options = m_options;
    m_directed = options.directed;
    m_valid = GraphModuleUtils::parse(inputData, false, options, input);
    m_mathText = createScriptText(scene, m_valid
        ? (m_directed ? QString("欧拉路径（有向图）：统计入度 out[]，用 Hierholzer 回溯出栈成路径")
                      : QString("欧拉路径（无向图）：统计度数 deg[]，用 Hierholzer 回溯出栈成路径"))
        : QString("输入格式：n m，随后输入 m 条边 u v"));
    m_mathText->setFont(UiFonts::mono(15, true));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(140, 18);
    if (!m_valid) return;

    m_n = input.n;
    m_m = input.m;
    for (const auto& edge : input.edges) m_edgeList.push_back({edge.u, edge.v});

    GraphRenderHelper::buildNodes(scene, m_nodes, m_n, QPointF(270, 262), m_n <= 8 ? 105 : 125, this);
    GraphRenderHelper::buildEdges(scene, m_nodes, m_edges, input.edges, options, input.weighted);

    const double degY = 462.0;
    const double rowGap = 76.0;
    const int startX = 170;
    const int cellGap = 56;

    if (m_directed) {
        GraphRenderHelper::addLabel(scene, "in[] 入度", QPointF(0, degY + 8));
        GraphRenderHelper::addLabel(scene, "out[] 出度", QPointF(0, degY + rowGap + 8));
    } else {
        GraphRenderHelper::addLabel(scene, "deg[] 度数", QPointF(0, degY + 8));
    }

    for (int i = 1; i <= m_n; ++i) {
        if (m_directed) {
            ArrayBlock* inDeg = new ArrayBlock(i, 0, QString::number(i));
            inDeg->setCellSize(52, 36);
            inDeg->setCustomValue("0");
            inDeg->setPos(startX + (i - 1) * cellGap, degY);
            scene->addItem(inDeg);
            m_indegBlocks[i] = inDeg;

            ArrayBlock* outDeg = new ArrayBlock(i, 0, QString::number(i));
            outDeg->setCellSize(52, 36);
            outDeg->setCustomValue("0");
            outDeg->setPos(startX + (i - 1) * cellGap, degY + rowGap);
            scene->addItem(outDeg);
            m_outdegBlocks[i] = outDeg;
        } else {
            ArrayBlock* deg = new ArrayBlock(i, 0, QString::number(i));
            deg->setCellSize(52, 36);
            deg->setCustomValue("0");
            deg->setPos(startX + (i - 1) * cellGap, degY);
            scene->addItem(deg);
            m_degBlocks[i] = deg;
        }
    }

    const int displayCap = qMin(m_m + 2, 10);
    const double stackBaseY = 420.0;
    const double stackX = 470.0;
    const double pathX = 600.0;
    const double stackLabelY = stackBaseY - displayCap * 34.0 - 30.0;

    GraphRenderHelper::addLabel(scene, "stk[] 栈", QPointF(stackX - 10, stackLabelY));
    GraphRenderHelper::addLabel(scene, "path[] 答案", QPointF(pathX - 10, stackLabelY));

    buildStackColumn(m_stackBlocks, stackX, stackBaseY, displayCap);
    buildStackColumn(m_pathBlocks, pathX, stackBaseY, displayCap);

    const double resultY = 618.0;
    GraphRenderHelper::addLabel(scene, "欧拉路径", QPointF(0, resultY + 8));
    const int resultCap = qMin(m_m + 2, 16);
    for (int i = 0; i < resultCap; ++i) {
        ArrayBlock* block = new ArrayBlock(i, 0, QString::number(i));
        block->setCellSize(44, 32);
        block->setShowLabel(false);
        block->showValue = false;
        block->setPos(120 + i * 46.0, resultY);
        scene->addItem(block);
        m_resultBlocks.push_back(block);
    }

    m_trail = new PathTrail(kEuPath);
    m_trail->setTrailWidth(5.0);
    m_trail->setHeadRadius(7.5);
    scene->addItem(m_trail);
    m_trail->clearPoints();

    m_spliceLine = new LinkLine(QPointF(), QPointF());
    m_spliceLine->setStyle(kEuSplice, 3.0, true, true);
    m_spliceLine->setZValue(6);
    m_spliceLine->setOpacity(0);
    scene->addItem(m_spliceLine);

    m_verdictText = createScriptText(scene, "");
    m_verdictText->setFont(UiFonts::cjk(13, true));
    m_verdictText->setDefaultTextColor(QColor(40, 140, 70));
    m_verdictText->setPos(150, 682);

    m_bridgeText = createScriptText(scene, "");
    m_bridgeText->setFont(UiFonts::cjk(12, true));
    m_bridgeText->setDefaultTextColor(kEuBridge.darker(115));
    m_bridgeText->setPos(150, 720);
}

std::vector<AnimFrame> EulerModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;

    std::vector<std::vector<int>> adj(m_n + 1);
    std::vector<int> deg(m_n + 1, 0);
    std::vector<int> indeg(m_n + 1, 0);
    std::vector<int> outdeg(m_n + 1, 0);
    std::vector<std::pair<int, int>> edgeEnds(m_m + 4);
    int edgeId = 0;
    for (const auto& e : m_edgeList) {
        ++edgeId;
        if (m_directed) {
            adj[e.first].push_back(edgeId);
            ++outdeg[e.first];
            ++indeg[e.second];
            edgeEnds[edgeId] = {e.first, e.second};
        } else {
            adj[e.first].push_back(edgeId);
            adj[e.second].push_back(edgeId);
            edgeEnds[edgeId] = {e.first, e.second};
            ++deg[e.first];
            ++deg[e.second];
        }
    }
    std::vector<bool> used(edgeId + 1, false);

    m_frames.push_back({ActionType::EU_RESET, 0, 0, 0, 0, {},
                        m_directed ? QString("统计每个节点的入度与出度")
                                   : QString("统计每个节点的度数"),
                        m_directed ? QString("in[i] / out[i]") : QString("deg[i] = 关联边数")});

    for (int i = 1; i <= m_n; ++i) {
        if (m_directed) {
            m_frames.push_back({ActionType::EU_DEGREE, i, 0, indeg[i], outdeg[i], {},
                                QString("节点 %1：入度 in[%1] = %2，出度 out[%1] = %3")
                                    .arg(i).arg(indeg[i]).arg(outdeg[i]),
                                QString("in[%1] = %2, out[%1] = %3").arg(i).arg(indeg[i]).arg(outdeg[i])});
        } else {
            m_frames.push_back({ActionType::EU_DEGREE, i, 0, deg[i], deg[i] % 2, {},
                                QString("节点 %1 的度数为 %2%3")
                                    .arg(i).arg(deg[i])
                                    .arg(deg[i] % 2 ? QString("（奇度点）") : QString()),
                                QString("deg[%1] = %2").arg(i).arg(deg[i])});
        }
    }

    int start = 1;
    int oddCount = 0;
    bool feasible = true;
    if (m_directed) {
        int plus = 0;
        int minus = 0;
        for (int i = 1; i <= m_n; ++i) {
            const int diff = outdeg[i] - indeg[i];
            if (diff == 1) ++plus;
            else if (diff == -1) ++minus;
            else if (diff != 0) { feasible = false; break; }
        }
        if (feasible && plus == 1 && minus == 1) {
            for (int i = 1; i <= m_n; ++i) if (outdeg[i] - indeg[i] == 1) { start = i; break; }
        } else {
            feasible = false;
        }
        if (!feasible) {
            m_frames.push_back({ActionType::EU_DONE, 0, 0, 0, 1, {},
                                QString("有向图不满足欧拉路径条件：出度与入度必须处处相等，"
                                        "或恰有一处出度比入度大 1、一处小 1"),
                                QString("有向图欧拉路径判定失败")});
            return m_frames;
        }
    } else {
        for (int i = 1; i <= m_n; ++i) {
            if (deg[i] % 2 == 1) {
                ++oddCount;
                start = i;
            }
        }
        if (oddCount != 0 && oddCount != 2) {
            m_frames.push_back({ActionType::EU_DONE, 0, oddCount, 0, 1, {},
                                QString("奇度节点有 %1 个（应为 0 或 2），不存在欧拉路径").arg(oddCount),
                                QString("奇度点数量必须为 0 或 2")});
            return m_frames;
        }
    }

    std::vector<int> stack;
    std::vector<int> path;
    std::vector<int> incoming(m_m + 4, 0);
    stack.push_back(start);
    incoming[0] = 0;
    m_frames.push_back({ActionType::EU_PUSH, start, 0, 0, 0, {},
                        QString("从起点 %1 出发入栈").arg(start),
                        QString("stk[0] = %1").arg(start)});

    int guard = 0;
    while (!stack.empty() && guard < 4000) {
        ++guard;
        const int u = stack.back();
        bool moved = false;
        for (int eid : adj[u]) {
            if (used[eid]) continue;
            used[eid] = true;
            const int v = (edgeEnds[eid].first == u) ? edgeEnds[eid].second : edgeEnds[eid].first;
            edgeEnds[eid] = {u, v};
            m_frames.push_back({ActionType::EU_EDGE, u, v, eid, 0, {},
                                QString("沿边 %1—%2 前进").arg(u).arg(v),
                                QString("stk.push(%1)").arg(v)});
            stack.push_back(v);
            incoming[stack.size() - 1] = eid;
            m_frames.push_back({ActionType::EU_PUSH, v, 0, static_cast<int>(stack.size()) - 1, 0, {},
                                QString("节点 %1 入栈（栈深 %2）").arg(v).arg(stack.size()),
                                QString("stk.push(%1)").arg(v)});
            moved = true;
            break;
        }
        if (moved) continue;

        const int dead = stack.back();
        const int retreatEdge = incoming[stack.size() - 1];
        m_frames.push_back({ActionType::EU_DEADEND, dead, 0, static_cast<int>(stack.size()) - 1, 0, {},
                            QString("节点 %1 已无未用边 ⇒ 走到死路，必须回溯").arg(dead),
                            QString("stk.top() = %1 无可用边").arg(dead)});

        bool splice = false;
        for (int x : path) {
            if (x == dead) { splice = true; break; }
        }
        stack.pop_back();
        path.push_back(dead);
        m_frames.push_back({ActionType::EU_POP, dead, 0, static_cast<int>(path.size()) - 1, 0, {},
                            QString("节点 %1 出栈并记入答案").arg(dead),
                            QString("path.push(%1)").arg(dead)});

        if (retreatEdge > 0) {
            m_frames.push_back({ActionType::EU_BRIDGE, edgeEnds[retreatEdge].second, edgeEnds[retreatEdge].first,
                                retreatEdge, 0, {},
                                QString("边 %1—%2 是延迟边：它在回溯时才被消费，最终路径必须在此拼接")
                                    .arg(edgeEnds[retreatEdge].second).arg(edgeEnds[retreatEdge].first),
                                QString("延迟边 ⇒ 需要 splice 拼接")});
        }

        if (splice) {
            m_frames.push_back({ActionType::EU_SPLICE, dead, 0, static_cast<int>(path.size()) - 1, 0, {},
                                QString("节点 %1 已在答案中 ⇒ 两段回路在此拼接成完整欧拉回路").arg(dead),
                                QString("splice：在 %1 处把两段接起来").arg(dead)});
        }

        if (!stack.empty()) {
            m_frames.push_back({ActionType::EU_BACKTRACK, stack.back(), 0, static_cast<int>(stack.size()) - 1,
                                retreatEdge, {},
                                QString("回退到节点 %1，继续找未用边").arg(stack.back()),
                                QString("回溯至 stk.top() = %1").arg(stack.back())});
        }
    }

    std::vector<int> ordered(path.rbegin(), path.rend());
    m_frames.push_back({ActionType::EU_PATH, static_cast<int>(ordered.size()), 0, 0, 0, ordered,
                        QString("逆序出栈得到欧拉路径，共 %1 个节点").arg(ordered.size()),
                        QString("路径长度 = %1 条边").arg(ordered.size() - 1)});

    const bool circuit = m_directed ? true : (oddCount == 0);
    m_frames.push_back({ActionType::EU_DONE, 1, 0, static_cast<int>(ordered.size()), 0, ordered,
                        circuit ? QString("欧拉回路：起点与终点相同，可以一笔画")
                                : QString("欧拉路径：从奇度点出发，可以一笔画"),
                        circuit ? QString("存在欧拉回路")
                                : QString("恰有两个奇度点，存在欧拉路径")});
    return m_frames;
}

void EulerModule::renderFrame(const AnimFrame& frame) {
    if (m_mathText) {
        setScriptText(m_mathText, frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    }
    if (!m_valid) return;

    if (frame.type == ActionType::EU_RESET) {
        for (auto& kv : m_nodes) {
            kv.second->color = kEuIdle;
            kv.second->setHighlightColor(Qt::transparent);
            kv.second->update();
        }
        for (auto& kv : m_edges) {
            kv.second->setColor(Qt::black);
            kv.second->setPenWidth(2.0);
            kv.second->update();
        }
        for (auto& kv : m_degBlocks) {
            kv.second->setCustomValue("0");
            kv.second->color = Qt::white;
            kv.second->update();
        }
        for (auto& kv : m_indegBlocks) {
            kv.second->setCustomValue("0");
            kv.second->color = Qt::white;
            kv.second->update();
        }
        for (auto& kv : m_outdegBlocks) {
            kv.second->setCustomValue("0");
            kv.second->color = Qt::white;
            kv.second->update();
        }
        clearStackColumn(m_stackBlocks);
        clearStackColumn(m_pathBlocks);
        clearStackColumn(m_resultBlocks);
        clearTrail();
        if (m_spliceLine) {
            m_spliceLine->setOpacity(0);
            m_spliceLine->setPoints(QPointF(), QPointF());
        }
        if (m_verdictText) {
            setScriptText(m_verdictText, "");
            m_verdictText->setDefaultTextColor(QColor(40, 140, 70));
        }
        if (m_bridgeText) {
            setScriptText(m_bridgeText, "");
            m_bridgeText->setDefaultTextColor(kEuBridge.darker(115));
        }
    } else if (frame.type == ActionType::EU_DEGREE) {
        const int i = frame.u;
        if (m_directed) {
            if (m_indegBlocks.count(i)) {
                m_indegBlocks[i]->setCustomValue(QString::number(frame.val1));
                m_indegBlocks[i]->color = QColor(160, 205, 245);
                m_indegBlocks[i]->update();
            }
            if (m_outdegBlocks.count(i)) {
                m_outdegBlocks[i]->setCustomValue(QString::number(frame.val2));
                m_outdegBlocks[i]->color = QColor(255, 210, 145);
                m_outdegBlocks[i]->update();
            }
        } else {
            if (m_degBlocks.count(i)) {
                m_degBlocks[i]->setCustomValue(QString::number(frame.val1));
                m_degBlocks[i]->color = frame.val2 == 1 ? kEuOdd : QColor(160, 205, 245);
                m_degBlocks[i]->update();
                if (m_animated) playScalePulse(m_degBlocks[i], 1.2, 220);
            }
        }
        if (m_nodes.count(i)) {
            m_nodes[i]->setHighlightColor(frame.val2 == 1 && !m_directed ? kEuOdd : QColor(255, 235, 180));
            m_nodes[i]->update();
            playNodeWake(m_nodes[i], frame.val2 == 1 && !m_directed ? kEuOdd : QColor(255, 215, 140), 30.0, 440);
        }
    } else if (frame.type == ActionType::EU_PUSH) {
        const int index = frame.val1;
        setStackSlot(m_stackBlocks, index, frame.u, true);
        if (index >= 0 && index < static_cast<int>(m_stackBlocks.size())) {
            m_stackBlocks[index]->color = kEuWalk;
            m_stackBlocks[index]->update();
            if (m_animated) playScalePulse(m_stackBlocks[index], 1.2, 220);
        }
        if (m_nodes.count(frame.u)) {
            m_nodes[frame.u]->color = kEuWalk;
            m_nodes[frame.u]->setHighlightColor(kEuWalk);
            m_nodes[frame.u]->update();
            playNodePop(m_nodes[frame.u], 1.18, 280);
        }
    } else if (frame.type == ActionType::EU_EDGE) {
        GraphEdge* edge = GraphRenderHelper::findEdge(m_edges, frame.u, frame.v, m_directed);
        if (edge) {
            edge->setColor(QColor(235, 155, 55));
            edge->setPenWidth(3.5);
            edge->update();
            playEdgeTravelFrom(edge, QColor(235, 155, 55), frame.u, 460);
        }
    } else if (frame.type == ActionType::EU_DEADEND) {
        const int node = frame.u;
        if (m_nodes.count(node)) {
            m_nodes[node]->color = kEuDead;
            m_nodes[node]->setHighlightColor(QColor(255, 170, 150));
            m_nodes[node]->update();
            if (m_animated) {
                playNodeRipple(m_nodes[node], QColor(226, 96, 62), 40.0, 560);
                playScalePulse(m_nodes[node], 0.94, 240);
            }
        }
        const int index = frame.val1;
        if (index >= 0 && index < static_cast<int>(m_stackBlocks.size())) {
            m_stackBlocks[index]->color = kEuDead;
            m_stackBlocks[index]->update();
        }
    } else if (frame.type == ActionType::EU_POP) {
        const int index = frame.val1;
        setStackSlot(m_pathBlocks, index, frame.u, true);
        if (index >= 0 && index < static_cast<int>(m_pathBlocks.size())) {
            m_pathBlocks[index]->color = kEuPath;
            m_pathBlocks[index]->update();
            if (m_animated) playScalePulse(m_pathBlocks[index], 1.18, 240);
        }
        if (m_nodes.count(frame.u)) {
            m_nodes[frame.u]->color = kEuDead;
            m_nodes[frame.u]->update();
            playNodePop(m_nodes[frame.u], 0.94, 260);
        }
    } else if (frame.type == ActionType::EU_BRIDGE) {
        if (!m_highlightBridges) return;
        GraphEdge* edge = GraphRenderHelper::findEdge(m_edges, frame.u, frame.v, m_directed);
        if (edge) {
            edge->setColor(kEuBridge);
            edge->setPenWidth(4.0);
            edge->update();
            if (m_animated) playEdgeTraceFrom(edge, kEuBridge, frame.u, 520);
        }
        if (m_bridgeText) {
            setScriptText(m_bridgeText, frame.explanation);
            m_bridgeText->setDefaultTextColor(kEuBridge.darker(115));
        }
    } else if (frame.type == ActionType::EU_BACKTRACK) {
        const int node = frame.u;
        if (m_nodes.count(node)) {
            m_nodes[node]->setHighlightColor(kEuWalk);
            m_nodes[node]->update();
            if (m_animated) playNodeWake(m_nodes[node], kEuWalk, 32.0, 480);
        }
        const int index = frame.val1;
        if (index >= 0 && index < static_cast<int>(m_stackBlocks.size())) {
            m_stackBlocks[index]->color = kEuWalk;
            m_stackBlocks[index]->update();
        }
    } else if (frame.type == ActionType::EU_SPLICE) {
        const int node = frame.u;
        if (m_nodes.count(node) && m_spliceLine) {
            const QPointF a = m_nodes[node]->pos();
            const QPointF b = a + QPointF(0, -70);
            m_spliceLine->setPoints(a, b);
            m_spliceLine->setOpacity(1);
            if (m_animated) {
                playFadeItem(m_spliceLine, 0.0, 1.0, 420);
                playRipple(a, kEuSplice, 46.0, 620);
            }
            m_nodes[node]->setHighlightColor(kEuSplice);
            m_nodes[node]->update();
            if (m_animated) playNodePop(m_nodes[node], 1.2, 280);
        }
        if (m_bridgeText) {
            setScriptText(m_bridgeText, frame.explanation);
            m_bridgeText->setDefaultTextColor(kEuSplice.darker(115));
        }
    } else if (frame.type == ActionType::EU_PATH) {
        for (auto* b : m_pathBlocks) {
            if (b->showValue) { b->color = kEuPath; b->update(); }
        }
        const std::vector<int>& ordered = frame.group;
        for (size_t i = 0; i < ordered.size() && i < m_resultBlocks.size(); ++i) {
            m_resultBlocks[i]->showValue = true;
            m_resultBlocks[i]->value = ordered[i];
            m_resultBlocks[i]->color = kEuPath;
            m_resultBlocks[i]->update();
        }
        if (m_trail && !ordered.empty()) {
            QVector<QPointF> points;
            for (int id : ordered) {
                if (!m_nodes.count(id)) continue;
                points.append(m_nodes[id]->pos());
            }
            if (points.size() >= 2) playPathTrail(m_trail, points, kEuPath, 760);
        }
    } else if (frame.type == ActionType::EU_DONE) {
        if (m_verdictText) {
            const bool failed = frame.val2 == 1;
            setScriptText(m_verdictText, failed ? QString("结论：不存在欧拉路径")
                                               : QString("结论：存在欧拉路径，可以一笔画"));
            m_verdictText->setDefaultTextColor(failed ? QColor(200, 60, 60) : QColor(40, 140, 70));
        }
        if (m_spliceLine) m_spliceLine->setOpacity(0);
        if (frame.val2 != 1) {
            for (auto& kv : m_edges) {
                if (kv.second->color == QColor(235, 155, 55)) {
                    kv.second->setColor(kEuPath);
                    kv.second->setPenWidth(4.0);
                    kv.second->update();
                }
            }
            for (auto& kv : m_nodes) playNodePop(kv.second, 1.10, 260);
        }
    }
}

void EulerModule::reset() {
    m_frames.clear();
    m_nodes.clear();
    m_edges.clear();
    m_degBlocks.clear();
    m_indegBlocks.clear();
    m_outdegBlocks.clear();
    m_stackBlocks.clear();
    m_pathBlocks.clear();
    m_resultBlocks.clear();
    m_edgeList.clear();
    m_mathText = nullptr;
    m_verdictText = nullptr;
    m_bridgeText = nullptr;
    m_trail = nullptr;
    m_spliceLine = nullptr;
    m_n = 0;
    m_m = 0;
    m_directed = false;
    m_valid = false;
}

void EulerModule::releaseSceneItems() {
    m_nodes.clear();
    m_edges.clear();
    m_degBlocks.clear();
    m_indegBlocks.clear();
    m_outdegBlocks.clear();
    m_stackBlocks.clear();
    m_pathBlocks.clear();
    m_resultBlocks.clear();
    m_mathText = nullptr;
    m_verdictText = nullptr;
    m_bridgeText = nullptr;
    m_trail = nullptr;
    m_spliceLine = nullptr;
}
