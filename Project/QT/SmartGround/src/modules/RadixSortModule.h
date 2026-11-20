#ifndef RADIXSORTMODULE_H
#define RADIXSORTMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <QObject>
#include <QGraphicsTextItem>
#include <vector>

class RadixSortModule : public QObject, public IModule {
    Q_OBJECT
public:
    RadixSortModule();
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
    static constexpr int kDigitBuckets = 10;

    ArrayBlock* bucketSlot(int digit, int slot) const;

    std::vector<AnimFrame> m_frames;
    std::vector<int> m_data;
    std::vector<ArrayBlock*> m_srcBlocks;
    std::vector<ArrayBlock*> m_dstBlocks;
    std::vector<ArrayBlock*> m_bucketHeads;
    std::vector<std::vector<ArrayBlock*>> m_bucketSlots;
    QGraphicsTextItem* m_mathText = nullptr;
    QGraphicsTextItem* m_noteText = nullptr;
    PointerBlock* m_ptrI = nullptr;
    PointerBlock* m_ptrD = nullptr;
    int m_maxDigit = 0;
    bool m_valid = false;
};

#endif
