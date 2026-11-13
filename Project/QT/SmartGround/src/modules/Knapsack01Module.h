#ifndef KNAPSACK01MODULE_H
#define KNAPSACK01MODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <vector>
#include <map>
#include <QObject>
class Knapsack01Module : public QObject, public IModule {
    Q_OBJECT
public:
    Knapsack01Module();
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
    std::vector<std::vector<ArrayBlock*>> m_dpGrid;
    std::vector<ArrayBlock*> m_itemBlocks;
    QGraphicsTextItem* m_mathText;
    int m_gridStartX, m_gridStartY;
    int W, n;
    std::vector<int> w, v;
    bool m_valid;
};
#endif
