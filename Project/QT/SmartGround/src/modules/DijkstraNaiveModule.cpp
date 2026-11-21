#include "DijkstraNaiveModule.h"
#include "GraphModuleUtils.h"
#include "GraphRenderHelper.h"
#include <QFont>
#include <algorithm>
#include <cmath>
#include <iterator>

DijkstraNaiveModule::DijkstraNaiveModule() : m_pickPointer(nullptr), m_relaxPointer(nullptr), m_mathText(nullptr), m_n(0), m_m(0), m_src(1), m_edgeCount(0), m_valid(false) {
    reset();
}

QString DijkstraNaiveModule::getModuleName() const { return "Dijkstra 最短路"; }
QString DijkstraNaiveModule::getDefaultInput() const { return "6 9 1\n1 2 7\n1 3 9\n1 6 14\n2 3 10\n2 4 15\n3 4 11\n3 6 2\n4 5 6\n5 6 9"; }
bool DijkstraNaiveModule::supportsGraphOptions() const { return true; }
bool DijkstraNaiveModule::supportsDirected() const { return true; }
GraphModuleUtils::GraphOptions DijkstraNaiveModule::getGraphOptions() const { return m_options; }
void DijkstraNaiveModule::setGraphOptions(const GraphModuleUtils::GraphOptions& options) { m_options = options; }
QString DijkstraNaiveModule::getInputHint() const {
    return "第一行输入三个正整数：顶点数 n、边数 m、源点 src\n"
           "接下来 m 行，每行三个整数 u, v, w，表示一条权值为 w 的边\n"
           "权值必须非负，否则本模块拒绝该输入";
}
std::map<int, QPointF> DijkstraNaiveModule::captureNodePositions() const {
    return GraphRenderHelper::captureNodePositions(m_nodes);
}
void DijkstraNaiveModule::restoreNodePositions(const std::map<int, QPointF>& positions) {
    GraphRenderHelper::restoreNodePositions(m_nodes, positions);
}

void DijkstraNaiveModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;
    GraphModuleUtils::GraphInput input;
    m_valid = GraphModuleUtils::parse(inputData, true, m_options, input);
    if (m_valid && input.weighted) for (const auto& edge : input.edges) if (edge.w < 0) m_valid = false;
    m_mathText = createScriptText(scene, m_valid ? "朴素 Dijkstra：每轮扫描全体未确定点，选择 dist 最小者" : "输入格式：n m src，随后输入 m 条非负权边 u v w");
    m_mathText->setFont(QFont("Consolas", 14, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(110, 18);
    if (!m_valid) return;
    m_n = input.n;
    m_m = input.m;
    m_src = input.src;
    auto addArc = [this](int u, int v, int weight) {
        int id = ++m_edgeCount;
        m_to[id] = v;
        m_weight[id] = weight;
        m_next[id] = m_head[u];
        m_head[u] = id;
    };
    for (const auto& edge : input.edges) {
        int w = liveValue("w", static_cast<int>(m_edgeList.size()), edge.w);
        m_edgeList.push_back({edge.u, edge.v, w});
        addArc(edge.u, edge.v, w);
        if (!m_options.directed) addArc(edge.v, edge.u, w);
    }    std::vector<GraphModuleUtils::EdgeInput> liveEdges;
    for (const auto& edge : input.edges) {
        GraphModuleUtils::EdgeInput le = edge;
        le.w = liveValue("w", static_cast<int>(liveEdges.size()), edge.w);
        liveEdges.push_back(le);
    }
    GraphRenderHelper::buildNodes(scene, m_nodes, m_n, QPointF(470, 170), m_n <= 8 ? 115 : 135, this);
    GraphRenderHelper::buildEdges(scene, m_nodes, m_edges, liveEdges, m_options, input.weighted);
    GraphRenderHelper::registerEdgeSlots(this, m_edges, input.edges, m_options);
    int startX = std::max(140, 470 - m_n * 30);
    GraphRenderHelper::addLabel(scene, "dis", QPointF(35, 348));
    GraphRenderHelper::addLabel(scene, "vis", QPointF(35, 448));
    GraphRenderHelper::addLabel(scene, "pa", QPointF(35, 548));
    for (int i = 1; i <= m_n; ++i) {
        int x = startX + (i - 1) * 60;
        ArrayBlock* distance = new ArrayBlock(i, 0, QString::number(i));
        distance->setCustomValue("∞");
        distance->setPos(x, 365);
        scene->addItem(distance);
        m_distBlocks[i] = distance;
        registerArraySlot(distance, "dis", i);
        ArrayBlock* done = new ArrayBlock(i, 0, QString::number(i));
        done->setCustomValue("0");
        done->setPos(x, 465);
        scene->addItem(done);
        m_doneBlocks[i] = done;
        registerArraySlot(done, "vis", i);
        ArrayBlock* parent = new ArrayBlock(i, 0, QString::number(i));
        parent->setCustomValue("—");
        parent->setPos(x, 565);
        scene->addItem(parent);
        m_parentBlocks[i] = parent;
        registerArraySlot(parent, "pa", i);
    }
    m_pickPointer = new PointerBlock("u", QColor(210, 80, 55));
    m_relaxPointer = new PointerBlock("v", QColor(45, 130, 210));
    m_pickPointer->setOpacity(0);
    m_relaxPointer->setOpacity(0);
    scene->addItem(m_pickPointer);
    scene->addItem(m_relaxPointer);

    buildCandidateRuler();
}

void DijkstraNaiveModule::buildCandidateRuler() {
    const double rulerX = 900.0;
    const double rulerY = 175.0;
    const double cellW = 62.0;
    const double cellH = 40.0;
    const double rowStep = 78.0;

    m_rulerText = createScriptText(m_scene,
        QString("候选尺（按 dist 升序）"));
    m_rulerText->setFont(QFont("Microsoft YaHei", 10, QFont::Bold));
    m_rulerText->setDefaultTextColor(QColor(110, 125, 145));
    m_rulerText->setPos(rulerX - 6.0, rulerY - 62.0);

    for (int i = 1; i <= m_n; ++i) {
        ArrayBlock* block = new ArrayBlock(i, 0, QString::number(i));
        block->setCellSize(cellW, cellH);
        block->setPos(rulerX, rulerY + (i - 1) * rowStep);
        block->color = QColor(252, 252, 254);
        m_scene->addItem(block);
        m_candBlocks[i] = block;
    }

    m_candPointer = new PointerBlock("min", QColor(150, 60, 140));
    m_candPointer->setOpacity(0);
    m_candPointer->setDirection(PointerBlock::Left);
    m_candPointer->setAnchorOffset(QPointF(-10.0, 20.0));
    m_candPointer->setLabelAtTail(10.0);
    m_scene->addItem(m_candPointer);
}

void DijkstraNaiveModule::paintRulerSlot(int index, const QColor& color, bool strike) {
    auto it = m_candBlocks.find(index);
    if (it == m_candBlocks.end()) return;
    it->second->color = color;
    it->second->setStruck(strike);
    it->second->update();
}

void DijkstraNaiveModule::refreshCandidateRuler(const std::vector<int>& dist, const std::vector<bool>& used,
                                                const std::vector<int>& doneOrder) {
    std::vector<std::pair<int, int>> order;
    for (int i = 1; i <= m_n; ++i) {
        if (used[i]) continue;
        order.push_back({dist[i], i});
    }
    std::stable_sort(order.begin(), order.end(),
                     [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
                         return a.first < b.first;
                     });
    m_candOrder.clear();
    for (const auto& entry : order) m_candOrder.push_back(entry.second);

    int slot = 0;
    for (int node : m_candOrder) {
        auto it = m_candBlocks.find(node);
        if (it == m_candBlocks.end()) continue;
        it->second->setPos(900.0, 175.0 + slot * 78.0);
        it->second->setCustomValue(dist[node] >= 1000000000
                                       ? QString("∞")
                                       : QString::number(dist[node]));
        it->second->color = QColor(252, 252, 254);
        it->second->setStruck(false);
        it->second->update();
        ++slot;
    }
    int doneSlot = slot;
    for (int node : doneOrder) {
        auto it = m_candBlocks.find(node);
        if (it == m_candBlocks.end()) continue;
        it->second->setPos(900.0, 175.0 + doneSlot * 78.0);
        it->second->setCustomValue(dist[node] >= 1000000000
                                       ? QString("∞")
                                       : QString::number(dist[node]));
        it->second->color = QColor(214, 226, 218);
        it->second->setStruck(true);
        it->second->update();
        ++doneSlot;
    }
}

std::vector<AnimFrame> DijkstraNaiveModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;
    constexpr int inf = 1000000000;
    int distance[101];
    int parent[101]{};
    bool used[101]{};
    std::fill(std::begin(distance), std::end(distance), inf);
    distance[m_src] = 0;
    bool srcOverridden = false;
    for (int i = 1; i <= m_n; ++i) {
        int override = liveValue("dis", i, -1);
        if (override >= 0) {
            distance[i] = override;
            if (i == m_src) srcOverridden = true;
        }
        int visOverride = liveValue("vis", i, -1);
        if (visOverride > 0) used[i] = true;
    }
    if (!srcOverridden) distance[m_src] = 0;
    m_frames.push_back({ActionType::DJ_RESET, m_src, 0, 0, 0, {}, QString("从源点 %1 初始化距离数组").arg(m_src), QString("dist[%1] = 0，其余为 ∞").arg(m_src)});
    std::vector<int> doneOrder;
    for (int round = 1; round <= m_n; ++round) {
        std::vector<int> candidates;
        for (int i = 1; i <= m_n; ++i) {
            if (!used[i]) candidates.push_back(i);
        }
        std::vector<int> snapshot;
        for (int i = 1; i <= m_n; ++i) {
            snapshot.push_back(distance[i] >= 1000000000 ? -1 : distance[i]);
        }
        m_frames.push_back({ActionType::DJ_CANDIDATE, round, static_cast<int>(candidates.size()), 0, 0, snapshot,
                            QString("第 %1 轮：列出全部未确定点，候选尺按 dist 升序排列").arg(round),
                            QString("候选尺：未确定点共 %1 个").arg(candidates.size())});
        int u = 0;
        for (int i = 1; i <= m_n; ++i) {
            if (used[i]) continue;
            int oldBest = u ? distance[u] : 1000000000;
            bool choose = u == 0 || distance[i] < distance[u];
            if (choose) u = i;
            m_frames.push_back({ActionType::DJ_SCAN, u, i, distance[i], oldBest, {0, choose ? 1 : 0}, QString("扫描未确定点 %1，比较当前最小距离").arg(i), QString("min(%1, %2) = %3").arg(oldBest == 1000000000 ? QString("∞") : QString::number(oldBest)).arg(distance[i] == 1000000000 ? QString("∞") : QString::number(distance[i])).arg(distance[u] == 1000000000 ? QString("∞") : QString::number(distance[u]))});
        }
        if (!u || distance[u] == 1000000000) {
            for (int i = 1; i <= m_n; ++i) if (!used[i]) m_frames.push_back({ActionType::DJ_UNREACHABLE, i, 0, 0, 0, {}, QString("节点 %1 与源点不连通").arg(i), QString("dist[%1] = ∞").arg(i)});
            break;
        }
        m_frames.push_back({ActionType::DJ_CHOOSE, u, 0, distance[u], 0, {},
                            QString("候选尺最上方就是 dist 最小的点 %1，dist = %2").arg(u).arg(distance[u]),
                            QString("选 u = %1（dist 最小）").arg(u)});
        used[u] = true;
        doneOrder.push_back(u);
        m_frames.push_back({ActionType::DJ_SELECT, u, round, distance[u], 0, {}, QString("确定节点 %1 的最短距离").arg(u), QString("dist[%1] = %2，加入已确定集合").arg(u).arg(distance[u])});
        for (int edge = m_head[u]; edge; edge = m_next[edge]) {
            int v = m_to[edge];
            int weight = m_weight[edge];
            m_frames.push_back({ActionType::DJ_SCAN, u, v, weight, distance[v], {1, used[v] ? 1 : 0}, QString("沿边 %1 — %2 检查候选路径").arg(u).arg(v), QString("dist[%1] = %2，边权 w = %3").arg(v).arg(distance[v] == 1000000000 ? QString("∞") : QString::number(distance[v])).arg(weight)});
            if (used[v]) continue;
            int candidate = distance[u] + weight;
            if (candidate < distance[v]) {
                int old = distance[v];
                distance[v] = candidate;
                parent[v] = u;
                m_frames.push_back({ActionType::DJ_RELAX, u, v, candidate, old, {parent[v]}, QString("发现更短路径，更新 dist[%1]").arg(v), QString("dist[%1] = min(%2, %3 + %4) = %5").arg(v).arg(old == 1000000000 ? QString("∞") : QString::number(old)).arg(distance[u]).arg(weight).arg(candidate)});
            } else {
                m_frames.push_back({ActionType::DJ_RELAX_KEEP, u, v, candidate, distance[v], {},
                                    QString("候选值 %1 不小于现有 dist[%2] = %3，保持不变")
                                        .arg(candidate).arg(v).arg(distance[v] == 1000000000 ? QString("∞") : QString::number(distance[v])),
                                    QString("dist[%1] 不变").arg(v)});
            }
        }
    }
    m_frames.push_back({ActionType::DJ_FINISH, m_n, 0, 0, 0, {}, "Dijkstra 扫描完成", "非负边权保证每次选点后距离永久确定"});
    return m_frames;
}

