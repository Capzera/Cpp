#include "KruskalModule.h"
#include "GraphModuleUtils.h"
#include "GraphRenderHelper.h"
#include <QFont>
#include <algorithm>
#include <cmath>

KruskalModule::KruskalModule() : m_edgePointer(nullptr), m_mathText(nullptr), m_n(0), m_m(0), m_valid(false) {}

QString KruskalModule::getModuleName() const { return "Kruskal 最小生成树"; }
QString KruskalModule::getDefaultInput() const { return "6 9\n1 2 4\n1 3 3\n2 3 2\n2 4 5\n3 4 3\n3 5 6\n4 5 1\n4 6 7\n5 6 8"; }
bool KruskalModule::supportsGraphOptions() const { return true; }
bool KruskalModule::supportsDirected() const { return false; }
bool KruskalModule::supportsBidirectional() const { return true; }
GraphModuleUtils::GraphOptions KruskalModule::getGraphOptions() const { return m_options; }
void KruskalModule::setGraphOptions(const GraphModuleUtils::GraphOptions& options) { m_options = options; }
QString KruskalModule::getInputHint() const {
    return "第一行输入两个正整数：顶点数 n、边数 m\n"
           "接下来 m 行，每行三个整数 u, v, w，表示一条无向边 u - v，权值为 w\n"
           "本模块固定按无向图处理，可勾选双向箭头显示";
}
std::map<int, QPointF> KruskalModule::captureNodePositions() const {
    return GraphRenderHelper::captureNodePositions(m_nodes);
}
void KruskalModule::restoreNodePositions(const std::map<int, QPointF>& positions) {
    GraphRenderHelper::restoreNodePositions(m_nodes, positions);
}

void KruskalModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;
    GraphModuleUtils::GraphInput input;
    GraphModuleUtils::GraphOptions options = m_options;
    options.directed = false;
    m_valid = GraphModuleUtils::parse(inputData, false, options, input);
    m_mathText = createScriptText(scene, m_valid ? "Kruskal：按边权从小到大扫描，用并查集避免成环" : "输入格式：n m，随后输入 m 条无向边 u v w");
    m_mathText->setFont(QFont("Consolas", 14, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(140, 18);
    if (!m_valid) return;
    m_n = input.n;
    m_m = input.m;
    int liveIdx = 0;
    for (const auto& edge : input.edges) {
        int w = liveValue("w", liveIdx++, edge.w);
        m_edgeList.push_back({edge.u, edge.v, w});
    }
    std::stable_sort(m_edgeList.begin(), m_edgeList.end(), [](const auto& a, const auto& b) {
        if (std::get<2>(a) != std::get<2>(b)) return std::get<2>(a) < std::get<2>(b);
        if (std::get<0>(a) != std::get<0>(b)) return std::get<0>(a) < std::get<0>(b);
        return std::get<1>(a) < std::get<1>(b);
    });
    std::vector<GraphModuleUtils::EdgeInput> liveEdges;
    for (const auto& edge : input.edges) {
        GraphModuleUtils::EdgeInput le = edge;
        le.w = liveValue("w", static_cast<int>(liveEdges.size()), edge.w);
        liveEdges.push_back(le);
    }
    GraphRenderHelper::buildNodes(scene, m_nodes, m_n, QPointF(470, 165), m_n <= 8 ? 110 : 130, this);
    GraphRenderHelper::buildEdges(scene, m_nodes, m_edges, liveEdges, options, input.weighted);
    GraphRenderHelper::registerEdgeSlots(this, m_edges, input.edges, options);
    GraphRenderHelper::addLabel(scene, "e[]", QPointF(25, 348));
    GraphRenderHelper::addLabel(scene, "pa", QPointF(25, 478));
    int edgeStartX = std::max(20, 470 - m_m * 30);
    for (int i = 0; i < m_m; ++i) {
        auto [u, v, weight] = m_edgeList[i];
        ArrayBlock* edgeBlock = new ArrayBlock(i, weight, QString::number(i));
        edgeBlock->setPos(edgeStartX + i * 60, 370);
        scene->addItem(edgeBlock);
        m_edgeBlocks.push_back(edgeBlock);
    }
    int nodeStartX = std::max(140, 470 - m_n * 30);
    for (int i = 1; i <= m_n; ++i) {
        ArrayBlock* block = new ArrayBlock(i, i, QString::number(i));
        block->setPos(nodeStartX + (i - 1) * 60, 500);
        scene->addItem(block);
        m_parentBlocks[i] = block;
    }
    m_edgePointer = new PointerBlock("edge", QColor(210, 80, 55));
    m_edgePointer->setOpacity(0);
    scene->addItem(m_edgePointer);
}

std::vector<AnimFrame> KruskalModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;
    std::vector<std::tuple<int,int,int>> edges = m_edgeList;
    std::stable_sort(edges.begin(), edges.end(), [](const auto& a, const auto& b) {
        if (std::get<2>(a) != std::get<2>(b)) return std::get<2>(a) < std::get<2>(b);
        if (std::get<0>(a) != std::get<0>(b)) return std::get<0>(a) < std::get<0>(b);
        return std::get<1>(a) < std::get<1>(b);
    });
    std::vector<int> parent(m_n + 1, 0);
    std::vector<int> size(m_n + 1, 1);
    for (int i = 1; i <= m_n; ++i) parent[i] = i;
    m_frames.push_back({ActionType::KR_RESET, 0, 0, 0, 0, {}, "每个节点初始化为独立集合", "parent[i] = i"});
    int accepted = 0, total = 0;
    for (int i = 0; i < static_cast<int>(edges.size()) && accepted < m_n - 1; ++i) {
        auto [u, v, weight] = edges[i];
        m_frames.push_back({ActionType::KR_CHECK, u, v, weight, i, {},
                            QString("检查第 %1 条边 %2—%3，权值 %4").arg(i + 1).arg(u).arg(v).arg(weight),
                            QString("边 %1：%2 — %3，w = %4").arg(i + 1).arg(u).arg(v).arg(weight)});

        std::vector<int> pathU;
        int x = u;
        int guard = 0;
        while (parent[x] != x && guard++ < 64) {
            pathU.push_back(x);
            x = parent[x];
            m_frames.push_back({ActionType::KR_FIND_STEP, x, 0, parent[x], 0,
                                {pathU.begin(), pathU.end()},
                                QString("find(%1)：沿 parent 链从 %2 跳到 %3").arg(u).arg(pathU.back()).arg(x),
                                QString("parent[%1] = %2").arg(pathU.back()).arg(x)});
        }
        pathU.push_back(x);
        const int rootU = x;
        m_frames.push_back({ActionType::KR_FIND_ROOT, rootU, u, 1, 0,
                            {pathU.begin(), pathU.end()},
                            QString("find(%1) 的根是 %2").arg(u).arg(rootU),
                            QString("root(%1) = %2").arg(u).arg(rootU)});

        std::vector<int> pathV;
        x = v;
        guard = 0;
        while (parent[x] != x && guard++ < 64) {
            pathV.push_back(x);
            x = parent[x];
            m_frames.push_back({ActionType::KR_FIND_STEP, x, 0, parent[x], 0,
                                {pathV.begin(), pathV.end()},
                                QString("find(%1)：沿 parent 链从 %2 跳到 %3").arg(v).arg(pathV.back()).arg(x),
                                QString("parent[%1] = %2").arg(pathV.back()).arg(x)});
        }
        pathV.push_back(x);
        const int rootV = x;
        m_frames.push_back({ActionType::KR_FIND_ROOT, rootV, v, 1, 0,
                            {pathV.begin(), pathV.end()},
                            QString("find(%1) 的根是 %2").arg(v).arg(rootV),
                            QString("root(%1) = %2").arg(v).arg(rootV)});

        if (rootU == rootV) {
            std::vector<int> ring;
            ring.push_back(u);
            for (size_t k = 0; k < pathU.size(); ++k) ring.push_back(pathU[k]);
            m_frames.push_back({ActionType::KR_CYCLE, u, v, weight, i, ring,
                                QString("两个根相同（都是 %1），这条边会和树上路径构成一个环，必须舍弃").arg(rootU),
                                QString("root(%1) = root(%2) ⇒ 成环，舍弃").arg(u).arg(v)});
            continue;
        }

        int child = rootV;
        int root = rootU;
        if (size[rootU] < size[rootV]) std::swap(root, child);
        parent[child] = root;
        size[root] += size[child];
        ++accepted;
        total += weight;
        std::vector<int> group{root, child, child, root};
        for (int node = 1; node <= m_n; ++node) {
            int probe = node;
            int g2 = 0;
            while (parent[probe] != probe && g2++ < 64) probe = parent[probe];
            if (probe == root) group.push_back(node);
        }
        m_frames.push_back({ActionType::KR_ACCEPT, u, v, weight, i, group,
                            QString("接受边 %1—%2，当前生成树权值 %3").arg(u).arg(v).arg(total),
                            QString("合并集合，MST 权值 = %1 + %2 = %3").arg(total - weight).arg(weight).arg(total)});
    }
    if (accepted == m_n - 1) m_frames.push_back({ActionType::KR_FINISH, accepted, m_n, total, 0, {}, QString("最小生成树完成，共接受 %1 条边").arg(accepted), QString("MST 总权值 = %1").arg(total)});
    else m_frames.push_back({ActionType::KR_DISCONNECTED, accepted, m_n, total, 0, {}, QString("图不连通，只能选出 %1/%2 条生成边").arg(accepted).arg(m_n - 1), "不存在覆盖全部节点的生成树"});
    return m_frames;
}

