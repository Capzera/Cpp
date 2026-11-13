#ifndef SANDBOXGRAPHMODULE_H
#define SANDBOXGRAPHMODULE_H
#include "FreeStructureBase.h"
#include <map>
#include <vector>

class SandboxGraphModule : public FreeStructureBase {
    Q_OBJECT
public:
    SandboxGraphModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    QString getInputHint() const override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    bool supportsGraphOptions() const override { return true; }
    bool supportsDirected() const override { return true; }
    bool supportsBidirectional() const override { return true; }
    GraphModuleUtils::GraphOptions getGraphOptions() const override;
    void setGraphOptions(const GraphModuleUtils::GraphOptions& options) override;
    void reset() override;

private:
    void rebuildGraph();

    int m_n = 0;
    std::vector<std::pair<int, int>> m_edges;
    GraphModuleUtils::GraphOptions m_options;
    std::map<int, GraphNode*> m_nodes;
    std::vector<GraphEdge*> m_edgesItems;
};

#endif
