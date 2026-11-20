#include "DSUBasicModule.h"
#include <QRegularExpression>
#include <QPropertyAnimation>

DSUBasicModule::DSUBasicModule() {}

QString DSUBasicModule::getModuleName() const {
    return "并查集";
}

QString DSUBasicModule::getDefaultInput() const {
    return "10\n0 1 1 2 3 4 4 5 6 7";
}
QString DSUBasicModule::getInputHint() const {
    return "第一行输入一个正整数 n，表示集合中元素的个数\n"
           "接下来 n 个整数 p[i]，表示元素 i 的父节点编号（从 0 开始）\n"
           "p[i] = i 表示该元素自成一个集合";
}
std::map<int, QPointF> DSUBasicModule::captureNodePositions() const {
    std::map<int, QPointF> result;
    for (const auto& pair : m_nodes) result[pair.first] = pair.second->pos();
    return result;
}
void DSUBasicModule::restoreNodePositions(const std::map<int, QPointF>& positions) {
    for (const auto& pair : positions) {
        auto it = m_nodes.find(pair.first);
        if (it != m_nodes.end()) it->second->setPos(pair.second);
    }
}

void DSUBasicModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    m_nodes.clear();
    m_edges.clear();
    m_frames.clear();
    QStringList items = inputData.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    if (items.isEmpty()) return;
    n = items[0].toInt();
    if (n < 0) n = 0;
    if (n > items.size() - 1) n = items.size() - 1;
    if (n <= 0) return;
    std::vector<int> parent(n);
    for (int i = 1; i <= n; ++i) {
        int p = items[i].toInt();
        if (p < 0 || p >= n) p = i - 1;
        parent[i - 1] = p;
    }
    for (int i = 0; i < n; ++i) {
        GraphNode* node = new GraphNode(i);
        m_scene->addItem(node);
        node->setPos(100 + (i % 5) * 150, 150 + (i / 5) * 200);
        m_nodes[i] = node;
    }
    for (int i = 0; i < n; ++i) {
        if (parent[i] != i) {
            GraphEdge* edge = new GraphEdge(m_nodes[i], m_nodes[parent[i]], true);
            m_scene->addItem(edge);
            m_nodes[i]->addEdge(edge);
            m_nodes[parent[i]]->addEdge(edge);
            m_edges.push_back(edge);
        }
    }
    m_mathText = createScriptText(m_scene, "并查集：快速判断连通性和合并集合");
    m_mathText->setPos(100, 50);
    m_mathText->setFont(QFont("Consolas", 12, QFont::Bold));
}

std::vector<AnimFrame> DSUBasicModule::runAlgorithm() {
    m_frames.clear();
    int n = m_nodes.size();
    std::vector<int> parent(n), rnk(n, 0);
    for (int i = 0; i < n; ++i) {
        parent[i] = i;
    }
    m_frames.push_back({ActionType::GN_HIGHLIGHT_NODE, 0, 0, 0, 0, {},
        QString("初始化：每个节点是独立的集合")});
    std::vector<std::pair<int,int>> unions = {{0, 1}, {2, 3}, {1, 3}, {4, 5}, {5, 6}};
    for (auto& uv : unions) {
        if (uv.first >= n || uv.second >= n) continue;
        auto [u, v] = uv;
        m_frames.push_back({ActionType::GN_HIGHLIGHT_NODE, u, 0, 0, 0, {},
            QString("Find(%1) 查询根节点").arg(u)});
        int ru = find(parent, u);
        m_frames.push_back({ActionType::GN_HIGHLIGHT_NODE, ru, 0, 0, 0, {},
            QString("Found root: %1").arg(ru)});
        int rv = find(parent, v);
        m_frames.push_back({ActionType::GN_HIGHLIGHT_NODE, rv, 0, 0, 0, {},
            QString("Found root: %1").arg(rv)});
        if (ru != rv) {
            m_frames.push_back({ActionType::GN_TRAVERSE_EDGE, u, v, 0, 0, {},
                QString("Union(%1, %2): 合并两个集合").arg(u).arg(v)});
            if (rnk[ru] < rnk[rv]) {
                std::swap(ru, rv);
            }
            parent[rv] = ru;
            if (rnk[ru] == rnk[rv]) {
                rnk[ru]++;
            }
            m_frames.push_back({ActionType::GN_UPDATE_LOW, ru, rv, 0, 0, {},
                QString("集合合并完成")});
        } else {
            m_frames.push_back({ActionType::GN_CONDENSE, u, v, 0, 0, {},
                QString("已在同一集合中，不需合并")});
        }
    }
    return m_frames;
}

