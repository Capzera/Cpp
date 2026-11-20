#include "BipartiteModule.h"
#include "GraphModuleUtils.h"
#include "GraphRenderHelper.h"
#include <QFont>
#include <algorithm>
#include "ui/UiFonts.h"

static const QColor kBpBlue = QColor(120, 175, 235);
static const QColor kBpOrange = QColor(245, 195, 145);
static const QColor kBpIdle = QColor(252, 252, 254);
static const QColor kBpConflict = QColor(240, 130, 120);

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
           "算法用 0/1 交替染色（蓝/橙两色），一旦发现相邻两点同色即判定不是二分图";
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
    m_mathText = scene->addText(m_valid ? "二分图判定：相邻节点必须异色（2-染色）"
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
    GraphRenderHelper::addLabel(scene, "col", QPointF(30, 470));
    for (int i = 1; i <= m_n; ++i) {
        ArrayBlock* block = new ArrayBlock(i, 0, QString::number(i));
        block->setCustomValue("-1");
        block->setPos(startX + (i - 1) * 60, 487);
        scene->addItem(block);
        m_colorBlocks[i] = block;
    }

    m_verdictText = scene->addText("");
    m_verdictText->setFont(UiFonts::cjk(14, true));
    m_verdictText->setDefaultTextColor(QColor(40, 140, 70));
    m_verdictText->setPos(150, 610);
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

std::vector<AnimFrame> BipartiteModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;

    std::fill(std::begin(m_color), std::end(m_color), -1);
    bool ok = true;

    m_frames.push_back({ActionType::BP_RESET, 0, 0, 0, 0, {},
                        QString("初始化颜色数组，全部未染色"),
                        QString("col[i] = -1")});

    for (int s = 1; s <= m_n && ok; ++s) {
        if (m_color[s] != -1) continue;
        m_color[s] = 0;
        m_frames.push_back({ActionType::BP_COLOR, s, 0, 0, 0, {},
                            QString("从孤立分量起点 %1 开始，染成蓝色（0）").arg(s),
                            QString("col[%1] = 0（蓝）").arg(s)});
        int queue[101]{};
        int head = 0;
        int tail = 0;
        queue[tail++] = s;
        while (head < tail && ok) {
            const int u = queue[head++];
            for (int edge = m_head[u]; edge; edge = m_next[edge]) {
                const int v = m_to[edge];
                if (m_color[v] == -1) {
                    m_color[v] = m_color[u] ^ 1;
                    m_frames.push_back({ActionType::BP_COLOR, v, u, m_color[v], 0, {},
                                        QString("节点 %1 与 %2 相邻，染成%3色（%4）")
                                            .arg(v).arg(u).arg(m_color[v] == 0 ? "蓝" : "橙").arg(m_color[v]),
                                        QString("col[%1] = col[%2] ^ 1 = %3").arg(v).arg(u).arg(m_color[v])});
                    queue[tail++] = v;
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
        m_mathText->setPlainText(frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
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
        if (m_verdictText) {
            m_verdictText->setPlainText("");
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
            edge->setColor(QColor(80, 175, 120));
            edge->update();
        }
    } else if (frame.type == ActionType::BP_CHECK) {
        GraphEdge* edge = GraphRenderHelper::findEdge(m_edges, frame.u, frame.v);
        const bool same = (frame.val1 == frame.val2);
        if (edge) {
            edge->setColor(same ? kBpConflict : QColor(80, 175, 120));
            edge->update();
        }
        if (m_nodes.count(frame.u)) {
            m_nodes[frame.u]->setHighlightColor(QColor(255, 225, 165));
            m_nodes[frame.u]->update();
        }
        if (m_nodes.count(frame.v)) {
            m_nodes[frame.v]->setHighlightColor(QColor(255, 225, 165));
            m_nodes[frame.v]->update();
        }
    } else if (frame.type == ActionType::BP_CONFLICT) {
        if (m_nodes.count(frame.u)) {
            m_nodes[frame.u]->color = kBpConflict;
            m_nodes[frame.u]->setHighlightColor(kBpConflict);
            m_nodes[frame.u]->update();
            if (m_animated) playScalePulse(m_nodes[frame.u], 1.3, 300);
        }
        if (m_nodes.count(frame.v)) {
            m_nodes[frame.v]->color = kBpConflict;
            m_nodes[frame.v]->setHighlightColor(kBpConflict);
            m_nodes[frame.v]->update();
            if (m_animated) playScalePulse(m_nodes[frame.v], 1.3, 300);
        }
        GraphEdge* edge = GraphRenderHelper::findEdge(m_edges, frame.u, frame.v);
        if (edge) {
            edge->setColor(kBpConflict);
            edge->setPenWidth(5.0);
            edge->update();
        }
        if (m_verdictText) {
            m_verdictText->setPlainText(QString("冲突：%1 与 %2 同色，非二分图").arg(frame.u).arg(frame.v));
            m_verdictText->setDefaultTextColor(kBpConflict.darker(115));
        }
    } else if (frame.type == ActionType::BP_FINISH) {
        if (m_verdictText) {
            const bool ok = frame.u == 1;
            m_verdictText->setPlainText(ok ? QString("结论：该图是二分图")
                                          : QString("结论：该图不是二分图"));
            m_verdictText->setDefaultTextColor(ok ? QColor(40, 140, 70) : kBpConflict.darker(115));
        }
        for (auto& kv : m_nodes) {
            kv.second->setHighlightColor(Qt::transparent);
            kv.second->update();
        }
    }
}

void BipartiteModule::reset() {
    m_frames.clear();
    m_nodes.clear();
    m_edges.clear();
    m_colorBlocks.clear();
    m_edgeList.clear();
    m_mathText = nullptr;
    m_verdictText = nullptr;
    m_n = 0;
    m_m = 0;
    m_edgeCount = 0;
    m_valid = false;
    std::fill(std::begin(m_head), std::end(m_head), 0);
    std::fill(std::begin(m_to), std::end(m_to), 0);
    std::fill(std::begin(m_next), std::end(m_next), 0);
    std::fill(std::begin(m_color), std::end(m_color), -1);
}

void BipartiteModule::releaseSceneItems() {
    m_nodes.clear();
    m_edges.clear();
    m_colorBlocks.clear();
    m_mathText = nullptr;
    m_verdictText = nullptr;
}
