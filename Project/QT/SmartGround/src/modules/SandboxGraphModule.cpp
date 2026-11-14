#include "SandboxGraphModule.h"
#include "ui/UiTheme.h"
#include <QFont>
#include <QRegularExpression>

SandboxGraphModule::SandboxGraphModule() {}

QString SandboxGraphModule::getModuleName() const { return "自由结构 - 图结构"; }
QString SandboxGraphModule::getDefaultInput() const { return "5\n1 2 3\n1 3 2\n2 4 5\n3 4 1\n4 5 4"; }
QString SandboxGraphModule::getInputHint() const {
    return "第一行输入顶点数 n\n"
           "接下来每行输入一条边的三个数 u v w：端点 u、端点 v、边权 w\n"
           "若某行只写了两个数 u v，则该条边不带权（画图时不显示权值）\n"
           "提示：可在左侧「图结构选项」切换有向 / 无向、双向箭头\n"
           "节点与边均可自由拖动，双击节点或边权可直接修改";
}

GraphModuleUtils::GraphOptions SandboxGraphModule::getGraphOptions() const {
    return m_options;
}

void SandboxGraphModule::setGraphOptions(const GraphModuleUtils::GraphOptions& options) {
    m_options = options;
    if (m_valid) rebuildGraph();
}

void SandboxGraphModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;

    QStringList lines = inputData.split(QRegularExpression("[\\r\\n]"), Qt::SkipEmptyParts);
    if (lines.isEmpty()) return;
    bool ok = false;
    m_n = lines[0].toInt(&ok);
    if (!ok || m_n < 1 || m_n > 12) return;
    m_edges.clear();
    for (int i = 1; i < lines.size(); ++i) {
        QStringList tokens = lines[i].split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (tokens.size() < 2) continue;
        bool uok = false;
        bool vok = false;
        int u = tokens[0].toInt(&uok);
        int v = tokens[1].toInt(&vok);
        if (!uok || !vok || u < 1 || u > m_n || v < 1 || v > m_n) continue;
        EdgeSpec spec;
        spec.u = u;
        spec.v = v;
        if (tokens.size() >= 3) {
            bool wok = false;
            int w = tokens[2].toInt(&wok);
            if (wok) {
                spec.hasWeight = true;
                spec.weight = w;
            }
        }
        m_edges.push_back(spec);
    }
    m_valid = true;
    rebuildGraph();
}

void SandboxGraphModule::rebuildGraph() {
    if (!m_scene) return;
    for (GraphEdge* edge : m_edgesItems) {
        if (!edge) continue;
        edge->detach();
        if (edge->scene()) edge->scene()->removeItem(edge);
        delete edge;
    }
    m_edgesItems.clear();
    for (auto& kv : m_nodes) {
        GraphNode* node = kv.second;
        if (!node) continue;
        node->setParentItem(nullptr);
        if (node->scene()) node->scene()->removeItem(node);
        delete node;
    }
    m_nodes.clear();

    double radius = 130.0;
    for (int i = 0; i < m_n; ++i) {
        GraphNode* node = new GraphNode(i + 1);
        QPointF p = GraphModuleUtils::circlePoint(i, m_n, QPointF(420, 300), radius);
        node->setPos(p);
        node->color = QColor(252, 252, 254);
        m_scene->addItem(node);
        m_nodes[i + 1] = node;
        registerNodeSlot(i + 1, "v", i + 1);
    }
    int counter = 0;
    for (const auto& spec : m_edges) {
        GraphNode* u = m_nodes[spec.u];
        GraphNode* v = m_nodes[spec.v];
        if (!u || !v) continue;
        GraphEdge* edge = new GraphEdge(u, v, m_options.directed, m_options.bidirectional);
        if (spec.hasWeight) edge->setWeight(spec.weight);
        m_scene->addItem(edge);
        u->addEdge(edge);
        v->addEdge(edge);
        m_edgesItems.push_back(edge);
        registerEdgeSlot(edge->key(), "w", counter++);
    }
}

void SandboxGraphModule::reset() {
    FreeStructureBase::reset();
    m_n = 0;
    m_edges.clear();
    m_nodes.clear();
    m_edgesItems.clear();
}
