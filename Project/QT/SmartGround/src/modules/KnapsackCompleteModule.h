#ifndef KNAPSACKCOMPLETEMODULE_H
#define KNAPSACKCOMPLETEMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <vector>
#include <map>
#include <QObject>
#include <QGraphicsTextItem>

class KnapsackCompleteModule : public QObject, public IModule {
    Q_OBJECT
public:
    KnapsackCompleteModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    QString getInputHint() const override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    std::vector<AnimFrame> runAlgorithm() override;
    void renderFrame(const AnimFrame& frame) override;
    void reset() override;
    void releaseSceneItems() override;
    bool supportsLiveEditing() const override { return true; }
    void onLiveDataChanged(const QString& row, int index, int value) override;

private:
    int encode(int i, int j) const { return i * (m_cols + 1) + j; }

    std::vector<AnimFrame> m_frames;
    std::vector<std::vector<ArrayBlock*>> m_dpGrid;
    std::vector<ArrayBlock*> m_itemBlocks;
    QGraphicsTextItem* m_mathText = nullptr;
    int m_gridStartX = 0;
    int m_gridStartY = 0;
    int m_cols = 0;
    int m_n = 0;
    std::vector<int> m_w;
    std::vector<int> m_v;
    bool m_valid = false;
};

#endif
