#include "BFSModule.h"
#include "GraphModuleUtils.h"
#include "GraphRenderHelper.h"
#include <QFont>
#include <algorithm>
#include <iterator>

BFSModule::BFSModule() : m_headPointer(nullptr), m_tailPointer(nullptr), m_mathText(nullptr), m_n(0), m_m(0), m_src(1), m_edgeCount(0), m_valid(false) {
    reset();
}

QString BFSModule::getModuleName() const { return "BFS 广度优先搜索"; }
QString BFSModule::getDefaultInput() const { return "7 8 1\n1 2\n1 3\n2 4\n2 5\n3 5\n3 6\n5 7\n6 7"; }
bool BFSModule::supportsGraphOptions() const { return true; }
bool BFSModule::supportsDirected() const { return true; }
GraphModuleUtils::GraphOptions BFSModule::getGraphOptions() const { return m_options; }
void BFSModule::setGraphOptions(const GraphModuleUtils::GraphOptions& options) { m_options = options; }
QString BFSModule::getInputHint() const {
    return "第一行输入三个正整数：顶点数 n、边数 m、起点 src\n"
           "接下来 m 行，每行两个整数 u, v，表示一条边 u - v\n"
           "若任意一行写了三个数 u v w，则整张图按带权图处理";
}
std::map<int, QPointF> BFSModule::captureNodePositions() const {
    return GraphRenderHelper::captureNodePositions(m_nodes);
}
void BFSModule::restoreNodePositions(const std::map<int, QPointF>& positions) {
    GraphRenderHelper::restoreNodePositions(m_nodes, positions);
}

void BFSModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;
    GraphModuleUtils::GraphInput input;
    m_valid = GraphModuleUtils::parse(inputData, true, m_options, input);
    m_mathText = createScriptText(scene, m_valid ? "BFS：按层扩展，节点在入队时标记访问" : "输入格式：n m src，随后输入 m 条边 u v");
    m_mathText->setFont(QFont("Consolas", 15, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(180, 18);
    if (!m_valid) return;
    m_n = input.n;
    m_m = input.m;
    m_src = input.src;
    for (const auto& edge : input.edges) {
        m_edgeList.push_back({edge.u, edge.v});
        if (m_options.directed) {
            int id = ++m_edgeCount;
            m_to[id] = edge.v;
            m_next[id] = m_head[edge.u];
            m_head[edge.u] = id;
        } else {
            int id = ++m_edgeCount;
            m_to[id] = edge.v;
            m_next[id] = m_head[edge.u];
            m_head[edge.u] = id;
            id = ++m_edgeCount;
            m_to[id] = edge.u;
            m_next[id] = m_head[edge.v];
            m_head[edge.v] = id;
        }
    }
    GraphRenderHelper::buildNodes(scene, m_nodes, m_n, QPointF(470, 170), m_n <= 8 ? 115 : 135, this);
    GraphRenderHelper::buildEdges(scene, m_nodes, m_edges, input.edges, m_options, input.weighted);
    int startX = std::max(140, 470 - m_n * 30);
    GraphRenderHelper::addLabel(scene, "q", QPointF(35, 348));
    GraphRenderHelper::addLabel(scene, "dis", QPointF(35, 448));
    GraphRenderHelper::addLabel(scene, "pa", QPointF(35, 548));
    for (int i = 1; i <= m_n; ++i) {
        int x = startX + (i - 1) * 60;
        ArrayBlock* queueBlock = new ArrayBlock(i - 1, 0, QString::number(i - 1));
        queueBlock->showValue = false;
        queueBlock->setPos(x, 365);
        scene->addItem(queueBlock);
        m_queueBlocks[i - 1] = queueBlock;
        ArrayBlock* distance = new ArrayBlock(i, 0, QString::number(i));
        distance->setCustomValue("∞");
        distance->setPos(x, 465);
        scene->addItem(distance);
        m_distanceBlocks[i] = distance;
        ArrayBlock* parent = new ArrayBlock(i, 0, QString::number(i));
        parent->setCustomValue("—");
        parent->setPos(x, 565);
        scene->addItem(parent);
        m_parentBlocks[i] = parent;
    }
    m_headPointer = new PointerBlock("H", QColor(45, 130, 210));
    m_tailPointer = new PointerBlock("T", QColor(210, 95, 50));
    m_headPointer->setOpacity(0);
    m_tailPointer->setOpacity(0);
    scene->addItem(m_headPointer);
    scene->addItem(m_tailPointer);
}

std::vector<AnimFrame> BFSModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;
    int distance[101];
    int parent[101]{};
    int queue[101]{};
    std::fill(std::begin(distance), std::end(distance), -1);
    int head = 0, tail = 0;
    m_frames.push_back({ActionType::BF_RESET, m_src, 0, 0, 0, {}, "从起点开始，初始化距离与队列", "dist[src] = 0"});
    distance[m_src] = 0;
    queue[tail] = m_src;
    m_frames.push_back({ActionType::BF_DISCOVER, m_src, 0, 0, 0, {}, QString("起点 %1 的距离为 0").arg(m_src), QString("dist[%1] = 0").arg(m_src)});
    m_frames.push_back({ActionType::BF_ENQUEUE, m_src, tail, tail + 1, 0, {}, QString("起点 %1 入队").arg(m_src), QString("q[%1] = %2").arg(tail).arg(m_src)});
    ++tail;
    while (head < tail) {
        int u = queue[head];
        m_frames.push_back({ActionType::BF_DEQUEUE, u, head, head + 1, 0, {}, QString("节点 %1 出队，开始扩展第 %2 层").arg(u).arg(distance[u]), QString("head = %1").arg(head + 1)});
        ++head;
        for (int edge = m_head[u]; edge; edge = m_next[edge]) {
            int v = m_to[edge];
            m_frames.push_back({ActionType::BF_SCAN, u, v, distance[v], 0, {}, QString("检查邻点 %1").arg(v), QString("dist[%1] = %2").arg(v).arg(distance[v] < 0 ? QString("∞") : QString::number(distance[v]))});
            if (distance[v] != -1) continue;
            distance[v] = distance[u] + 1;
            parent[v] = u;
            m_frames.push_back({ActionType::BF_DISCOVER, u, v, distance[v], parent[v], {}, QString("首次到达节点 %1，记录层数与父节点").arg(v), QString("dist[%1] = dist[%2] + 1 = %3").arg(v).arg(u).arg(distance[v])});
            queue[tail] = v;
            m_frames.push_back({ActionType::BF_ENQUEUE, v, tail, tail + 1, 0, {}, QString("节点 %1 入队，访问标记已锁定").arg(v), QString("q[%1] = %2").arg(tail).arg(v)});
            ++tail;
        }
    }
    m_frames.push_back({ActionType::BF_FINISH, tail, m_n, 0, 0, {}, QString("BFS 完成，访问 %1/%2 个节点").arg(tail).arg(m_n), "同层节点拥有相同最短边数"});
    return m_frames;
}

