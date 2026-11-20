#ifndef DIJKSTRAHEAPMODULE_H
#define DIJKSTRAHEAPMODULE_H
#include "IModule.h"
#include "GraphModuleUtils.h"
#include "render/GraphElements.h"
#include <QObject>
#include <QGraphicsTextItem>
#include <map>
#include <vector>

class DijkstraHeapModule : public QObject, public IModule {
    Q_OBJECT
public:
    DijkstraHeapModule();
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
    bool supportsLiveEditing() const override { return true; }
    void onLiveDataChanged(const QString& row, int index, int value) override;

private:
    void layoutPqView(int capacity);
    void clearPqSlots();
    void writePqSlot(int slot, int dist, int node);
    void shiftPqFrom(int slot);
    std::vector<AnimFrame> m_frames;
    std::map<int, GraphNode*> m_nodes;
    std::map<std::pair<int, int>, GraphEdge*> m_edges;
    std::map<int, ArrayBlock*> m_distBlocks;
    std::map<int, ArrayBlock*> m_pqBlocks;
    std::vector<ArrayBlock*> m_pqOrder;
    std::vector<ArrayBlock*> m_pqSpare;
    std::vector<GraphModuleUtils::EdgeInput> m_edgesInput;
    GraphModuleUtils::GraphOptions m_options;
    QGraphicsTextItem* m_mathText = nullptr;
    QGraphicsTextItem* m_heapText = nullptr;
    QGraphicsTextItem* m_pqTitle = nullptr;
    PointerBlock* m_ptrU = nullptr;
    int m_n = 0;
    int m_m = 0;
    int m_src = 1;
    int m_heapCapacity = 0;
    double m_pqCellW = 98.0;
    double m_pqCellH = 42.0;
    double m_pqX = 80.0;
    double m_pqY = 470.0;
    bool m_valid = false;
};

#endif
