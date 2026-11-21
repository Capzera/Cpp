#ifndef KRUSKALMODULE_H
#define KRUSKALMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <vector>
#include <map>
#include <QObject>
class KruskalModule : public QObject, public IModule {
    Q_OBJECT
public:
    KruskalModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
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
    QString getInputHint() const override;
    bool supportsLiveEditing() const override { return true; }
    std::map<int, QPointF> captureNodePositions() const override;
    void restoreNodePositions(const std::map<int, QPointF>& positions) override;
private:
    void paintFindPath(const std::vector<int>& path, const QColor& color);
    void paintCycleRing(int u, int v);
    std::vector<AnimFrame> m_frames;
    std::map<int, GraphNode*> m_nodes;
    std::map<std::pair<int,int>, GraphEdge*> m_edges;
    std::vector<ArrayBlock*> m_edgeBlocks;
    std::map<int, ArrayBlock*> m_parentBlocks;
    std::map<int, QColor> m_componentColors;
    PointerBlock* m_edgePointer;
    QGraphicsTextItem* m_mathText;
    std::vector<std::tuple<int,int,int>> m_edgeList;
    GraphModuleUtils::GraphOptions m_options;
    int m_n, m_m;
    bool m_valid;
};
#endif
