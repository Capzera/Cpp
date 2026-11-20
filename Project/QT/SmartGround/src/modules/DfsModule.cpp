#include "DfsModule.h"
#include "GraphModuleUtils.h"
#include "GraphRenderHelper.h"
#include <QFont>
#include <algorithm>
#include "ui/UiFonts.h"

static const QColor kDfsIdle = QColor(252, 252, 254);
static const QColor kDfsActive = QColor(255, 205, 130);
static const QColor kDfsDone = QColor(150, 215, 170);
static const QColor kDfsBack = QColor(210, 190, 235);

DfsModule::DfsModule() {}

QString DfsModule::getModuleName() const { return "DFS 深度优先搜索"; }
QString DfsModule::getDefaultInput() const { return "7 8 1\n1 2\n1 3\n2 4\n2 5\n3 5\n3 6\n5 7\n6 7"; }
bool DfsModule::supportsGraphOptions() const { return true; }
bool DfsModule::supportsDirected() const { return true;
}
GraphModuleUtils::GraphOptions DfsModule::getGraphOptions() const { return m_options; }
void DfsModule::setGraphOptions(const GraphModuleUtils::GraphOptions& options) { m_options = options; }
QString DfsModule::getInputHint() const {
    return "第一行输入三个正整数：顶点数 n、边数 m、起点 src\n"
           "接下来 m 行，每行两个整数 u, v，表示一条边 u - v\n"
           "算法用显式栈模拟递归：入栈标记访问，弹栈时回溯到父节点";
}
std::map<int, QPointF> DfsModule::captureNodePositions() const {
    return GraphRenderHelper::captureNodePositions(m_nodes);
}
void DfsModule::restoreNodePositions(const std::map<int, QPointF>& positions) {
    GraphRenderHelper::restoreNodePositions(m_nodes, positions);
}

void DfsModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;
    GraphModuleUtils::GraphInput input;
    m_valid = GraphModuleUtils::parse(inputData, true, m_options, input);
    m_mathText = createScriptText(scene, m_valid ? "DFS：沿一条路走到尽头，再回溯换路"
                                        : "输入格式：n m src，随后输入 m 条边 u v");
    m_mathText->setFont(UiFonts::mono(15, true));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(180, 18);
    if (!m_valid) return;

    m_n = input.n;
    m_m = input.m;
    m_src = input.src;
    for (const auto& edge : input.edges) {
        m_edgeList.push_back({edge.u, edge.v});
        int id = ++m_edgeCount;
        m_to[id] = edge.v;
        m_next[id] = m_head[edge.u];
        m_head[edge.u] = id;
        if (!m_options.directed) {
            id = ++m_edgeCount;
            m_to[id] = edge.u;
            m_next[id] = m_head[edge.v];
            m_head[edge.v] = id;
        }
    }

    GraphRenderHelper::buildNodes(scene, m_nodes, m_n, QPointF(470, 205), m_n <= 8 ? 115 : 135, this);
    GraphRenderHelper::buildEdges(scene, m_nodes, m_edges, input.edges, m_options, input.weighted);

    const int startX = std::max(140, 470 - m_n * 30);
    GraphRenderHelper::addLabel(scene, "stk", QPointF(30, 348));
    GraphRenderHelper::addLabel(scene, "vis", QPointF(30, 478));
    GraphRenderHelper::addLabel(scene, "ord", QPointF(30, 608));
    for (int i = 1; i <= m_n; ++i) {
        const int x = startX + (i - 1) * 60;
        ArrayBlock* stackBlock = new ArrayBlock(i - 1, 0, QString::number(i - 1));
        stackBlock->showValue = false;
        stackBlock->setPos(x, 365);
        scene->addItem(stackBlock);
        m_stackBlocks[i - 1] = stackBlock;

        ArrayBlock* vis = new ArrayBlock(i, 0, QString::number(i));
        vis->setCustomValue("0");
        vis->setPos(x, 495);
        scene->addItem(vis);
        m_visBlocks[i] = vis;

        ArrayBlock* ord = new ArrayBlock(i - 1, 0, QString::number(i - 1));
        ord->showValue = false;
        ord->setPos(x, 625);
        scene->addItem(ord);
        m_orderBlocks[i - 1] = ord;
    }

    m_stackPointer = new PointerBlock("top", QColor(210, 95, 50));
    m_stackPointer->setOpacity(0);
    scene->addItem(m_stackPointer);
    m_curPointer = new PointerBlock("u", QColor(45, 130, 210));
    m_curPointer->setOpacity(0);
    m_curPointer->setDirection(PointerBlock::Left);
    m_curPointer->setLabelToRight();
    m_curPointer->setAnchorOffset(QPointF(20, 0));
    scene->addItem(m_curPointer);
}

