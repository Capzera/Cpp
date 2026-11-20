#include "SandboxTreeModule.h"
#include "ui/UiTheme.h"
#include <QFont>
#include <QRegularExpression>

SandboxTreeModule::SandboxTreeModule() {}

QString SandboxTreeModule::getModuleName() const { return "自由结构 - 树形结构"; }
QString SandboxTreeModule::getDefaultInput() const { return "1 2 3\n2 4 5\n3 6"; }
QString SandboxTreeModule::getInputHint() const {
    return "每行输入一个节点的子节点编号，空格分隔\n"
           "第一行的编号为根节点，编号从 1 开始\n"
           "例如 1 2 3 表示根 1 有孩子 2 和 3\n"
           "提示：节点可自由拖动，双击节点可改写其显示值";
}

void SandboxTreeModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;

    QStringList lines = inputData.split(QRegularExpression("[\\r\\n]"), Qt::SkipEmptyParts);
    std::map<int, std::vector<int>> children;
    int maxId = 0;
    for (const QString& line : lines) {
        QStringList tokens = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (tokens.isEmpty()) continue;
        bool ok = false;
        int parent = tokens[0].toInt(&ok);
        if (!ok || parent < 1) continue;
        maxId = qMax(maxId, parent);
        for (int i = 1; i < tokens.size(); ++i) {
            bool cok = false;
            int child = tokens[i].toInt(&cok);
            if (!cok || child < 1) continue;
            children[parent].push_back(child);
            maxId = qMax(maxId, child);
        }
    }
    if (children.empty() || maxId == 0) return;
    layoutTree(1, children);
}

void SandboxTreeModule::layoutTree(int root, const std::map<int, std::vector<int>>& children) {
    struct Frame { int id; int depth; double offset; };
    std::vector<Frame> stack;
    stack.push_back({root, 0, 0.0});
    std::map<int, QPointF> positions;
    double leafSpan = 110.0;
    double nextOffset = 0.0;
    std::map<int, double> childOffset;
    std::vector<int> order;

    std::function<void(int, int)> assign = [&](int id, int depth) {
        auto it = children.find(id);
        if (it == children.end() || it->second.empty()) {
            childOffset[id] = nextOffset;
            nextOffset += leafSpan;
            positions[id] = QPointF(420.0 + childOffset[id], 180.0 + depth * 100.0);
            order.push_back(id);
            return;
        }
        for (int child : it->second) {
            assign(child, depth + 1);
        }
        double sum = 0.0;
        for (int child : it->second) sum += childOffset[child];
        childOffset[id] = sum / it->second.size();
        positions[id] = QPointF(420.0 + childOffset[id], 180.0 + depth * 100.0);
        order.push_back(id);
    };
    assign(root, 0);

    for (int id : order) {
        GraphNode* node = new GraphNode(id);
        node->setPos(positions[id]);
        node->color = QColor(252, 252, 254);
        node->setCustomValueText(QString::number(id));
        m_scene->addItem(node);
        m_nodes[id] = node;
        registerNodeSlot(id, "v", id);
    }
    for (int id : order) {
        auto it = children.find(id);
        if (it == children.end()) continue;
        for (int child : it->second) {
            GraphNode* parent = m_nodes[id];
            GraphNode* kid = m_nodes[child];
            if (!parent || !kid) continue;
            GraphEdge* edge = new GraphEdge(parent, kid, true, false);
            m_scene->addItem(edge);
            parent->addEdge(edge);
            kid->addEdge(edge);
        }
    }
}

void SandboxTreeModule::onNodeValueEdited(int id, const QString& text) {
    m_values[id] = text;
}

void SandboxTreeModule::reset() {
    FreeStructureBase::reset();
    m_nodes.clear();
    m_values.clear();
}
