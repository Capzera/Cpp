#ifndef DSUPATHCOMPRESSIONMODULE_H
#define DSUPATHCOMPRESSIONMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <QObject>
#include <QGraphicsTextItem>
#include <map>
#include <vector>

class DSUPathCompressionModule : public QObject, public IModule {
    Q_OBJECT
public:
    DSUPathCompressionModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    QString getInputHint() const override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    std::vector<AnimFrame> runAlgorithm() override;
    void renderFrame(const AnimFrame& frame) override;
    void reset() override;
    void releaseSceneItems() override;
    std::map<int, QPointF> captureNodePositions() const override;
    void restoreNodePositions(const std::map<int, QPointF>& positions) override;
    bool supportsLiveEditing() const override { return true; }
    void onLiveDataChanged(const QString& row, int index, int value) override;

private:
    std::vector<AnimFrame> m_frames;
    std::map<int, GraphNode*> m_nodes;
    std::map<std::pair<int, int>, GraphEdge*> m_edges;
    std::map<int, ArrayBlock*> m_paBlocks;
    std::map<int, ArrayBlock*> m_szBlocks;
    std::vector<int> m_parents;
    GraphModuleUtils::GraphOptions m_options;
    QGraphicsTextItem* m_mathText = nullptr;
    PointerBlock* m_ptrX = nullptr;
    int m_n = 0;
    bool m_valid = false;
};

#endif