std::vector<AnimFrame> DfsModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;

    bool visited[101]{};
    int stack[101]{};
    int top = 0;
    int orderCount = 0;

    m_frames.push_back({ActionType::DFS_RESET, m_src, 0, 0, 0, {},
                        QString("从起点 %1 开始，初始化访问标记与栈").arg(m_src),
                        QString("vis[] = 0, stk[top++] = %1").arg(m_src)});

    visited[m_src] = true;
    stack[top++] = m_src;
    m_frames.push_back({ActionType::DFS_PUSH, m_src, 0, 0, 0, {},
                        QString("起点 %1 入栈并标记已访问").arg(m_src),
                        QString("vis[%1] = 1").arg(m_src)});

    while (top > 0) {
        const int u = stack[top - 1];
        int nextV = 0;
        for (int edge = m_head[u]; edge; edge = m_next[edge]) {
            const int v = m_to[edge];
            m_frames.push_back({ActionType::DFS_EDGE, u, v, visited[v] ? 1 : 0, 0, {},
                                QString("检查 %1 的邻点 %2").arg(u).arg(v),
                                visited[v] ? QString("vis[%1] = 1，已访问，跳过").arg(v)
                                           : QString("vis[%1] = 0，可以深入").arg(v)});
            if (visited[v]) continue;
            nextV = v;
            break;
        }

        if (nextV == 0) {
            m_frames.push_back({ActionType::DFS_POP, u, 0, top - 1, 0, {},
                                QString("%1 没有未访问邻点，回溯出栈").arg(u),
                                QString("stk[--top]").arg(u)});
            --top;
            continue;
        }

        visited[nextV] = true;
        stack[top++] = nextV;
        m_frames.push_back({ActionType::DFS_PUSH, nextV, u, 0, 0, {},
                            QString("深入到 %1（来自 %2）").arg(nextV).arg(u),
                            QString("vis[%1] = 1").arg(nextV)});
        m_frames.push_back({ActionType::DFS_VISIT, nextV, 0, orderCount, 0, {},
                            QString("%1 记入访问序列第 %2 位").arg(nextV).arg(orderCount + 1),
                            QString("ord[%1] = %2").arg(orderCount).arg(nextV)});
        ++orderCount;
    }

    m_frames.push_back({ActionType::DFS_DONE, orderCount, m_n, 0, 0, {},
                        QString("DFS 完成，访问 %1/%2 个节点").arg(orderCount).arg(m_n),
                        QString("DFS 访问序列共 %1 个节点").arg(orderCount)});
    return m_frames;
}

