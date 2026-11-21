#ifndef DIJKSTRANAIVEMODULE_H
#define DIJKSTRANAIVEMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <vector>
#include <map>
#include <QObject>
class DijkstraNaiveModule : public QObject, public IModule {
    Q_OBJECT
public:
    DijkstraNaiveModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    std::vector<AnimFrame> runAlgorithm() override;
    void renderFrame(const AnimFrame& frame) override;
    void reset() override;

    void releaseSceneItems() override;
    bool supportsGraphOptions() const override;
    bool supportsDirected() const override;
    GraphModuleUtils::GraphOptions getGraphOptions() const override;
    void setGraphOptions(const GraphModuleUtils::GraphOptions& options) override;
    QString getInputHint() const override;
    bool supportsLiveEditing() const override { return true; }
    std::map<int, QPointF> captureNodePositions() const override;
    void restoreNodePositions(const std::map<int, QPointF>& positions) override;
private:
    void buildCandidateRuler();
    void refreshCandidateRuler(const std::vector<int>& dist, const std::vector<bool>& used,
                               const std::vector<int>& doneOrder);
    void paintRulerSlot(int index, const QColor& color, bool strike);
    std::vector<AnimFrame> m_frames;
    std::map<int, GraphNode*> m_nodes;
    std::map<std::pair<int,int>, GraphEdge*> m_edges;
    std::map<int, ArrayBlock*> m_distBlocks;
    std::map<int, ArrayBlock*> m_doneBlocks;
    std::map<int, ArrayBlock*> m_parentBlocks;
    std::map<int, ArrayBlock*> m_candBlocks;
    std::vector<int> m_candOrder;
    PointerBlock* m_pickPointer;
    PointerBlock* m_relaxPointer;
    PointerBlock* m_candPointer;
    QGraphicsTextItem* m_mathText;
    QGraphicsTextItem* m_rulerText;
    std::vector<std::tuple<int,int,int>> m_edgeList;
    GraphModuleUtils::GraphOptions m_options;
    int m_head[101];
    int m_to[2005];
    int m_next[2005];
    int m_weight[2005];
    int m_n, m_m, m_src, m_edgeCount;
    bool m_valid;
};
#endif
