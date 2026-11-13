#ifndef TARJANCUTVERTEXBRIDGEMODULE_H
#define TARJANCUTVERTEXBRIDGEMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include "GraphModuleUtils.h"
#include <map>
#include <vector>
#include <QObject>
#include <QGraphicsTextItem>

class TarjanCutVertexBridgeModule : public QObject, public IModule {
    Q_OBJECT
public:
    TarjanCutVertexBridgeModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    QString getInputHint() const override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    std::vector<AnimFrame> runAlgorithm() override;
    void renderFrame(const AnimFrame& frame) override;
    void reset() override;
    void releaseSceneItems() override;
    bool supportsGraphOptions() const override;
    bool supportsDirected() const override;
    bool supportsBidirectional() const override;
    GraphModuleUtils::GraphOptions getGraphOptions() const override;
    void setGraphOptions(const GraphModuleUtils::GraphOptions& options) override;
    std::map<int, QPointF> captureNodePositions() const override;
    void restoreNodePositions(const std::map<int, QPointF>& positions) override;

private:
    void initGraph();
    void resetTraversalState();
    void addEdge(int u, int v);
    void tarjan(int u, int parent);
    ArrayBlock* dfnBlock(int u) const;
    ArrayBlock* lowBlock(int u) const;

    std::vector<AnimFrame> m_frames;
    std::map<int, GraphNode*> m_nodes;
    std::map<std::pair<int, int>, GraphEdge*> m_edges;
    std::map<int, ArrayBlock*> m_dfnBlocks;
    std::map<int, ArrayBlock*> m_lowBlocks;
    std::map<int, GraphEdge*> m_bridgeEdges;
    std::map<int, int> m_edgeKeyU;
    std::map<int, int> m_edgeKeyV;
    QGraphicsTextItem* m_mathText = nullptr;
    GraphModuleUtils::GraphOptions m_options;
    int m_n = 0;
    int m_m = 0;
    int m_head[205];
    int m_to[205];
    int m_next[205];
    int m_tot = 0;
    int m_dfn[205];
    int m_low[205];
    int m_timestamp = 0;
    int m_edgeCounter = 0;
    bool m_valid = false;
};

#endif
