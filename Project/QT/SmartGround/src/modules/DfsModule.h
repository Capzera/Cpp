#ifndef DFSMODULE_H
#define DFSMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <QObject>
#include <QGraphicsTextItem>
#include <map>
#include <vector>

class DfsModule : public QObject, public IModule {
    Q_OBJECT
public:
    DfsModule();
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
    GraphModuleUtils::GraphOptions getGraphOptions() const override;
    void setGraphOptions(const GraphModuleUtils::GraphOptions& options) override;
    std::map<int, QPointF> captureNodePositions() const override;
    void restoreNodePositions(const std::map<int, QPointF>& positions) override;

private:
    std::vector<AnimFrame> m_frames;
    std::map<int, GraphNode*> m_nodes;
    std::map<std::pair<int, int>, GraphEdge*> m_edges;
    std::map<int, ArrayBlock*> m_stackBlocks;
    std::map<int, ArrayBlock*> m_visBlocks;
    std::map<int, ArrayBlock*> m_orderBlocks;
    PointerBlock* m_stackPointer = nullptr;
    PointerBlock* m_curPointer = nullptr;
    QGraphicsTextItem* m_mathText = nullptr;
    std::vector<std::pair<int, int>> m_edgeList;
    GraphModuleUtils::GraphOptions m_options;
    int m_head[101];
    int m_to[2005];
    int m_next[2005];
    int m_n = 0;
    int m_m = 0;
    int m_src = 1;
    int m_edgeCount = 0;
    bool m_valid = false;
};

#endif