void DfsModule::renderFrame(const AnimFrame& frame) {
    if (m_mathText) {
        setScriptText(m_mathText, frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    }
    if (!m_valid) return;

    if (frame.type == ActionType::DFS_RESET) {
        for (auto& kv : m_nodes) {
            kv.second->color = kDfsIdle;
            kv.second->setHighlightColor(Qt::transparent);
            kv.second->update();
        }
        for (auto& kv : m_edges) kv.second->setColor(Qt::black);
        for (auto& kv : m_visBlocks) {
            kv.second->setCustomValue("0");
            kv.second->color = Qt::white;
            kv.second->update();
        }
        for (auto& kv : m_stackBlocks) {
            kv.second->showValue = false;
            kv.second->color = Qt::white;
            kv.second->update();
        }
        for (auto& kv : m_orderBlocks) {
            kv.second->showValue = false;
            kv.second->color = Qt::white;
            kv.second->update();
        }
        m_stackPointer->setOpacity(0);
        m_curPointer->setOpacity(0);
    } else if (frame.type == ActionType::DFS_PUSH) {
        const int u = frame.u;
        if (m_nodes.count(u)) {
            m_nodes[u]->color = kDfsActive;
            m_nodes[u]->setHighlightColor(kDfsActive);
            m_nodes[u]->update();
            if (m_animated) playScalePulse(m_nodes[u], 1.25, 260);
        }
        if (m_visBlocks.count(u)) {
            m_visBlocks[u]->setCustomValue("1");
            m_visBlocks[u]->color = kDfsDone;
            m_visBlocks[u]->update();
        }
        const int slot = frame.v;
        if (slot >= 0 && slot < static_cast<int>(m_stackBlocks.size())) {
            playValueFlight(m_nodes[u] ? m_nodes[u]->pos() : QPointF(470, 170),
                            m_stackBlocks[slot], u, QColor(185, 220, 245), true);
        }
        m_stackPointer->setOpacity(1);
        m_curPointer->setOpacity(1);
        if (slot >= 0 && slot < static_cast<int>(m_stackBlocks.size())) {
            playPointerArrange({m_stackPointer}, {m_stackBlocks[slot]}, 42);
        }
        if (m_nodes.count(u)) {
            retargetPointerToNode(m_curPointer, m_nodes[u]);
        }
    } else if (frame.type == ActionType::DFS_EDGE) {
        for (auto& kv : m_edges) {
            if (kv.second->color == QColor(235, 155, 55)) kv.second->setColor(Qt::black);
        }
        GraphEdge* edge = GraphRenderHelper::findEdge(m_edges, frame.u, frame.v);
        if (edge) {
            edge->setColor(frame.val1 ? QColor(200, 205, 214) : QColor(235, 155, 55));
            edge->update();
            if (!frame.val1) playEdgeTravel(edge, QColor(235, 155, 55), 440);
            else playEdgeSweep(edge, QColor(200, 205, 214), 320);
        }
    } else if (frame.type == ActionType::DFS_VISIT) {
        const int index = frame.val1;
        if (index >= 0 && index < static_cast<int>(m_orderBlocks.size())) {
            playValueFlight(m_nodes[frame.u] ? m_nodes[frame.u]->pos() : QPointF(470, 170),
                            m_orderBlocks[index], frame.u, kDfsDone, true);
        }
        if (m_nodes.count(frame.u)) playDiscover(m_nodes[frame.u], QColor(120, 200, 255), 48.0, 640);
    } else if (frame.type == ActionType::DFS_POP) {
        if (m_nodes.count(frame.u)) {
            m_nodes[frame.u]->color = kDfsBack;
            m_nodes[frame.u]->update();
            playNodePop(m_nodes[frame.u], 0.92, 260);
        }
        const int slot = frame.val1;
        if (slot >= 0 && slot < static_cast<int>(m_stackBlocks.size())) {
            m_stackBlocks[slot]->color = QColor(200, 200, 208);
            m_stackBlocks[slot]->update();
        }
        const int nextSlot = slot - 1;
        if (nextSlot >= 0 && nextSlot < static_cast<int>(m_stackBlocks.size())) {
            playPointerArrange({m_stackPointer}, {m_stackBlocks[nextSlot]}, 42);
        } else {
            m_stackPointer->setOpacity(0);
        }
    } else if (frame.type == ActionType::DFS_DONE) {
        for (auto& kv : m_nodes) {
            kv.second->setHighlightColor(Qt::transparent);
            if (kv.second->color == kDfsActive) {
                kv.second->color = kDfsDone;
                kv.second->update();
            }
        }
        for (auto& kv : m_orderBlocks) {
            if (kv.second->showValue) {
                kv.second->color = kDfsDone;
                kv.second->update();
            }
        }
        m_stackPointer->setOpacity(0);
    }
}

void DfsModule::reset() {
    m_frames.clear();
    m_nodes.clear();
    m_edges.clear();
    m_stackBlocks.clear();
    m_visBlocks.clear();
    m_orderBlocks.clear();
    m_edgeList.clear();
    m_stackPointer = nullptr;
    m_curPointer = nullptr;
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

void DfsModule::releaseSceneItems() {
    m_nodes.clear();
    m_edges.clear();
    m_stackBlocks.clear();
    m_visBlocks.clear();
    m_orderBlocks.clear();
    m_stackPointer = nullptr;
    m_curPointer = nullptr;
    m_mathText = nullptr;
}
