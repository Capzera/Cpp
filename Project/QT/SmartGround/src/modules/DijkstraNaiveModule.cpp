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

QString DijkstraNaiveModule::getModuleName() const { return "朴素 Dijkstra 最短路"; }
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
    m_mathText = scene->addText(m_valid ? "朴素 Dijkstra：每轮扫描全体未确定点，选择 dist 最小者" : "输入格式：n m src，随后输入 m 条非负权边 u v w");
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
    for (int round = 1; round <= m_n; ++round) {
        int u = 0;
        for (int i = 1; i <= m_n; ++i) {
            if (used[i]) continue;
            int oldBest = u ? distance[u] : inf;
            bool choose = u == 0 || distance[i] < distance[u];
            if (choose) u = i;
            m_frames.push_back({ActionType::DJ_SCAN, u, i, distance[i], oldBest, {0, choose ? 1 : 0}, QString("扫描未确定点 %1，比较当前最小距离").arg(i), QString("min(%1, %2) = %3").arg(oldBest == inf ? QString("∞") : QString::number(oldBest)).arg(distance[i] == inf ? QString("∞") : QString::number(distance[i])).arg(distance[u] == inf ? QString("∞") : QString::number(distance[u]))});
        }
        if (!u || distance[u] == inf) {
            for (int i = 1; i <= m_n; ++i) if (!used[i]) m_frames.push_back({ActionType::DJ_UNREACHABLE, i, 0, 0, 0, {}, QString("节点 %1 与源点不连通").arg(i), QString("dist[%1] = ∞").arg(i)});
            break;
        }
        used[u] = true;
        m_frames.push_back({ActionType::DJ_SELECT, u, round, distance[u], 0, {}, QString("确定节点 %1 的最短距离").arg(u), QString("dist[%1] = %2，加入已确定集合").arg(u).arg(distance[u])});
        for (int edge = m_head[u]; edge; edge = m_next[edge]) {
            int v = m_to[edge];
            int weight = m_weight[edge];
            m_frames.push_back({ActionType::DJ_SCAN, u, v, weight, distance[v], {1, used[v] ? 1 : 0}, QString("沿边 %1 — %2 检查候选路径").arg(u).arg(v), QString("dist[%1] = %2，边权 w = %3").arg(v).arg(distance[v] == inf ? QString("∞") : QString::number(distance[v])).arg(weight)});
            if (used[v]) continue;
            int candidate = distance[u] + weight;
            if (candidate < distance[v]) {
                int old = distance[v];
                distance[v] = candidate;
                parent[v] = u;
                m_frames.push_back({ActionType::DJ_RELAX, u, v, candidate, old, {parent[v]}, QString("发现更短路径，更新 dist[%1]").arg(v), QString("dist[%1] = min(%2, %3 + %4) = %5").arg(v).arg(old == inf ? QString("∞") : QString::number(old)).arg(distance[u]).arg(weight).arg(candidate)});
            }
        }
    }
    m_frames.push_back({ActionType::DJ_FINISH, m_n, 0, 0, 0, {}, "Dijkstra 扫描完成", "非负边权保证每次选点后距离永久确定"});
    return m_frames;
}

void DijkstraNaiveModule::renderFrame(const AnimFrame& frame) {
    if (!m_mathText) return;
    m_mathText->setPlainText(frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
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
        m_distBlocks[frame.u]->setCustomValue("0");
        m_distBlocks[frame.u]->color = QColor(170, 220, 245);
        m_distBlocks[frame.u]->update();
    } else if (frame.type == ActionType::DJ_SCAN) {
        if (frame.group.size() > 0 && frame.group[0] == 1) {
            GraphEdge* edge = GraphRenderHelper::findEdge(m_edges, frame.u, frame.v);
            if (frame.group.size() > 1 && frame.group[1]) {
                if (edge && edge->color != QColor(70, 175, 125)) edge->setColor(QColor(195, 200, 208));
            } else {
                for (auto& pair : m_edges) if (pair.second->color == QColor(235, 155, 55)) pair.second->setColor(Qt::black);
                for (int i = 1; i <= m_n; ++i) {
                    if (m_doneBlocks[i]->customValue == "0" && m_nodes[i]->color == QColor(255, 220, 165)) {
                        m_nodes[i]->color = m_distBlocks[i]->customValue == "∞" ? Qt::white : QColor(155, 210, 245);
                        m_nodes[i]->update();
                    }
                }
                if (edge && edge->color != QColor(70, 175, 125)) edge->setColor(QColor(235, 155, 55));
                m_nodes[frame.v]->color = QColor(255, 220, 165);
                m_nodes[frame.v]->update();
                m_relaxPointer->setOpacity(1);
                m_relaxPointer->setAnchorNode(m_nodes[frame.v]);
                m_relaxPointer->syncToAnchor();
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
            if (frame.group.size() > 1 && frame.group[1]) {
                m_pickPointer->setOpacity(1);
                m_pickPointer->setAnchorNode(m_nodes[frame.v]);
                m_pickPointer->syncToAnchor();
            }
        }
    } else if (frame.type == ActionType::DJ_SELECT) {
        m_nodes[frame.u]->color = QColor(150, 220, 190);
        m_nodes[frame.u]->update();
        m_doneBlocks[frame.u]->setCustomValue("1");
        m_doneBlocks[frame.u]->color = QColor(150, 220, 190);
        m_doneBlocks[frame.u]->update();
        m_pickPointer->setOpacity(1);
        m_pickPointer->setAnchorNode(m_nodes[frame.u]);
        m_pickPointer->syncToAnchor();
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
        if (edge) edge->setColor(QColor(70, 175, 125));
    } else if (frame.type == ActionType::DJ_UNREACHABLE) {
        m_nodes[frame.u]->color = QColor(205, 210, 218);
        m_nodes[frame.u]->update();
        m_distBlocks[frame.u]->color = QColor(220, 222, 226);
        m_distBlocks[frame.u]->update();
    } else if (frame.type == ActionType::DJ_FINISH) {
        m_relaxPointer->setOpacity(0);
    }
}

void DijkstraNaiveModule::reset() {
    m_frames.clear();
    m_nodes.clear();
    m_edges.clear();
    m_distBlocks.clear();
    m_doneBlocks.clear();
    m_parentBlocks.clear();
    m_edgeList.clear();
    m_pickPointer = nullptr;
    m_relaxPointer = nullptr;
    m_mathText = nullptr;
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
    m_pickPointer = nullptr;
    m_relaxPointer = nullptr;
    m_mathText = nullptr;
}