void DijkstraNaiveModule::renderFrame(const AnimFrame& frame) {
    if (!m_mathText) return;
    setScriptText(m_mathText, frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    if (frame.type == ActionType::DJ_RESET) {
        for (auto& pair : m_edges) pair.second->setColor(Qt::black);
        for (int i = 1; i <= m_n; ++i) {
            m_nodes[i]->color = Qt::white;
            m_nodes[i]->update();
            m_distBlocks[i]->setCustomValue("∞");
            m_parentBlocks[i]->setCustomValue("—");
            m_distBlocks[i]->color = Qt::white;
            m_parentBlocks[i]->color = Qt::white;
            m_doneBlocks[i]->setCustomValue("0");
            m_doneBlocks[i]->color = Qt::white;
            m_distBlocks[i]->update();
            m_parentBlocks[i]->update();
            m_doneBlocks[i]->update();
        }
        for (auto& pair : m_edges) pair.second->setColor(Qt::black);
        m_pickPointer->setOpacity(0);
        m_relaxPointer->setOpacity(0);
        m_pickPointer->clearAnchor();
        m_relaxPointer->clearAnchor();
        if (m_candPointer) {
            m_candPointer->setOpacity(0);
            m_candPointer->clearAnchor();
        }
        for (int i = 1; i <= m_n; ++i) {
            auto it = m_candBlocks.find(i);
            if (it == m_candBlocks.end()) continue;
            it->second->setPos(900.0, 175.0 + (i - 1) * 78.0);
            it->second->setCustomValue(i == frame.u ? QString("0") : QString("∞"));
            it->second->color = QColor(252, 252, 254);
            it->second->setStruck(false);
            it->second->update();
        }
        m_distBlocks[frame.u]->setCustomValue("0");
        m_distBlocks[frame.u]->color = QColor(170, 220, 245);
        m_distBlocks[frame.u]->update();
    } else if (frame.type == ActionType::DJ_SCAN) {
        if (frame.group.size() > 0 && frame.group[0] == 1) {
            GraphEdge* edge = GraphRenderHelper::findEdge(m_edges, frame.u, frame.v);
            if (frame.group.size() > 1 && frame.group[1]) {
                if (edge && edge->color != QColor(70, 175, 125)) {
                    edge->setColor(QColor(195, 200, 208));
                    playEdgeSweep(edge, QColor(195, 200, 208), 300);
                }
            } else {
                for (auto& pair : m_edges) if (pair.second->color == QColor(235, 155, 55)) pair.second->setColor(Qt::black);
                for (int i = 1; i <= m_n; ++i) {
                    if (m_doneBlocks[i]->customValue == "0" && m_nodes[i]->color == QColor(255, 220, 165)) {
                        m_nodes[i]->color = m_distBlocks[i]->customValue == "∞" ? Qt::white : QColor(155, 210, 245);
                        m_nodes[i]->update();
                    }
                }
                if (edge && edge->color != QColor(70, 175, 125)) {
                    edge->setColor(QColor(235, 155, 55));
                    playEdgeTravel(edge, QColor(235, 155, 55), 460);
                }
                m_nodes[frame.v]->color = QColor(255, 220, 165);
                m_nodes[frame.v]->update();
                playNodeWake(m_nodes[frame.v], QColor(255, 200, 110), 32.0, 460);
                m_relaxPointer->setOpacity(1);
                retargetPointerToNode(m_relaxPointer, m_nodes[frame.v]);
            }
        } else {
            for (int i = 1; i <= m_n; ++i) {
                if (m_doneBlocks[i]->customValue == "0" && m_nodes[i]->color == QColor(255, 220, 165)) {
                    m_nodes[i]->color = m_distBlocks[i]->customValue == "∞" ? Qt::white : QColor(155, 210, 245);
                    m_nodes[i]->update();
                }
            }
            m_nodes[frame.v]->color = QColor(255, 220, 165);
            m_nodes[frame.v]->update();
            playNodeWake(m_nodes[frame.v], QColor(255, 200, 110), 28.0, 420);
            if (frame.group.size() > 1 && frame.group[1]) {
                m_pickPointer->setOpacity(1);
                retargetPointerToNode(m_pickPointer, m_nodes[frame.v]);
            }
        }
    } else if (frame.type == ActionType::DJ_CANDIDATE) {
        std::vector<int> dist(m_n + 1, 1000000000);
        std::vector<bool> used(m_n + 1, false);
        for (int i = 1; i <= m_n; ++i) {
            const int idx = i - 1;
            if (idx < static_cast<int>(frame.group.size())) {
                dist[i] = frame.group[idx] < 0 ? 1000000000 : frame.group[idx];
            }
            if (m_doneBlocks.count(i) && m_doneBlocks[i]->customValue == "1") used[i] = true;
        }
        std::vector<int> doneOrder;
        for (int i = 1; i <= m_n; ++i) if (used[i]) doneOrder.push_back(i);
        refreshCandidateRuler(dist, used, doneOrder);
        if (m_rulerText) {
            setScriptText(m_rulerText, QString("第 %1 轮候选尺：未确定点按 dist 升序").arg(frame.u));
            m_rulerText->setDefaultTextColor(QColor(110, 125, 145));
        }
        if (!m_candOrder.empty()) {
            auto it = m_candBlocks.find(m_candOrder.front());
            if (it != m_candBlocks.end()) {
                m_candPointer->setOpacity(1);
                retargetPointerTo(m_candPointer, it->second, QPointF(-10.0, 20.0));
                it->second->color = QColor(255, 214, 140);
                it->second->update();
                if (m_animated) {
                    playScalePulse(it->second, 1.18, 260);
                    playCellRipple(it->second, QColor(255, 200, 110), 0.9, 480);
                }
            }
        }
    } else if (frame.type == ActionType::DJ_CHOOSE) {
        const int u = frame.u;
        if (m_nodes.count(u)) {
            m_nodes[u]->color = QColor(255, 214, 140);
            m_nodes[u]->update();
            playNodeWake(m_nodes[u], QColor(255, 200, 110), 36.0, 520);
        }
        if (m_pickPointer->opacity() < 0.05) {
            m_pickPointer->setOpacity(1);
            if (m_nodes.count(u)) retargetPointerToNode(m_pickPointer, m_nodes[u]);
        }
        if (m_mathText) {
            setScriptText(m_mathText, QString("候选尺顶端 dist = %1 的点就是 %2")
                               .arg(frame.val1).arg(u));
            m_mathText->setDefaultTextColor(QColor(190, 110, 40));
        }
    } else if (frame.type == ActionType::DJ_RELAX_KEEP) {
        const int u = frame.u, v = frame.v;
        GraphEdge* edge = GraphRenderHelper::findEdge(m_edges, u, v);
        if (edge) {
            edge->setColor(QColor(198, 190, 205));
            playEdgeSweep(edge, QColor(198, 190, 205), 340);
        }
        if (m_distBlocks.count(v)) {
            m_distBlocks[v]->color = QColor(155, 210, 245);
            m_distBlocks[v]->update();
            playCellRipple(m_distBlocks[v], QColor(150, 160, 190), 0.7, 420);
        }
    } else if (frame.type == ActionType::DJ_SELECT) {
        m_nodes[frame.u]->color = QColor(150, 220, 190);
        m_nodes[frame.u]->update();
        m_doneBlocks[frame.u]->setCustomValue("1");
        m_doneBlocks[frame.u]->color = QColor(150, 220, 190);
        m_doneBlocks[frame.u]->update();
        playDiscover(m_nodes[frame.u], QColor(120, 220, 175), 52.0, 700);
        m_pickPointer->setOpacity(1);
        retargetPointerToNode(m_pickPointer, m_nodes[frame.u]);
    } else if (frame.type == ActionType::DJ_RELAX) {
        m_distBlocks[frame.v]->setCustomValue(QString::number(frame.val1));
        m_distBlocks[frame.v]->color = QColor(155, 220, 245);
        m_distBlocks[frame.v]->update();
        m_parentBlocks[frame.v]->setCustomValue(QString::number(frame.u));
        m_parentBlocks[frame.v]->color = QColor(205, 220, 240);
        m_parentBlocks[frame.v]->update();
        m_nodes[frame.v]->color = QColor(155, 220, 245);
        m_nodes[frame.v]->update();
        GraphEdge* edge = GraphRenderHelper::findEdge(m_edges, frame.u, frame.v);
        if (edge) {
            edge->setColor(QColor(70, 175, 125));
            playEdgeDiscover(edge, m_nodes[frame.v], QColor(70, 175, 125), 520);
        }
        playValueFlight(m_nodes[frame.u]->pos(), m_distBlocks[frame.v], frame.val1, QColor(155, 220, 245), true);
    } else if (frame.type == ActionType::DJ_UNREACHABLE) {
        m_nodes[frame.u]->color = QColor(205, 210, 218);
        m_nodes[frame.u]->update();
        m_distBlocks[frame.u]->color = QColor(220, 222, 226);
        m_distBlocks[frame.u]->update();
        playNodePop(m_nodes[frame.u], 0.94, 260);
    } else if (frame.type == ActionType::DJ_FINISH) {
        m_relaxPointer->setOpacity(0);
        for (auto& kv : m_nodes) playNodePop(kv.second, 1.10, 260);
    }
}

void DijkstraNaiveModule::reset() {
    m_frames.clear();
    m_nodes.clear();
    m_edges.clear();
    m_distBlocks.clear();
    m_doneBlocks.clear();
    m_parentBlocks.clear();
    m_candBlocks.clear();
    m_candOrder.clear();
    m_edgeList.clear();
    m_pickPointer = nullptr;
    m_relaxPointer = nullptr;
    m_candPointer = nullptr;
    m_mathText = nullptr;
    m_rulerText = nullptr;
    m_n = 0;
    m_m = 0;
    m_src = 1;
    m_edgeCount = 0;
    m_valid = false;
    std::fill(std::begin(m_head), std::end(m_head), 0);
    std::fill(std::begin(m_to), std::end(m_to), 0);
    std::fill(std::begin(m_next), std::end(m_next), 0);
    std::fill(std::begin(m_weight), std::end(m_weight), 0);
}
void DijkstraNaiveModule::releaseSceneItems() {
    m_nodes.clear();
    m_edges.clear();
    m_distBlocks.clear();
    m_doneBlocks.clear();
    m_parentBlocks.clear();
    m_candBlocks.clear();
    m_candOrder.clear();
    m_pickPointer = nullptr;
    m_relaxPointer = nullptr;
    m_candPointer = nullptr;
    m_mathText = nullptr;
    m_rulerText = nullptr;
}