void KruskalModule::paintFindPath(const std::vector<int>& path, const QColor& color) {
    for (size_t k = 0; k + 1 < path.size(); ++k) {
        GraphEdge* edge = GraphRenderHelper::findEdge(m_edges, path[k], path[k + 1]);
        if (!edge) continue;
        edge->setColor(color);
        edge->setPenWidth(3.4);
        edge->update();
    }
    for (int node : path) {
        if (!m_nodes.count(node)) continue;
        m_nodes[node]->color = color;
        m_nodes[node]->setHighlightColor(color);
        m_nodes[node]->update();
    }
}

void KruskalModule::paintCycleRing(int u, int v) {
    GraphEdge* edge = GraphRenderHelper::findEdge(m_edges, u, v);
    if (edge) {
        edge->setColor(QColor(228, 82, 74));
        edge->setPenWidth(4.0);
        edge->update();
        playEdgeTravel(edge, QColor(228, 82, 74), 520);
    }
}

void KruskalModule::renderFrame(const AnimFrame& frame) {
    if (!m_mathText) return;
    setScriptText(m_mathText, frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    if (frame.type == ActionType::KR_RESET) {
        m_componentColors.clear();
        for (int i = 1; i <= m_n; ++i) {
            m_nodes[i]->color = Qt::white;
            m_nodes[i]->update();
            m_parentBlocks[i]->value = i;
            m_parentBlocks[i]->color = Qt::white;
            m_parentBlocks[i]->update();
        }
        for (auto& pair : m_edges) {
            pair.second->setColor(Qt::black);
            pair.second->setPenWidth(2.0);
            pair.second->setPathLabel(QString());
            pair.second->update();
        }
        for (ArrayBlock* block : m_edgeBlocks) { block->color = Qt::white; block->update(); }
        m_edgePointer->setOpacity(0);
    } else if (frame.type == ActionType::KR_CHECK) {
        for (ArrayBlock* block : m_edgeBlocks) if (block->color == QColor(255, 205, 125)) { block->color = Qt::white; block->update(); }
        for (int i = 1; i <= m_n; ++i) {
            m_nodes[i]->color = m_componentColors.count(i) ? m_componentColors[i] : Qt::white;
            m_nodes[i]->update();
        }
        int index = frame.val2;
        if (index >= 0 && index < static_cast<int>(m_edgeBlocks.size())) {
            m_edgeBlocks[index]->color = QColor(255, 205, 125);
            m_edgeBlocks[index]->update();
            m_edgePointer->setOpacity(1);
            playPointerMove(m_edgePointer, QPointF(m_edgeBlocks[index]->pos().x() + 30, m_edgeBlocks[index]->pos().y() + 42));
        }
        if (m_nodes.count(frame.u)) {
            m_nodes[frame.u]->color = QColor(255, 205, 125);
            m_nodes[frame.u]->update();
            playNodeWake(m_nodes[frame.u], QColor(255, 190, 90), 30.0, 440);
        }
        if (m_nodes.count(frame.v)) {
            m_nodes[frame.v]->color = QColor(155, 210, 245);
            m_nodes[frame.v]->update();
            playNodeWake(m_nodes[frame.v], QColor(140, 190, 245), 30.0, 440);
        }
        GraphEdge* probe = GraphRenderHelper::findEdge(m_edges, frame.u, frame.v);
        if (probe) {
            probe->setColor(QColor(255, 205, 125));
            playEdgeTravel(probe, QColor(255, 205, 125), 420);
        }
    } else if (frame.type == ActionType::KR_FIND_STEP) {
        const std::vector<int>& path = frame.group;
        for (int node : path) {
            if (!m_nodes.count(node)) continue;
            m_nodes[node]->color = QColor(214, 180, 245);
            m_nodes[node]->setHighlightColor(QColor(214, 180, 245));
            m_nodes[node]->update();
            if (m_animated) playNodeRipple(m_nodes[node], QColor(190, 150, 235), 30.0, 420);
        }
        if (path.size() >= 2) {
            GraphEdge* edge = GraphRenderHelper::findEdge(m_edges, path[path.size() - 2], path[path.size() - 1]);
            if (edge) {
                edge->setColor(QColor(158, 122, 214));
                edge->setPenWidth(3.2);
                edge->update();
                playEdgeTravel(edge, QColor(158, 122, 214), 440);
            }
        }
    } else if (frame.type == ActionType::KR_FIND_ROOT) {
        const std::vector<int>& path = frame.group;
        paintFindPath(path, QColor(158, 122, 214));
        if (m_nodes.count(frame.u)) {
            m_nodes[frame.u]->color = QColor(150, 214, 170);
            m_nodes[frame.u]->setHighlightColor(QColor(150, 214, 170));
            m_nodes[frame.u]->update();
            if (m_animated) {
                playScalePulse(m_nodes[frame.u], 1.24, 260);
                playNodeRipple(m_nodes[frame.u], QColor(150, 214, 170), 44.0, 580);
            }
        }
    } else if (frame.type == ActionType::KR_CYCLE) {
        const std::vector<int>& ring = frame.group;
        for (int node : ring) {
            if (!m_nodes.count(node)) continue;
            m_nodes[node]->color = QColor(245, 155, 155);
            m_nodes[node]->setHighlightColor(QColor(228, 82, 74));
            m_nodes[node]->update();
            playNodePop(m_nodes[node], 0.94, 280);
        }
        for (size_t k = 0; k + 1 < ring.size(); ++k) {
            GraphEdge* edge = GraphRenderHelper::findEdge(m_edges, ring[k], ring[k + 1]);
            if (!edge) continue;
            edge->setColor(QColor(228, 82, 74));
            edge->setPenWidth(3.6);
            edge->update();
        }
        paintCycleRing(frame.u, frame.v);
        int index = frame.val2;
        if (index >= 0 && index < static_cast<int>(m_edgeBlocks.size())) {
            m_edgeBlocks[index]->color = QColor(240, 170, 165);
            m_edgeBlocks[index]->update();
            playScalePulse(m_edgeBlocks[index], 0.92, 260);
        }
    } else if (frame.type == ActionType::KR_ACCEPT) {
        GraphEdge* edge = GraphRenderHelper::findEdge(m_edges, frame.u, frame.v);
        if (edge) {
            edge->setColor(QColor(55, 175, 115));
            if (m_nodes.count(frame.u) && m_nodes.count(frame.v)) {
                playEdgeDiscover(edge, m_nodes[frame.v], QColor(55, 175, 115), 560);
            } else {
                playEdgeTravel(edge, QColor(55, 175, 115), 560);
            }
        }
        if (frame.group.size() >= 4) {
            int child = frame.group[2], root = frame.group[3];
            m_parentBlocks[child]->value = root;
            m_parentBlocks[child]->color = QColor(165, 220, 185);
            m_parentBlocks[child]->update();
            QColor componentColor = QColor::fromHsv((root * 55 + 145) % 360, 75, 245);
            for (int i = 4; i < static_cast<int>(frame.group.size()); ++i) {
                m_nodes[frame.group[i]]->color = componentColor;
                m_componentColors[frame.group[i]] = componentColor;
                m_nodes[frame.group[i]]->update();
                playNodePop(m_nodes[frame.group[i]], 1.14, 300);
            }
        }
        int index = frame.val2;
        if (index >= 0 && index < static_cast<int>(m_edgeBlocks.size())) {
            m_edgeBlocks[index]->color = QColor(155, 220, 175);
            m_edgeBlocks[index]->update();
            playScalePulse(m_edgeBlocks[index], 1.20, 260);
        }
    } else if (frame.type == ActionType::KR_REJECT) {
        GraphEdge* edge = GraphRenderHelper::findEdge(m_edges, frame.u, frame.v);
        if (edge && edge->color != QColor(55, 175, 115)) {
            edge->setColor(QColor(195, 200, 208));
            playEdgeSweep(edge, QColor(195, 200, 208), 320);
        }
        for (int i = 1; i <= m_n; ++i) {
            m_nodes[i]->color = m_componentColors.count(i) ? m_componentColors[i] : Qt::white;
            m_nodes[i]->update();
        }
        int index = frame.val2;
        if (index >= 0 && index < static_cast<int>(m_edgeBlocks.size())) {
            m_edgeBlocks[index]->color = QColor(215, 218, 222);
            m_edgeBlocks[index]->update();
            playScalePulse(m_edgeBlocks[index], 0.92, 260);
        }
    } else if (frame.type == ActionType::KR_FINISH) {
        m_edgePointer->setOpacity(0);
        for (auto& kv : m_nodes) playNodePop(kv.second, 1.10, 260);
    } else if (frame.type == ActionType::KR_DISCONNECTED) {
        m_edgePointer->setOpacity(0);
        for (auto& pair : m_nodes) if (pair.second->color == Qt::white) pair.second->color = QColor(225, 205, 205);
        for (auto& pair : m_nodes) pair.second->update();
    }
}

void KruskalModule::reset() {
    m_frames.clear();
    m_nodes.clear();
    m_edges.clear();
    m_edgeBlocks.clear();
    m_parentBlocks.clear();
    m_componentColors.clear();
    m_edgePointer = nullptr;
    m_mathText = nullptr;
    m_edgeList.clear();
    m_n = 0;
    m_m = 0;
    m_valid = false;
}
void KruskalModule::releaseSceneItems() {
    m_nodes.clear();
    m_edges.clear();
    m_edgeBlocks.clear();
    m_parentBlocks.clear();
    m_edgePointer = nullptr;
    m_mathText = nullptr;
}