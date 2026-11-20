#include "TarjanCutVertexBridgeModule.h"
#include "GraphRenderHelper.h"
#include <QFont>
#include <QRegularExpression>
#include <algorithm>

static const QColor kTcVisit = QColor(255, 230, 180);
static const QColor kTcStack = QColor(190, 215, 245);
static const QColor kTcCut = QColor(238, 120, 108);
static const QColor kTcBridge = QColor(200, 70, 60);
static const QColor kTcPale = QColor(250, 250, 252);

TarjanCutVertexBridgeModule::TarjanCutVertexBridgeModule() { initGraph(); }

QString TarjanCutVertexBridgeModule::getModuleName() const { return "Tarjan 割点与桥"; }
QString TarjanCutVertexBridgeModule::getDefaultInput() const {
    return "6 6\n1 2\n2 3\n3 1\n3 4\n4 5\n5 6";
}

bool TarjanCutVertexBridgeModule::supportsGraphOptions() const { return true; }
bool TarjanCutVertexBridgeModule::supportsDirected() const { return false; }
bool TarjanCutVertexBridgeModule::supportsBidirectional() const { return true;
}
GraphModuleUtils::GraphOptions TarjanCutVertexBridgeModule::getGraphOptions() const { return m_options; }
void TarjanCutVertexBridgeModule::setGraphOptions(const GraphModuleUtils::GraphOptions& options) {
    m_options = options;
}

QString TarjanCutVertexBridgeModule::getInputHint() const {
    return "第一行输入两个正整数：顶点数 n、边数 m\n"
           "接下来 m 行，每行两个整数 u, v，表示一条无向边 u - v\n"
           "割点判定：存在子节点 v 满足 low[v] >= dfn[u]\n"
           "桥判定：存在子节点 v 满足 low[v] > dfn[u]";
}

void TarjanCutVertexBridgeModule::initGraph() {
    m_tot = 0;
    std::fill(m_head, m_head + 205, 0);
    std::fill(m_to, m_to + 205, 0);
    std::fill(m_next, m_next + 205, 0);
    resetTraversalState();
}

void TarjanCutVertexBridgeModule::resetTraversalState() {
    m_timestamp = 0;
    std::fill(m_dfn, m_dfn + 205, 0);
    std::fill(m_low, m_low + 205, 0);
}

void TarjanCutVertexBridgeModule::addEdge(int u, int v) {
    m_to[++m_tot] = v;
    m_next[m_tot] = m_head[u];
    m_head[u] = m_tot;
}

std::map<int, QPointF> TarjanCutVertexBridgeModule::captureNodePositions() const {
    return GraphRenderHelper::captureNodePositions(m_nodes);
}

void TarjanCutVertexBridgeModule::restoreNodePositions(const std::map<int, QPointF>& positions) {
    GraphRenderHelper::restoreNodePositions(m_nodes, positions);
}

void TarjanCutVertexBridgeModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    m_nodes.clear();
    m_edges.clear();
    m_dfnBlocks.clear();
    m_lowBlocks.clear();
    m_bridgeEdges.clear();
    m_edgeKeyU.clear();
    m_edgeKeyV.clear();
    m_frames.clear();
    initGraph();

    m_mathText = createScriptText(scene, "");
    m_mathText->setFont(QFont("Consolas", 14, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(50, 22);

    GraphModuleUtils::GraphInput input;
    GraphModuleUtils::GraphOptions options = m_options;
    options.directed = false;
    m_valid = GraphModuleUtils::parse(inputData, false, options, input);
    if (!m_valid) {
        setScriptText(m_mathText, "输入格式错误：第一行 n m，接下来 m 行 u v");
        return;
    }
    m_n = input.n;
    m_m = input.m;
    for (const auto& edge : input.edges) { addEdge(edge.u, edge.v); addEdge(edge.v, edge.u); }

    GraphRenderHelper::buildNodes(scene, m_nodes, m_n, QPointF(400, 268), 148.0);
    GraphRenderHelper::buildEdges(scene, m_nodes, m_edges, input.edges, options, input.weighted);

    const int cellW = 62;
    const int dfnX = 170;
    const int dfnY = 545;
    const int rowGap = 100;
    GraphRenderHelper::addLabel(scene, "dfn[]", QPointF(48, dfnY + 8));
    GraphRenderHelper::addLabel(scene, "low[]", QPointF(48, dfnY + rowGap + 8));
    for (int i = 1; i <= m_n; ++i) {
        ArrayBlock* dfn = new ArrayBlock(i, 0, QString::number(i));
        dfn->setCellSize(cellW, 40.0);
        dfn->setPos(dfnX + (i - 1) * cellW, dfnY);
        dfn->color = kTcPale;
        scene->addItem(dfn);
        m_dfnBlocks[i] = dfn;

        ArrayBlock* low = new ArrayBlock(1000 + i, 0, QString::number(i));
        low->setCellSize(cellW, 40.0);
        low->setPos(dfnX + (i - 1) * cellW, dfnY + rowGap);
        low->color = kTcPale;
        scene->addItem(low);
        m_lowBlocks[i] = low;
    }
    setScriptText(m_mathText, "Tarjan 求割点与桥：dfn[] 记录访问次序，low[] 记录子树能回溯到的最小 dfn");
}

ArrayBlock* TarjanCutVertexBridgeModule::dfnBlock(int u) const {
    auto it = m_dfnBlocks.find(u);
    return it == m_dfnBlocks.end() ? nullptr : it->second;
}

ArrayBlock* TarjanCutVertexBridgeModule::lowBlock(int u) const {
    auto it = m_lowBlocks.find(u);
    return it == m_lowBlocks.end() ? nullptr : it->second;
}

void TarjanCutVertexBridgeModule::tarjan(int u, int parent) {
    m_dfn[u] = m_low[u] = ++m_timestamp;
    m_frames.push_back({ActionType::TC_VISIT, u, 0, m_dfn[u], m_low[u], {},
                        QString("访问节点 %1，dfn[%1] = low[%1] = %2").arg(u).arg(m_timestamp),
                        QString("dfn[%1] = low[%1] = %2").arg(u).arg(m_timestamp)});

    int children = 0;
    for (int e = m_head[u]; e; e = m_next[e]) {
        int v = m_to[e];
        if (v == parent) continue;
        m_frames.push_back({ActionType::TC_EDGE, u, v, m_dfn[u], m_low[u], {e},
                            QString("检查边 %1 - %2").arg(u).arg(v),
                            QString("检查边 %1 - %2").arg(u).arg(v)});
        if (!m_dfn[v]) {
            ++children;
            tarjan(v, u);
            m_low[u] = std::min(m_low[u], m_low[v]);
            m_frames.push_back({ActionType::TC_LOW, u, 0, m_dfn[u], m_low[u], {v},
                                QString("回溯：low[%1] = min(low[%1], low[%2]) = %3").arg(u).arg(v).arg(m_low[u]),
                                QString("low[%1] = min(low[%1], low[%2]) = min(%3, %4) = %5")
                                    .arg(u).arg(v).arg(m_low[u]).arg(m_low[v]).arg(m_low[u])});
            if (m_low[v] > m_dfn[u]) {
                m_frames.push_back({ActionType::TC_BRIDGE, u, v, 0, m_dfn[u], {},
                                    QString("桥：low[%1]=%2 > dfn[%1]=%3，边 %1-%2 是桥").arg(u).arg(m_low[v]).arg(m_dfn[u]),
                                    QString("low[%1] = %2 > dfn[%1] = %3　→　(%1, %2) 是桥")
                                        .arg(u).arg(m_low[v]).arg(m_dfn[u])});
            }
            if (m_low[v] >= m_dfn[u]) {
                m_frames.push_back({ActionType::TC_CUT, u, v, 1, m_dfn[u], {},
                                    QString("割点：low[%1]=%2 >= dfn[%1]=%3，节点 %1 是割点").arg(u).arg(m_low[v]).arg(m_dfn[u]),
                                    QString("low[%1] = %2 >= dfn[%1] = %3　→　%1 是割点")
                                        .arg(u).arg(m_low[v]).arg(m_dfn[u])});
            }
        } else if (m_dfn[v] < m_low[u]) {
            m_low[u] = m_dfn[v];
            m_frames.push_back({ActionType::TC_LOW, u, 0, m_dfn[u], m_low[u], {v},
                                QString("返祖边 %1 - %2，low[%1] = dfn[%2] = %3").arg(u).arg(v).arg(m_low[u]),
                                QString("low[%1] = min(low[%1], dfn[%2]) = %3").arg(u).arg(v).arg(m_low[u])});
        }
    }
    if (children > 1) {
        m_frames.push_back({ActionType::TC_CUT, u, 0, 2, m_dfn[u], {},
                            QString("根节点 %1 有 %2 个子树，是割点").arg(u).arg(children),
                            QString("根节点 %1 有 %2 个子树（>1）→ 割点").arg(u).arg(children)});
    }
}

std::vector<AnimFrame> TarjanCutVertexBridgeModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;
    resetTraversalState();
    m_edgeCounter = 0;
    m_bridgeEdges.clear();

    m_frames.push_back({ActionType::TC_RESET, 0, 0, 0, 0, {},
                        "开始 DFS，用 dfn 与 low 判定割点和桥",
                        "割点：low[v] >= dfn[u]　　桥：low[v] > dfn[u]"});
    for (int i = 1; i <= m_n; ++i) {
        if (!m_dfn[i]) tarjan(i, 0);
    }
    m_frames.push_back({ActionType::TC_DONE, 0, 0, 0, 0, {},
                        "遍历完成，红色节点为割点，红色粗边为桥",
                        "Tarjan 求割点与桥 完成"});
    return m_frames;
}

