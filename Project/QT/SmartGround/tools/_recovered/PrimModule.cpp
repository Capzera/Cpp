#include "PrimModule.h"
#include "GraphModuleUtils.h"
#include "GraphRenderHelper.h"
#include <QFont>
#include <algorithm>
#include "ui/UiFonts.h"

static const QColor kPrTree = QColor(140, 205, 165);
static const QColor kPrTry = QColor(255, 205, 130);
static const QColor kPrIn = QColor(160, 195, 235);
static const QColor kPrIdle = QColor(252, 252, 254);

PrimModule::PrimModule() {}

QString PrimModule::getModuleName() const { return "Prim 最小生成树"; }
QString PrimModule::getDefaultInput() const {
    return "6 9 1\n"
           "1 2 4\n"
           "1 3 3\n"
           "2 3 2\n"
           "2 4 5\n"
           "3 4 3\n"
           "3 5 6\n"
           "4 5 1\n"
           "4 6 7\n"
           "5 6 8";
}
bool PrimModule::supportsGraphOptions() const { return true; }
bool PrimModule::supportsDirected() const { return false; }
bool PrimModule::supportsBidirectional() const { return true; }
GraphModuleUtils::GraphOptions PrimModule::getGraphOptions() const { return m_options; }
void PrimModule::setGraphOptions(const GraphModuleUtils::GraphOptions& options) { m_options = options; }
QString PrimModule::getInputHint() const {
    return "第一行输入三个正整数：顶点数 n、边数 m、起点 src\n"
           "接下来 m 行，每行三个整数 u, v, w，表示一条无向边 u - v，权值为 w\n"
           "本模块固定按无向图处理，从 src 出发不断吸收离生成树最近的边";
}
std::map<int, QPointF> PrimModule::captureNodePositions() const {
    return GraphRenderHelper::captureNodePositions(m_nodes);
}
void PrimModule::restoreNodePositions(const std::map<int, QPointF>& positions) {
    GraphRenderHelper::restoreNodePositions(m_nodes, positions);
}

void PrimModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;
    GraphModuleUtils::GraphInput input;
    GraphModuleUtils::GraphOptions options = m_options;
    options.directed = false;
    m_valid = GraphModuleUtils::parse(inputData, true, options, input);
    m_mathText = scene->addText(m_valid ? "Prim：从起点出发，贪心吸收离生成树最近的边"
                                        : "输入格式：n m src，随后输入 m 条无向边 u v w");
    m_mathText->setFont(UiFonts::mono(15, true));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(140, 18);
    if (!m_valid) return;

    m_n = input.n;
    m_m = input.m;
    m_src = input.src;

    std::vector<GraphModuleUtils::EdgeInput> liveEdges;
    for (const auto& edge : input.edges) {
        const int w = liveValue("w", static_cast<int>(m_edgeList.size()), edge.w);
        m_edgeList.push_back({edge.u, edge.v, w});
        GraphModuleUtils::EdgeInput le = edge;
        le.w = w;
        liveEdges.push_back(le);
    }

    GraphRenderHelper::buildNodes(scene, m_nodes, m_n, QPointF(470, 200), m_n <= 8 ? 110 : 130, this);
    GraphRenderHelper::buildEdges(scene, m_nodes, m_edges, liveEdges, options, input.weighted);
    GraphRenderHelper::registerEdgeSlots(this, m_edges, input.edges, options);

    const int startX = std::max(140, 470 - m_n * 30);
    GraphRenderHelper::addLabel(scene, "dis", QPointF(30, 348));
    GraphRenderHelper::addLabel(scene, "in", QPointF(30, 448));
    GraphRenderHelper::addLabel(scene, "e[]", QPointF(30, 548));
    for (int i = 1; i <= m_n; ++i) {
        const int x = startX + (i - 1) * 60;
        ArrayBlock* dist = new ArrayBlock(i, 0, QString::number(i));
        dist->setCustomValue("∞");
        dist->setPos(x, 365);
        scene->addItem(dist);
        m_distBlocks[i] = dist;

        ArrayBlock* inTree = new ArrayBlock(i, 0, QString::number(i));
        inTree->setCustomValue("0");
        inTree->setPos(x, 465);
        scene->addItem(inTree);
        m_inBlocks[i] = inTree;

        ArrayBlock* edge = new ArrayBlock(i - 1, 0, QString::number(i - 1));
        edge->showValue = false;
        edge->setPos(x, 565);
        scene->addItem(edge);
        m_edgeBlocks[i - 1] = edge;
    }

    m_pickPointer = new PointerBlock("u", QColor(210, 80, 55));
    m_pickPointer->setOpacity(0);
    m_pickPointer->setDirection(PointerBlock::Up);
    m_pickPointer->setAnchorOffset(QPointF(0, 20));
    m_pickPointer->setLabelOffset(QPointF(0, 30));
    m_pickPointer->setShaftLength(24);
    scene->addItem(m_pickPointer);
}

