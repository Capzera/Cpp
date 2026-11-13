#ifndef HUNGARIANMODULE_H
#define HUNGARIANMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <map>
#include <vector>
#include <QObject>
#include <QGraphicsTextItem>

class HungarianModule : public QObject, public IModule {
    Q_OBJECT
public:
    HungarianModule();
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
    std::map<int, int> m_matchLeft;
    std::map<int, int> m_matchRight;
    QGraphicsTextItem* m_mathText = nullptr;
    QGraphicsTextItem* m_resultText = nullptr;
    GraphModuleUtils::GraphOptions m_options;
    int m_leftCount = 0;
    int m_rightCount = 0;
    bool m_valid = false;
    double m_leftX = 190.0;
    double m_rightX = 610.0;
    double m_topY = 170.0;
    double m_rowGap = 92.0;
};

#endif
