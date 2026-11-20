#ifndef TREEDPBASE_H
#define TREEDPBASE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <QFont>
#include <QGraphicsTextItem>
#include <QRegularExpression>
#include <functional>
#include <map>
#include <vector>

class TreeDpBase : public IModule {
public:
    bool parseTreeInput(const QString& inputData, std::map<int, std::vector<int>>& children, int& maxId) {
        children.clear();
        maxId = 0;
        QStringList lines = inputData.split(QRegularExpression("[\\r\\n]"), Qt::SkipEmptyParts);
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
        return !children.empty() && maxId > 0;
    }

    void layoutTreeScene(const std::map<int, std::vector<int>>& children, int root,
                         QPointF origin, double levelGap, double leafSpan) {
        std::map<int, QPointF> positions;
        std::vector<int> order;
        double nextOffset = 0.0;
        std::map<int, double> childOffset;

        std::function<void(int, int)> assign = [&](int id, int depth) {
            auto it = children.find(id);
            if (it == children.end() || it->second.empty()) {
                childOffset[id] = nextOffset;
                nextOffset += leafSpan;
                positions[id] = QPointF(origin.x() + childOffset[id], origin.y() + depth * levelGap);
                order.push_back(id);
                return;
            }
            for (int child : it->second) assign(child, depth + 1);
            double sum = 0.0;
            for (int child : it->second) sum += childOffset[child];
            childOffset[id] = sum / it->second.size();
            positions[id] = QPointF(origin.x() + childOffset[id], origin.y() + depth * levelGap);
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
                GraphEdge* edge = new GraphEdge(parent, kid, true);
                edge->color = QColor(178, 188, 204);
                m_scene->addItem(edge);
                parent->addEdge(edge);
                kid->addEdge(edge);
                m_treeEdges[{id, child}] = edge;
            }
        }
    }

    void setNodeBadge(int id, const QString& text, const QColor& color) {
        auto it = m_nodes.find(id);
        if (it == m_nodes.end()) return;
        it->second->setTag(text, color);
    }

    std::map<int, QPointF> captureTreePositions() const {
        std::map<int, QPointF> result;
        for (const auto& pair : m_nodes) result[pair.first] = pair.second->pos();
        return result;
    }

    void restoreTreePositions(const std::map<int, QPointF>& positions) {
        for (const auto& pair : positions) {
            auto it = m_nodes.find(pair.first);
            if (it != m_nodes.end()) it->second->setPos(pair.second);
        }
    }

protected:
    std::map<int, GraphNode*> m_nodes;
    std::map<std::pair<int, int>, GraphEdge*> m_treeEdges;
};

#endif
