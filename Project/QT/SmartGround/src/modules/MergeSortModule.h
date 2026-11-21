#ifndef MERGESORTMODULE_H
#define MERGESORTMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <vector>
#include <map>
#include <set>
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
    const std::map<int, ArrayBlock*>& blocks() const { return m_blocks; }
    const std::map<int, ArrayBlock*>& auxBlocks() const { return m_auxBlocks; }

private:
    void mergeSort(std::vector<int>& arr, int l, int r, int depth);
    void merge(std::vector<int>& arr, int l, int mid, int r);
    void resetRowColors();
    void markSegment(int l, int r, const QColor& color);
    void applyLayout(int duration);
    double cellX(int index) const;

    std::vector<AnimFrame> m_frames;
    std::map<int, ArrayBlock*> m_blocks;
    std::map<int, ArrayBlock*> m_auxBlocks;
    std::set<int> m_openBounds;
    QGraphicsTextItem* m_mathText = nullptr;
    QGraphicsTextItem* m_mergeText = nullptr;
    QGraphicsTextItem* m_levelText = nullptr;
    int m_startX = 0;
    int m_n = 0;
    std::vector<int> m_data;
};

#endif
