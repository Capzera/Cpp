#ifndef GRAPHRENDERHELPER_H
#define GRAPHRENDERHELPER_H
#include "IModule.h"
#include "GraphModuleUtils.h"
#include <QFont>
#include <algorithm>
#include <map>
#include <utility>

namespace GraphRenderHelper {
inline const QColor kAnimIdle = QColor(255, 255, 255);
inline const QColor kAnimScan = QColor(255, 214, 138);
inline const QColor kAnimActive = QColor(255, 176, 92);
inline const QColor kAnimDone = QColor(146, 220, 178);
inline const QColor kAnimPath = QColor(72, 178, 128);
inline const QColor kAnimReject = QColor(198, 204, 214);
inline const QColor kAnimDead = QColor(212, 214, 220);
inline const QColor kAnimBase = QColor(24, 30, 44);

inline void buildNodes(QGraphicsScene* scene, std::map<int, GraphNode*>& nodes, int n, QPointF center, qreal radius, IModule* module = nullptr) {
    for (int i = 1; i <= n; ++i) {
        GraphNode* node = new GraphNode(i);
        node->setPos(GraphModuleUtils::circlePoint(i - 1, n, center, radius));
        scene->addItem(node);
        nodes[i] = node;
        if (module) module->registerNodeSlot(i, "v", i);
    }
}

inline GraphEdge* buildOneEdge(QGraphicsScene* scene, std::map<int, GraphNode*>& nodes, std::map<std::pair<int,int>, GraphEdge*>& edges,
                               const GraphModuleUtils::EdgeInput& edge, const GraphModuleUtils::GraphOptions& options, bool weighted) {
    bool directed = options.directed;
    bool bidirectional = !options.directed && options.bidirectional;
    auto key = directed ? std::make_pair(edge.u, edge.v) : std::make_pair(std::min(edge.u, edge.v), std::max(edge.u, edge.v));
    auto it = edges.find(key);
    if (it != edges.end()) return it->second;
    GraphEdge* item = new GraphEdge(nodes[edge.u], nodes[edge.v], directed, bidirectional);
    if (weighted) item->setWeight(edge.w);
    scene->addItem(item);
    nodes[edge.u]->addEdge(item);
    nodes[edge.v]->addEdge(item);
    edges[key] = item;
    return item;
}

inline void buildEdges(QGraphicsScene* scene, std::map<int, GraphNode*>& nodes, std::map<std::pair<int,int>, GraphEdge*>& edges,
                       const std::vector<GraphModuleUtils::EdgeInput>& edgeInputs, const GraphModuleUtils::GraphOptions& options, bool weighted) {
    for (const auto& edge : edgeInputs) buildOneEdge(scene, nodes, edges, edge, options, weighted);
}

inline GraphEdge* findEdge(const std::map<std::pair<int,int>, GraphEdge*>& edges, int u, int v, bool directed = false) {
    auto it = edges.find(std::make_pair(u, v));
    if (it != edges.end()) return it->second;
    if (directed) return nullptr;
    it = edges.find(std::make_pair(v, u));
    if (it != edges.end()) return it->second;
    it = edges.find(std::make_pair(std::min(u, v), std::max(u, v)));
    return it != edges.end() ? it->second : nullptr;
}

inline QGraphicsTextItem* addLabel(QGraphicsScene* scene, const QString& text, QPointF pos, int size = 10) {
    QGraphicsTextItem* item = scene->addText(text);
    item->setFont(QFont("Microsoft YaHei", size, QFont::Bold));
    item->setDefaultTextColor(QColor(60, 75, 95));
    item->setPos(pos);
    return item;
}

inline std::map<int, QPointF> captureNodePositions(const std::map<int, GraphNode*>& nodes) {
    std::map<int, QPointF> result;
    for (const auto& pair : nodes) result[pair.first] = pair.second->pos();
    return result;
}

inline void registerEdgeSlots(IModule* module, const std::map<std::pair<int,int>, GraphEdge*>& edges,
                              const std::vector<GraphModuleUtils::EdgeInput>& edgeInputs,
                              const GraphModuleUtils::GraphOptions& options) {
    if (!module) return;
    int counter = 0;
    for (const auto& pair : edges) {
        GraphEdge* edge = pair.second;
        if (!edge) continue;
        int u = edge->source ? edge->source->id : 0;
        int v = edge->dest ? edge->dest->id : 0;
        int index = -1;
        for (size_t i = 0; i < edgeInputs.size(); ++i) {
            const auto& in = edgeInputs[i];
            if (in.u == u && in.v == v) { index = static_cast<int>(i); break; }
            if (!options.directed && in.u == v && in.v == u) { index = static_cast<int>(i); break; }
        }
        if (index < 0) index = counter;
        module->registerEdgeSlot(edge->key(), "w", index);
        ++counter;
    }
}

inline void restoreNodePositions(const std::map<int, GraphNode*>& nodes, const std::map<int, QPointF>& positions) {
    for (const auto& pair : positions) {
        auto it = nodes.find(pair.first);
        if (it != nodes.end()) it->second->setPos(pair.second);
    }
}
}
#endif