std::vector<AnimFrame> PrimModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;

    constexpr int inf = 1000000000;
    std::vector<int> dist(m_n + 1, inf);
    std::vector<bool> inTree(m_n + 1, false);
    std::vector<int> from(m_n + 1, 0);

    m_frames.push_back({ActionType::PR_RESET, m_src, 0, 0, 0, {},
                        QString("从起点 %1 出发，初始化距离数组").arg(m_src),
                        QString("dist[%1] = 0，其余为 ∞").arg(m_src)});

    dist[m_src] = 0;
    inTree[m_src] = true;
    m_frames.push_back({ActionType::PR_SELECT, m_src, 0, 0, 0, {},
                        QString("起点 %1 加入生成树").arg(m_src),
                        QString("dist[%1] = 0").arg(m_src)});

    int total = 0;
    int picked = 1;
    int edgeIndex = 0;

    auto relaxFrom = [&](int u) {
        for (const auto& e : m_edgeList) {
            const int a = std::get<0>(e);
            const int b = std::get<1>(e);
            const int w = std::get<2>(e);
            int other = 0;
            if (a == u && !inTree[b]) other = b;
            else if (b == u && !inTree[a]) other = a;
            if (other == 0) continue;
            const int old = dist[other];
            const bool improved = w < old;
            const int next = improved ? w : old;
            m_frames.push_back({ActionType::PR_RELAX, u, other, improved ? next : -1, w, {},
                                QString("用边 %1—%2（权 %3）更新 dist[%4]：%5 → %6")
                                    .arg(a).arg(b).arg(w).arg(other)
                                    .arg(old == inf ? QString("∞") : QString::number(old))
                                    .arg(next),
                                QString("dist[%1] = min(%2, %3) = %4")
                                    .arg(other)
                                    .arg(old == inf ? QString("∞") : QString::number(old))
                                    .arg(w).arg(next)});
            if (improved) {
                dist[other] = w;
                from[other] = u;
            }
        }
    };

    relaxFrom(m_src);

    while (picked < m_n) {
        int u = 0;
        for (int i = 1; i <= m_n; ++i) {
            if (inTree[i]) continue;
            if (u == 0 || dist[i] < dist[u]) u = i;
        }
        if (u == 0 || dist[u] == inf) {
            m_frames.push_back({ActionType::PR_FINISH, picked, m_n, total, 1, {},
                                QString("剩余节点与生成树不连通，最小生成树不完整"),
                                QString("存在孤立分量，MST 不存在")});
            return m_frames;
        }

        inTree[u] = true;
        ++picked;
        total += dist[u];
        m_frames.push_back({ActionType::PR_TAKE, u, from[u], dist[u], edgeIndex, {},
                            QString("吸收节点 %1（连边权 %2），当前 MST 权值 %3").arg(u).arg(dist[u]).arg(total),
                            QString("MST 权值 += %1 = %2").arg(dist[u]).arg(total)});
        GraphEdge* edge = GraphRenderHelper::findEdge(m_edges, from[u], u);
        if (edge) {
            edge->setColor(QColor(55, 175, 115));
            edge->setPenWidth(4.0);
        }
        ++edgeIndex;

        relaxFrom(u);
    }

    m_frames.push_back({ActionType::PR_FINISH, picked, m_n, total, 0, {},
                        QString("Prim 完成，共 %1 条边，MST 总权值 %2").arg(picked - 1).arg(total),
                        QString("MST 总权值 = %1").arg(total)});
    return m_frames;
}

