#ifndef DSUBASICMODULE_H
#define DSUBASICMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <vector>
#include <map>
#include <QObject>
class DSUBasicModule : public QObject, public IModule {
    Q_OBJECT
public:
    DSUBasicModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    std::vector<AnimFrame> runAlgorithm() override;
    void renderFrame(const AnimFrame& frame) override;
    void reset() override;

    void releaseSceneItems() override;
    QString getInputHint() const override;
    std::map<int, QPointF> captureNodePositions() const override;
    void restoreNodePositions(const std::map<int, QPointF>& positions) override;
private:
    int find(std::vector<int>& parent, int x);
    std::vector<AnimFrame> m_frames;
    std::map<int, GraphNode*> m_nodes;
    std::vector<GraphEdge*> m_edges;
    QGraphicsTextItem* m_mathText;
    int n;
};
#endif