void TarjanCutVertexBridgeModule::renderFrame(const AnimFrame& frame) {
    if (m_mathText) {
        setScriptText(m_mathText, frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    }
    if (frame.type == ActionType::TC_RESET) {
        for (auto& kv : m_nodes) {
            kv.second->color = kTcPale;
            kv.second->setHighlightColor(Qt::transparent);
            kv.second->updateData(0, 0);
            kv.second->update();
        }
        for (auto& kv : m_edges) {
            kv.second->color = Qt::black;
            kv.second->setPenWidth(2.0);
            kv.second->update();
        }
        for (auto& kv : m_dfnBlocks) { kv.second->value = 0; kv.second->color = kTcPale; kv.second->update(); }
        for (auto& kv : m_lowBlocks) { kv.second->value = 0; kv.second->color = kTcPale; kv.second->update(); }
    } else if (frame.type == ActionType::TC_VISIT) {
        GraphNode* node = m_nodes[frame.u];
        if (node) {
            node->color = kTcVisit;
            node->updateData(frame.val1, frame.val2);
            node->setHighlightColor(kTcVisit);
            playScalePulse(node, 1.25, 260);
            playNodeRipple(node, kTcVisit, 40.0, 560);
        }
        if (ArrayBlock* dfn = dfnBlock(frame.u)) {
            dfn->value = frame.val1;
            dfn->color = kTcStack;
            dfn->update();
        }
        if (ArrayBlock* low = lowBlock(frame.u)) {
            low->value = frame.val2;
            low->color = kTcStack;
            low->update();
        }
    } else if (frame.type == ActionType::TC_EDGE) {
        GraphEdge* edge = GraphRenderHelper::findEdge(m_edges, frame.u, frame.v, false);
        if (edge) {
            edge->color = QColor(90, 140, 200);
            edge->update();
            playEdgeTravel(edge, QColor(90, 140, 200), 460);
        }
        GraphNode* node = m_nodes[frame.u];
        if (node) { node->setHighlightColor(QColor(90, 140, 200)); node->update(); }
    } else if (frame.type == ActionType::TC_LOW) {
        GraphNode* node = m_nodes[frame.u];
        if (node) {
            node->updateData(frame.val1, frame.val2);
            node->color = kTcVisit;
            node->update();
        }
        if (ArrayBlock* dfn = dfnBlock(frame.u)) {
            dfn->value = frame.val1;
            dfn->update();
        }
        if (ArrayBlock* low = lowBlock(frame.u)) {
            low->value = frame.val2;
            low->color = kTcStack;
            low->update();
        }
    } else if (frame.type == ActionType::TC_CUT) {
        GraphNode* node = m_nodes[frame.u];
        if (node) {
            node->color = kTcCut;
            node->setHighlightColor(kTcCut);
            node->update();
            playScalePulse(node, 1.4, 320);
            playNodeRipple(node, kTcCut, 54.0, 720);
        }
    } else if (frame.type == ActionType::TC_BRIDGE) {
        GraphEdge* edge = GraphRenderHelper::findEdge(m_edges, frame.u, frame.v, false);
        if (edge) {
            edge->color = kTcBridge;
            edge->setPenWidth(4.0);
            edge->update();
            playScalePulse(edge, 1.12, 280);
            playEdgeTravel(edge, kTcBridge, 540);
        }
    } else if (frame.type == ActionType::TC_DONE) {
        for (auto& kv : m_nodes) {
            kv.second->setHighlightColor(Qt::transparent);
            kv.second->update();
            playNodePop(kv.second, 1.10, 260);
        }
        for (auto& kv : m_edges) {
            if (kv.second->color != kTcBridge) { kv.second->color = Qt::black; kv.second->update(); }
        }
    }
}

void TarjanCutVertexBridgeModule::reset() {
    m_frames.clear();
    m_nodes.clear();
    m_edges.clear();
    m_dfnBlocks.clear();
    m_lowBlocks.clear();
    m_bridgeEdges.clear();
    m_edgeKeyU.clear();
    m_edgeKeyV.clear();
    m_mathText = nullptr;
    m_n = 0;
    m_m = 0;
    m_edgeCounter = 0;
    m_valid = false;
    initGraph();
}

void TarjanCutVertexBridgeModule::releaseSceneItems() {
    m_nodes.clear();
    m_edges.clear();
    m_dfnBlocks.clear();
    m_lowBlocks.clear();
    m_bridgeEdges.clear();
    m_mathText = nullptr;
}