void PrimModule::renderFrame(const AnimFrame& frame) {
    if (m_mathText) {
        m_mathText->setPlainText(frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    }
    if (!m_valid) return;

    if (frame.type == ActionType::PR_RESET) {
        for (auto& kv : m_nodes) {
            kv.second->color = kPrIdle;
            kv.second->setHighlightColor(Qt::transparent);
            kv.second->update();
        }
        for (auto& kv : m_edges) {
            kv.second->setColor(Qt::black);
            kv.second->setPenWidth(2.0);
            kv.second->update();
        }
        for (auto& kv : m_distBlocks) {
            kv.second->setCustomValue("∞");
            kv.second->color = Qt::white;
            kv.second->update();
        }
        for (auto& kv : m_inBlocks) {
            kv.second->setCustomValue("0");
            kv.second->color = Qt::white;
            kv.second->update();
        }
        for (auto& kv : m_edgeBlocks) {
            kv.second->showValue = false;
            kv.second->color = Qt::white;
            kv.second->update();
        }
        m_pickPointer->setOpacity(0);
    } else if (frame.type == ActionType::PR_SELECT) {
        if (m_nodes.count(frame.u)) {
            m_nodes[frame.u]->color = kPrTree;
            m_nodes[frame.u]->setHighlightColor(kPrTree);
            m_nodes[frame.u]->update();
            if (m_animated) playScalePulse(m_nodes[frame.u], 1.25, 260);
        }
        if (m_inBlocks.count(frame.u)) {
            m_inBlocks[frame.u]->setCustomValue("1");
            m_inBlocks[frame.u]->color = kPrTree;
            m_inBlocks[frame.u]->update();
        }
        if (m_distBlocks.count(frame.u)) {
            m_distBlocks[frame.u]->setCustomValue("0");
            m_distBlocks[frame.u]->color = kPrTree;
            m_distBlocks[frame.u]->update();
        }
        m_pickPointer->setOpacity(1);
        if (m_nodes.count(frame.u)) {
            retargetPointerToNode(m_pickPointer, m_nodes[frame.u]);
        }
    } else if (frame.type == ActionType::PR_RELAX) {
        GraphEdge* edge = GraphRenderHelper::findEdge(m_edges, frame.u, frame.v);
        const bool improved = frame.val1 >= 0;
        if (edge) {
            edge->setColor(improved ? QColor(235, 155, 55) : QColor(190, 195, 205));
            edge->update();
        }
        if (improved) {
            if (m_nodes.count(frame.v)) {
                m_nodes[frame.v]->color = kPrTry;
                m_nodes[frame.v]->update();
            }
            if (m_distBlocks.count(frame.v)) {
                m_distBlocks[frame.v]->setCustomValue(QString::number(frame.val1));
                m_distBlocks[frame.v]->color = kPrIn;
                m_distBlocks[frame.v]->update();
            }
        } else if (m_distBlocks.count(frame.v)) {
            m_distBlocks[frame.v]->setCustomValue(
                m_distBlocks[frame.v]->customValue.isEmpty() ? "∞" : m_distBlocks[frame.v]->customValue);
            m_distBlocks[frame.v]->color = kPrIdle;
            m_distBlocks[frame.v]->update();
        }
    } else if (frame.type == ActionType::PR_TAKE) {
        if (m_nodes.count(frame.u)) {
            m_nodes[frame.u]->color = kPrTree;
            m_nodes[frame.u]->setHighlightColor(kPrTree);
            m_nodes[frame.u]->update();
            if (m_animated) playScalePulse(m_nodes[frame.u], 1.3, 300);
        }
        if (m_inBlocks.count(frame.u)) {
            m_inBlocks[frame.u]->setCustomValue("1");
            m_inBlocks[frame.u]->color = kPrTree;
            m_inBlocks[frame.u]->update();
        }
        if (m_distBlocks.count(frame.u)) {
            m_distBlocks[frame.u]->setCustomValue(QString::number(frame.val1));
            m_distBlocks[frame.u]->color = kPrTree;
            m_distBlocks[frame.u]->update();
        }
        const int index = frame.val2;
        if (index >= 0 && index < static_cast<int>(m_edgeBlocks.size())) {
            m_edgeBlocks[index]->showValue = true;
            m_edgeBlocks[index]->value = frame.val1;
            m_edgeBlocks[index]->color = kPrTree;
            m_edgeBlocks[index]->update();
            if (m_animated) playScalePulse(m_edgeBlocks[index], 1.2, 240);
        }
        GraphEdge* edge = GraphRenderHelper::findEdge(m_edges, frame.v, frame.u);
        if (edge) {
            edge->setColor(QColor(55, 175, 115));
            edge->setPenWidth(4.0);
            edge->update();
        }
        m_pickPointer->setOpacity(1);
        if (m_nodes.count(frame.u)) {
            retargetPointerToNode(m_pickPointer, m_nodes[frame.u]);
        }
    } else if (frame.type == ActionType::PR_FINISH) {
        m_pickPointer->setOpacity(0);
        if (frame.val2 == 1) {
            for (auto& kv : m_nodes) {
                if (kv.second->color == kPrIdle) {
                    kv.second->color = QColor(228, 208, 208);
                    kv.second->update();
                }
            }
        }
    }
}

void PrimModule::reset() {
    m_frames.clear();
    m_nodes.clear();
    m_edges.clear();
    m_distBlocks.clear();
    m_inBlocks.clear();
    m_edgeBlocks.clear();
    m_edgeList.clear();
    m_pickPointer = nullptr;
    m_mathText = nullptr;
    m_n = 0;
    m_m = 0;
    m_src = 1;
    m_valid = false;
}

void PrimModule::releaseSceneItems() {
    m_nodes.clear();
    m_edges.clear();
    m_distBlocks.clear();
    m_inBlocks.clear();
    m_edgeBlocks.clear();
    m_pickPointer = nullptr;
    m_mathText = nullptr;
}
