#ifndef BIPARTITEMODULE_H
#define BIPARTITEMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <QObject>
#include <QGraphicsTextItem>
#include <QPointer>
#include <map>
#include <vector>

class BipartiteModule : public QObject, public IModule {
    Q_OBJECT
public:
    BipartiteModule();
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
    void applyColor(int node, int color);
    QColor colorFor(int color) const;
    QString blueText() const;
    QString orangeText() const;
    std::vector<int> extractOddCycle(int u, int v) const;
    void buildQueueRow(int capacity);
    void clearQueueRow();
    void setQueueSlot(int index, int value, bool active);
    void setQueuePointer(int index, const QString& label, PointerBlock* pointer);
public:
    const std::map<int, ArrayBlock*>& queueBlocks() const { return m_queueBlocks; }

    std::vector<AnimFrame> m_frames;
    std::map<int, GraphNode*> m_nodes;
    std::map<std::pair<int, int>, GraphEdge*> m_edges;
    std::map<int, ArrayBlock*> m_colorBlocks;
    std::map<int, ArrayBlock*> m_queueBlocks;
    QGraphicsTextItem* m_mathText = nullptr;
    QGraphicsTextItem* m_verdictText = nullptr;
    QGraphicsTextItem* m_cycleText = nullptr;
    QPointer<PathTrail> m_cycleTrail;
    PointerBlock* m_queueHeadPtr = nullptr;
    PointerBlock* m_queueTailPtr = nullptr;
    std::vector<std::pair<int, int>> m_edgeList;
    GraphModuleUtils::GraphOptions m_options;
    int m_head[101];
    int m_to[2005];
    int m_next[2005];
    int m_color[101];
    int m_parent[101];
    int m_depth[101];
    int m_n = 0;
    int m_m = 0;
    int m_edgeCount = 0;
    int m_queueCap = 0;
    bool m_valid = false;
};

#endif
