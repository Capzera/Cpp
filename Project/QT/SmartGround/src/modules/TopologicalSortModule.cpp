#include "TopologicalSortModule.h"
#include "GraphModuleUtils.h"
#include "GraphRenderHelper.h"
#include <QFont>
#include <QRegularExpression>
#include <algorithm>
#include <iterator>

TopologicalSortModule::TopologicalSortModule() : m_headPointer(nullptr), m_tailPointer(nullptr), m_mathText(nullptr), m_n(0), m_m(0), m_edgeCount(0), m_valid(false) {
    reset();
}

QString TopologicalSortModule::getModuleName() const { return "拓扑排序"; }
QString TopologicalSortModule::getDefaultInput() const { return "8 9\n1 3\n1 4\n2 4\n2 5\n3 6\n4 6\n4 7\n5 7\n6 8"; }
bool TopologicalSortModule::supportsGraphOptions() const { return true; }
bool TopologicalSortModule::supportsDirected() const { return true; }
GraphModuleUtils::GraphOptions TopologicalSortModule::getGraphOptions() const { return m_options; }
void TopologicalSortModule::setGraphOptions(const GraphModuleUtils::GraphOptions& options) { m_options = options; }
QString TopologicalSortModule::getInputHint() const {
    return "第一行输入两个正整数：顶点数 n、边数 m\n"
           "接下来 m 行，每行两个整数 u, v，表示一条有向边 u -> v\n"
           "若任意一行写了三个数 u v w，则整张图按带权图处理";
}
std::map<int, QPointF> TopologicalSortModule::captureNodePositions() const {
    return GraphRenderHelper::captureNodePositions(m_nodes);
}
void TopologicalSortModule::restoreNodePositions(const std::map<int, QPointF>& positions) {
    GraphRenderHelper::restoreNodePositions(m_nodes, positions);
}

void TopologicalSortModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;
    GraphModuleUtils::GraphInput input;
    GraphModuleUtils::GraphOptions options = m_options;
    options.directed = true;
    m_valid = GraphModuleUtils::parse(inputData, false, options, input);
    m_mathText = scene->addText(m_valid ? "拓扑排序：每次移除入度为 0 的点" : "输入格式：n m，随后输入 m 条有向边 u v");
    m_mathText->setFont(QFont("Consolas", 16, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(180, 18);
    if (!m_valid) return;
    m_n = input.n;
    m_m = input.m;
    m_edgeList.reserve(m_m);
    for (const auto& edge : input.edges) {
        m_edgeList.push_back({edge.u, edge.v});
        int id = ++m_edgeCount;
        m_to[id] = edge.v;
        m_next[id] = m_head[edge.u];
        m_head[edge.u] = id;
        ++m_initialDegree[edge.v];
    }
    std::vector<int> degree(m_n + 1);
    std::vector<int> layer(m_n + 1, 0);
    int queue[101]{};
    int head = 0, tail = 0, maxLayer = 0;
    for (int i = 1; i <= m_n; ++i) {
        degree[i] = m_initialDegree[i];
        if (degree[i] == 0) queue[tail++] = i;
    }
    while (head < tail) {
        int u = queue[head++];
        for (int edge = m_head[u]; edge; edge = m_next[edge]) {
            int v = m_to[edge];
            layer[v] = std::max(layer[v], layer[u] + 1);
            if (--degree[v] == 0) queue[tail++] = v;
        }
        maxLayer = std::max(maxLayer, layer[u]);
    }
    for (int i = 1; i <= m_n; ++i) if (degree[i] > 0) layer[i] = maxLayer + 1;
    std::vector<std::vector<int>> layers(maxLayer + 2);
    for (int i = 1; i <= m_n; ++i) layers[layer[i]].push_back(i);
    int maxLayerSize = 1;
    for (const auto& row : layers) maxLayerSize = std::max(maxLayerSize, static_cast<int>(row.size()));
    for (int level = 0; level < static_cast<int>(layers.size()); ++level) {
        const auto& row = layers[level];
        if (row.empty()) continue;
        for (int j = 0; j < static_cast<int>(row.size()); ++j) {
            int id = row[j];
            GraphNode* node = new GraphNode(id);
            node->setPos(120 + level * 150, 80 + j * 90);
            scene->addItem(node);
            m_nodes[id] = node;
            registerNodeSlot(id, "v", id);
        }
    }
    GraphRenderHelper::buildEdges(scene, m_nodes, m_edges, input.edges, options, input.weighted);
    int startX = std::max(140, 470 - m_n * 30);
    int degreeY = 100 + maxLayerSize * 90;
    GraphRenderHelper::addLabel(scene, "ind", QPointF(35, degreeY - 22), 11);
    GraphRenderHelper::addLabel(scene, "q", QPointF(35, degreeY + 83), 11);
    GraphRenderHelper::addLabel(scene, "out", QPointF(35, degreeY + 188), 11);
    for (int i = 1; i <= m_n; ++i) {
        ArrayBlock* block = new ArrayBlock(i, 0, QString::number(i));
        block->setPos(startX + (i - 1) * 60, degreeY);
        scene->addItem(block);
        m_degreeBlocks[i] = block;
        ArrayBlock* queueBlock = new ArrayBlock(i - 1, 0, QString::number(i - 1));
        queueBlock->showValue = false;
        queueBlock->setPos(startX + (i - 1) * 60, degreeY + 100);
        scene->addItem(queueBlock);
        m_queueBlocks[i - 1] = queueBlock;
        ArrayBlock* orderBlock = new ArrayBlock(i - 1, 0, QString("%1").arg(i - 1));
        orderBlock->showValue = false;
        orderBlock->setPos(startX + (i - 1) * 60, degreeY + 200);
        scene->addItem(orderBlock);
        m_orderBlocks[i - 1] = orderBlock;
    }
    m_headPointer = new PointerBlock("H", QColor(45, 130, 210));
    m_tailPointer = new PointerBlock("T", QColor(210, 95, 50));
    m_headPointer->setOpacity(0);
    m_tailPointer->setOpacity(0);
    scene->addItem(m_headPointer);
    scene->addItem(m_tailPointer);
}

std::vector<AnimFrame> TopologicalSortModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;
    int degree[101]{};
    int queue[101]{};
    int head = 0, tail = 0, count = 0;
    m_frames.push_back({ActionType::TS_RESET, 0, 0, 0, 0, {}, "初始化入度表、队列与结果序列", "indegree[v] = 0"});
    for (const auto& edge : m_edgeList) {
        int value = ++degree[edge.second];
        m_frames.push_back({ActionType::TS_DEGREE, edge.first, edge.second, value, 0, {}, QString("加入边 %1 → %2，入度增加为 %3").arg(edge.first).arg(edge.second).arg(value), QString("indegree[%1]++ = %2").arg(edge.second).arg(value)});
    }
    for (int i = 1; i <= m_n; ++i) {
        if (degree[i] == 0) {
            queue[tail] = i;
            m_frames.push_back({ActionType::TS_ENQUEUE, i, tail, tail + 1, 0, {}, QString("节点 %1 入度为 0，进入队列").arg(i), QString("q[%1] = %2").arg(tail).arg(i)});
            ++tail;
        }
    }
    while (head < tail) {
        int u = queue[head];
        m_frames.push_back({ActionType::TS_DEQUEUE, u, head, head + 1, 0, {}, QString("队头节点 %1 出队").arg(u), QString("head = %1").arg(head + 1)});
        ++head;
        m_frames.push_back({ActionType::TS_OUTPUT, u, count, count + 1, 0, {}, QString("节点 %1 加入拓扑序列").arg(u), QString("order[%1] = %2").arg(count).arg(u)});
        ++count;
        for (int edge = m_head[u]; edge; edge = m_next[edge]) {
            int v = m_to[edge];
            int value = --degree[v];
            m_frames.push_back({ActionType::TS_SCAN_EDGE, u, v, value, 0, {}, QString("删除边 %1 → %2，入度变为 %3").arg(u).arg(v).arg(value), QString("indegree[%1]-- = %2").arg(v).arg(value)});
            if (value == 0) {
                queue[tail] = v;
                m_frames.push_back({ActionType::TS_ENQUEUE, v, tail, tail + 1, 0, {}, QString("节点 %1 入度归零，进入队列").arg(v), QString("indegree[%1] = 0 ⇒ q[%2] = %1").arg(v).arg(tail)});
                ++tail;
            }
        }
    }
    if (count < m_n) {
        for (int i = 1; i <= m_n; ++i) if (degree[i] > 0) m_frames.push_back({ActionType::TS_BLOCKED, i, 0, degree[i], 0, {}, QString("节点 %1 仍有入度 %2，受有向环阻塞").arg(i).arg(degree[i]), QString("indegree[%1] = %2 > 0").arg(i).arg(degree[i])});
    }
    m_frames.push_back({ActionType::TS_FINISH, count, m_n, 0, 0, {}, count == m_n ? "拓扑排序完成" : QString("只输出 %1/%2 个节点，图中存在有向环").arg(count).arg(m_n), count == m_n ? "有向无环图" : "输出数 < n ⇒ 存在有向环"});
    return m_frames;
}

