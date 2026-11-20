#ifndef FORWARDSTARMODULE_H
#define FORWARDSTARMODULE_H
#include "IModule.h"
#include "GraphModuleUtils.h"
#include "render/GraphElements.h"
#include <QObject>
#include <QGraphicsTextItem>
#include <map>
#include <vector>

class ForwardStarModule : public QObject, public IModule {
    Q_OBJECT
public:
    ForwardStarModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    QString getInputHint() const override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    std::vector<AnimFrame> runAlgorithm() override;
    void renderFrame(const AnimFrame& frame) override;
    void reset() override;
    void releaseSceneItems() override;
    bool supportsGraphOptions() const override { return true; }
    bool supportsDirected() const override { return true; }
    bool supportsBidirectional() const override { return true; }
    GraphModuleUtils::GraphOptions getGraphOptions() const override { return m_options; }
    void setGraphOptions(const GraphModuleUtils::GraphOptions& options) override { m_options = options; }
    std::map<int, QPointF> captureNodePositions() const override;
    void restoreNodePositions(const std::map<int, QPointF>& positions) override;
    bool supportsLiveEditing() const override { return true; }
    void onLiveDataChanged(const QString& row, int index, int value) override;

private:
    void layoutArrays(int nodeCount, int edgeCapacity);
    void highlightCell(ArrayBlock* block, const QColor& color);
    std::vector<AnimFrame> m_frames;
    std::map<int, GraphNode*> m_nodes;
    std::map<int, ArrayBlock*> m_headBlocks;
    std::map<int, ArrayBlock*> m_toBlocks;
    std::map<int, ArrayBlock*> m_nxtBlocks;
    std::map<int, ArrayBlock*> m_weightBlocks;
    std::map<int, LinkLine*> m_toLines;
    std::map<int, LinkLine*> m_nxtLines;
    std::vector<GraphModuleUtils::EdgeInput> m_edges;
    GraphModuleUtils::GraphOptions m_options;
    QGraphicsTextItem* m_mathText = nullptr;
    PointerBlock* m_ptrK = nullptr;
    PointerBlock* m_ptrHead = nullptr;
    int m_n = 0;
    int m_m = 0;
    int m_src = 1;
    int m_capacity = 0;
    bool m_valid = false;
    double m_arraysX = 150.0;
    double m_arraysBottomY = 0.0;
};

#endif
