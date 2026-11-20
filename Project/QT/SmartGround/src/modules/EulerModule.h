#ifndef EULERMODULE_H
#define EULERMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <QObject>
#include <QGraphicsTextItem>
#include <map>
#include <vector>

class EulerModule : public QObject, public IModule {
    Q_OBJECT
public:
    EulerModule();
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
    std::vector<AnimFrame> m_frames;
    std::map<int, GraphNode*> m_nodes;
    std::map<std::pair<int, int>, GraphEdge*> m_edges;
    std::map<int, ArrayBlock*> m_degBlocks;
    std::map<int, ArrayBlock*> m_indegBlocks;
    std::map<int, ArrayBlock*> m_outdegBlocks;
    std::vector<ArrayBlock*> m_stackBlocks;
    std::vector<ArrayBlock*> m_pathBlocks;
    QGraphicsTextItem* m_mathText = nullptr;
    QGraphicsTextItem* m_verdictText = nullptr;
    std::vector<std::pair<int, int>> m_edgeList;
    GraphModuleUtils::GraphOptions m_options;
    int m_n = 0;
    int m_m = 0;
    bool m_valid = false;
    bool m_directed = false;
};

#endif