void TopologicalSortModule::renderFrame(const AnimFrame& frame) {
    if (!m_mathText) return;
    m_mathText->setPlainText(frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    if (frame.type == ActionType::TS_RESET) {
        for (int i = 1; i <= m_n; ++i) {
            m_degreeBlocks[i]->value = 0;
            m_degreeBlocks[i]->color = Qt::white;
            m_degreeBlocks[i]->update();
            m_nodes[i]->color = Qt::white;
            m_nodes[i]->update();
        }
        for (auto& pair : m_queueBlocks) { pair.second->showValue = false; pair.second->color = Qt::white; pair.second->update(); }
        for (auto& pair : m_orderBlocks) { pair.second->showValue = false; pair.second->color = Qt::white; pair.second->update(); }
        for (auto& pair : m_edges) pair.second->setColor(Qt::black);
        m_headPointer->setOpacity(0);
        m_tailPointer->setOpacity(0);
    } else if (frame.type == ActionType::TS_DEGREE) {
        m_degreeBlocks[frame.v]->value = frame.val1;
        m_degreeBlocks[frame.v]->color = QColor(170, 215, 255);
        m_degreeBlocks[frame.v]->update();
        GraphEdge* edge = GraphRenderHelper::findEdge(m_edges, frame.u, frame.v);
        if (edge) edge->setColor(QColor(65, 145, 220));
    } else if (frame.type == ActionType::TS_ENQUEUE) {
        GraphNode* node = m_nodes[frame.u];
        node->color = QColor(145, 220, 235);
        node->update();
        playValueFlight(node->pos(), m_queueBlocks[frame.v], frame.u, QColor(190, 240, 245), true);
        if (frame.v == 0) m_headPointer->setOpacity(1);
        int next = frame.v + 1;
        if (next < m_n) m_tailPointer->setOpacity(1);
        else m_tailPointer->setOpacity(0);
        if (frame.v == 0) playPointerArrange({m_headPointer, m_tailPointer}, {m_queueBlocks[0], m_queueBlocks[0]});
        else if (next < m_n) playPointerArrange({m_headPointer, m_tailPointer}, {m_queueBlocks[0], m_queueBlocks[next]});
    } else if (frame.type == ActionType::TS_DEQUEUE) {
        m_queueBlocks[frame.v]->color = QColor(205, 210, 218);
        m_queueBlocks[frame.v]->update();
        if (frame.val1 < m_n) {
            int tailIndex = (m_tailPointer->opacity() > 0.05) ? frame.val1 + 1 : frame.val1;
            if (tailIndex >= m_n) tailIndex = frame.val1;
            playPointerArrange({m_headPointer, m_tailPointer}, {m_queueBlocks[frame.val1], m_queueBlocks[tailIndex]});
        } else {
            m_headPointer->setOpacity(0);
        }
        m_nodes[frame.u]->color = QColor(255, 190, 105);
        m_nodes[frame.u]->update();
    } else if (frame.type == ActionType::TS_OUTPUT) {
        playValueFlight(m_nodes[frame.u]->pos(), m_orderBlocks[frame.v], frame.u, QColor(175, 220, 180), true);
        m_nodes[frame.u]->color = QColor(195, 200, 208);
        m_nodes[frame.u]->update();
    } else if (frame.type == ActionType::TS_SCAN_EDGE) {
        for (auto& pair : m_edges) if (pair.second->color == QColor(235, 155, 55)) pair.second->setColor(QColor(170, 195, 220));
        GraphEdge* edge = GraphRenderHelper::findEdge(m_edges, frame.u, frame.v);
        if (edge) edge->setColor(QColor(235, 155, 55));
        m_degreeBlocks[frame.v]->value = frame.val1;
        m_degreeBlocks[frame.v]->color = frame.val1 == 0 ? QColor(145, 220, 235) : QColor(255, 220, 165);
        m_degreeBlocks[frame.v]->update();
        if (frame.val1 == 0) { m_nodes[frame.v]->color = QColor(145, 220, 235); m_nodes[frame.v]->update(); }
    } else if (frame.type == ActionType::TS_BLOCKED) {
        m_nodes[frame.u]->color = QColor(245, 155, 155);
        m_nodes[frame.u]->update();
        m_degreeBlocks[frame.u]->color = QColor(245, 155, 155);
        m_degreeBlocks[frame.u]->update();
    } else if (frame.type == ActionType::TS_FINISH) {
        if (frame.u == m_n) {
            for (auto& pair : m_orderBlocks) pair.second->color = QColor(175, 220, 180);
        }
        for (auto& pair : m_orderBlocks) pair.second->update();
    }
}

void TopologicalSortModule::reset() {
    m_frames.clear();
    m_nodes.clear();
    m_edges.clear();
    m_degreeBlocks.clear();
    m_queueBlocks.clear();
    m_orderBlocks.clear();
    m_edgeList.clear();
    m_headPointer = nullptr;
    m_tailPointer = nullptr;
    m_mathText = nullptr;
    m_n = 0;
    m_m = 0;
    m_edgeCount = 0;
    m_valid = false;
    std::fill(std::begin(m_head), std::end(m_head), 0);
    std::fill(std::begin(m_to), std::end(m_to), 0);
    std::fill(std::begin(m_next), std::end(m_next), 0);
    std::fill(std::begin(m_initialDegree), std::end(m_initialDegree), 0);
}
void TopologicalSortModule::releaseSceneItems() {
    m_nodes.clear();
    m_edges.clear();
    m_degreeBlocks.clear();
    m_queueBlocks.clear();
    m_orderBlocks.clear();
    m_headPointer = nullptr;
    m_tailPointer = nullptr;
    m_mathText = nullptr;
}