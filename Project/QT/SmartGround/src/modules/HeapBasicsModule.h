#ifndef HEAPBASICSMODULE_H
#define HEAPBASICSMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <vector>
#include <map>
#include <QObject>
class HeapBasicsModule : public QObject, public IModule {
    Q_OBJECT
public:
    HeapBasicsModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    std::vector<AnimFrame> runAlgorithm() override;
    void renderFrame(const AnimFrame& frame) override;
    void reset() override;

    void releaseSceneItems() override;
    QString getInputHint() const override;
private:
    std::vector<AnimFrame> m_frames;
    std::map<int, ArrayBlock*> m_arrayBlocks;
    std::map<int, GraphNode*> m_treeNodes;
    std::map<std::pair<int,int>, GraphEdge*> m_treeEdges;
    QGraphicsTextItem* m_mathText;
    int m_arrayStartX;
    int m_treeRootX;
    std::vector<int> m_heap;
    int n;
};
#endif