void BFSModule::renderFrame(const AnimFrame& frame) {
    if (!m_mathText) return;
    setScriptText(m_mathText, frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    if (frame.type == ActionType::BF_RESET) {
        for (int i = 1; i <= m_n; ++i) {
            m_nodes[i]->color = Qt::white;
            m_nodes[i]->update();
            m_distanceBlocks[i]->setCustomValue("∞");
            m_parentBlocks[i]->setCustomValue("—");
            m_distanceBlocks[i]->color = Qt::white;
            m_parentBlocks[i]->color = Qt::white;
            m_distanceBlocks[i]->update();
            m_parentBlocks[i]->update();
        }
        for (auto& pair : m_queueBlocks) { pair.second->showValue = false; pair.second->color = Qt::white; pair.second->update(); }
        for (auto& pair : m_edges) pair.second->setColor(Qt::black);
        m_headPointer->setOpacity(0);
        m_tailPointer->setOpacity(0);
    } else if (frame.type == ActionType::BF_DISCOVER) {
        int nodeId = frame.v == 0 ? frame.u : frame.v;
        int distance = frame.v == 0 ? 0 : frame.val1;
        QColor layerColor = QColor::fromHsv((205 - distance * 18 + 360) % 360, 95, 245);
        m_nodes[nodeId]->color = layerColor;
        m_nodes[nodeId]->update();
        m_distanceBlocks[nodeId]->setCustomValue(QString::number(distance));
        m_distanceBlocks[nodeId]->color = QColor(185, 225, 245);
        m_distanceBlocks[nodeId]->update();
        if (frame.v != 0) {
            m_parentBlocks[nodeId]->setCustomValue(QString::number(frame.val2));
            m_parentBlocks[nodeId]->color = QColor(205, 220, 240);
            m_parentBlocks[nodeId]->update();
            GraphEdge* edge = GraphRenderHelper::findEdge(m_edges, frame.u, frame.v);
            if (edge) {
                edge->setColor(QColor(80, 175, 120));
                playEdgeDiscover(edge, m_nodes[nodeId], QColor(80, 175, 120), 520);
            }
        } else {
            playDiscover(m_nodes[nodeId], QColor(120, 200, 255), 52.0, 700);
        }
    } else if (frame.type == ActionType::BF_ENQUEUE) {
        playValueFlight(m_nodes[frame.u]->pos(), m_queueBlocks[frame.v], frame.u, QColor(175, 220, 240), true);
        playNodePop(m_nodes[frame.u], 1.16, 300);
        if (frame.v == 0) m_headPointer->setOpacity(1);
        int next = frame.v + 1;
        if (next < m_n) m_tailPointer->setOpacity(1);
        else m_tailPointer->setOpacity(0);
        if (frame.v == 0) playPointerArrange({m_headPointer, m_tailPointer}, {m_queueBlocks[0], m_queueBlocks[0]});
        else if (next < m_n) playPointerArrange({m_headPointer, m_tailPointer}, {m_queueBlocks[0], m_queueBlocks[next]});
    } else if (frame.type == ActionType::BF_DEQUEUE) {
        for (int i = 1; i <= m_n; ++i) {
            if (m_nodes[i]->color == QColor(255, 190, 105)) {
                int depth = m_distanceBlocks[i]->customValue.toInt();
                m_nodes[i]->color = QColor::fromHsv((205 - depth * 18 + 360) % 360, 95, 245);
                m_nodes[i]->update();
            }
        }
        m_queueBlocks[frame.v]->color = QColor(205, 210, 218);
        m_queueBlocks[frame.v]->update();
        m_nodes[frame.u]->color = QColor(255, 190, 105);
        m_nodes[frame.u]->update();
        playNodeWake(m_nodes[frame.u], QColor(255, 190, 105), 44.0, 560);
        if (frame.val1 < m_n) {
            int tailIndex = (m_tailPointer->opacity() > 0.05) ? frame.val1 + 1 : frame.val1;
            if (tailIndex >= m_n) tailIndex = frame.val1;
            playPointerArrange({m_headPointer, m_tailPointer}, {m_queueBlocks[frame.val1], m_queueBlocks[tailIndex]});
        } else {
            m_headPointer->setOpacity(0);
        }
    } else if (frame.type == ActionType::BF_SCAN) {
        for (auto& pair : m_edges) if (pair.second->color == QColor(235, 155, 55)) pair.second->setColor(Qt::black);
        for (int i = 1; i <= m_n; ++i) {
            if (m_nodes[i]->color == QColor(255, 225, 165)) {
                m_nodes[i]->color = Qt::white;
                m_nodes[i]->update();
            }
        }
        auto key = std::make_pair(std::min(frame.u, frame.v), std::max(frame.u, frame.v));
        GraphEdge* edge = GraphRenderHelper::findEdge(m_edges, frame.u, frame.v);
        if (edge && edge->color != QColor(80, 175, 120)) {
            edge->setColor(QColor(235, 155, 55));
            playEdgeTravel(edge, QColor(235, 155, 55), 460);
        }
        if (frame.val1 < 0) {
            m_nodes[frame.v]->color = QColor(255, 225, 165);
            m_nodes[frame.v]->update();
            playNodeWake(m_nodes[frame.v], QColor(255, 205, 120), 30.0, 440);
        }
    } else if (frame.type == ActionType::BF_FINISH) {
        for (int i = 1; i <= m_n; ++i) {
            if (m_nodes[i]->color == QColor(255, 190, 105)) {
                int depth = m_distanceBlocks[i]->customValue.toInt();
                m_nodes[i]->color = QColor::fromHsv((205 - depth * 18 + 360) % 360, 95, 245);
                m_nodes[i]->update();
            }
        }
        for (auto& pair : m_queueBlocks) pair.second->update();
        for (auto& kv : m_nodes) playNodePop(kv.second, 1.10, 260);
    }
}

void BFSModule::reset() {
    m_frames.clear();
    m_nodes.clear();
    m_edges.clear();
    m_queueBlocks.clear();
    m_distanceBlocks.clear();
    m_parentBlocks.clear();
    m_edgeList.clear();
    m_headPointer = nullptr;
    m_tailPointer = nullptr;
    m_mathText = nullptr;
    m_n = 0;
    m_m = 0;
    m_src = 1;
    m_edgeCount = 0;
    m_valid = false;
    std::fill(std::begin(m_head), std::end(m_head), 0);
    std::fill(std::begin(m_to), std::end(m_to), 0);
    std::fill(std::begin(m_next), std::end(m_next), 0);
}
void BFSModule::releaseSceneItems() {
    m_nodes.clear();
    m_edges.clear();
    m_queueBlocks.clear();
    m_distanceBlocks.clear();
    m_parentBlocks.clear();
    m_headPointer = nullptr;
    m_tailPointer = nullptr;
    m_mathText = nullptr;
}