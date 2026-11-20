#ifndef BUCKETSORTMODULE_H
#define BUCKETSORTMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <QObject>
#include <QGraphicsTextItem>
#include <vector>

class BucketSortModule : public QObject, public IModule {
    Q_OBJECT
public:
    BucketSortModule();
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
    static constexpr int kBucketCount = 5;
    static constexpr int kBucketWidth = 20;
    static constexpr int kBucketSlots = 4;

    void layoutBuckets();
    ArrayBlock* bucketSlot(int bucket, int slot) const;

    std::vector<AnimFrame> m_frames;
    std::vector<int> m_data;
    std::vector<ArrayBlock*> m_srcBlocks;
    std::vector<ArrayBlock*> m_bucketHeads;
    std::vector<std::vector<ArrayBlock*>> m_bucketSlots;
    std::vector<ArrayBlock*> m_outBlocks;
    std::vector<std::vector<int>> m_bucketContent;
    QGraphicsTextItem* m_mathText = nullptr;
    QGraphicsTextItem* m_noteText = nullptr;
    PointerBlock* m_ptrI = nullptr;
    PointerBlock* m_ptrB = nullptr;
    bool m_valid = false;
};

#endif