int DSUBasicModule::find(std::vector<int>& parent, int x) {
    int root = x;
    int guard = 0;
    while (parent[root] != root && guard <= (int)parent.size()) {
        root = parent[root];
        guard++;
    }
    if (parent[root] != root) return x;
    while (parent[x] != x) {
        int nxt = parent[x];
        parent[x] = root;
        x = nxt;
    }
    return root;
}

void DSUBasicModule::renderFrame(const AnimFrame& frame) {
    if (!m_mathText) return;
    setScriptText(m_mathText, frame.explanation);
    if (frame.type == ActionType::GN_HIGHLIGHT_NODE) {
        if (m_nodes.count(frame.u)) {
            m_nodes[frame.u]->color = QColor(255, 200, 100);
            m_nodes[frame.u]->setZValue(2);
            m_nodes[frame.u]->update();
            if (m_animated) {
                playScalePulse(m_nodes[frame.u], 1.15, 200);
                playNodeRipple(m_nodes[frame.u], QColor(255, 190, 90), 34.0, 480);
            }
        }
    } else if (frame.type == ActionType::GN_TRAVERSE_EDGE) {
        if (m_nodes.count(frame.u)) {
            m_nodes[frame.u]->color = QColor(150, 200, 255);
            m_nodes[frame.u]->update();
        }
        if (m_nodes.count(frame.v)) {
            m_nodes[frame.v]->color = QColor(150, 200, 255);
            m_nodes[frame.v]->update();
        }
        for (auto edge : m_edges) {
            if ((edge->source->id == frame.u && edge->dest->id == frame.v) ||
                (edge->source->id == frame.v && edge->dest->id == frame.u)) {
                edge->setColor(QColor(255, 100, 100));
                playEdgeTravel(edge, QColor(255, 100, 100), 460);
            }
        }
    } else if (frame.type == ActionType::GN_UPDATE_LOW) {
        if (m_nodes.count(frame.u)) {
            m_nodes[frame.u]->color = QColor(150, 255, 150);
            m_nodes[frame.u]->update();
            playNodeRipple(m_nodes[frame.u], QColor(140, 230, 160), 38.0, 520);
        }
        if (m_nodes.count(frame.v)) {
            m_nodes[frame.v]->color = QColor(150, 255, 150);
            m_nodes[frame.v]->update();
            playNodeRipple(m_nodes[frame.v], QColor(140, 230, 160), 38.0, 520);
        }
    } else if (frame.type == ActionType::GN_CONDENSE) {
        if (m_nodes.count(frame.u)) {
            m_nodes[frame.u]->color = QColor(200, 200, 200);
            m_nodes[frame.u]->update();
            playNodePop(m_nodes[frame.u], 0.93, 260);
        }
        if (m_nodes.count(frame.v)) {
            m_nodes[frame.v]->color = QColor(200, 200, 200);
            m_nodes[frame.v]->update();
            playNodePop(m_nodes[frame.v], 0.93, 260);
        }
    }
}

void DSUBasicModule::reset() {
    m_nodes.clear();
    m_edges.clear();
    m_frames.clear();
}
void DSUBasicModule::releaseSceneItems() {
    m_nodes.clear();
    m_edges.clear();
    m_mathText = nullptr;
}
