#ifndef MERGESORTMODULE_H
#define MERGESORTMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <vector>
#include <map>
#include <QObject>
class MergeSortModule : public QObject, public IModule {
    Q_OBJECT
public:
    MergeSortModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    std::vector<AnimFrame> runAlgorithm() override;
    void renderFrame(const AnimFrame& frame) override;
    void reset() override;

    void releaseSceneItems() override;
    QString getInputHint() const override;
    bool supportsLiveEditing() const override { return true; }
private:
    void mergeSort(std::vector<int>& arr, int l, int r, int depth);
    void merge(std::vector<int>& arr, int l, int mid, int r);
    std::vector<AnimFrame> m_frames;
    std::map<int, ArrayBlock*> m_blocks;
    std::map<int, ArrayBlock*> m_auxBlocks;
    QGraphicsTextItem* m_mathText;
    int m_startX;
    std::vector<int> m_data;
};
#endif
