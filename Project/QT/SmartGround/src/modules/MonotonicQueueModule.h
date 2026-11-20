#ifndef MONOTONICQUEUEMODULE_H
#define MONOTONICQUEUEMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <QObject>
#include <QGraphicsTextItem>
#include <vector>

class MonotonicQueueModule : public QObject, public IModule {
    Q_OBJECT
public:
    MonotonicQueueModule();
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
    ArrayBlock* qCell(int slot) const;
    ArrayBlock* aCell(int index) const;

    std::vector<AnimFrame> m_frames;
    std::vector<int> m_data;
    std::vector<int> m_window;
    std::vector<ArrayBlock*> m_aBlocks;
    std::vector<ArrayBlock*> m_qBlocks;
    std::vector<ArrayBlock*> m_maxBlocks;
    QGraphicsTextItem* m_mathText = nullptr;
    QGraphicsTextItem* m_noteText = nullptr;
    QGraphicsTextItem* m_windowText = nullptr;
    PointerBlock* m_ptrI = nullptr;
    PointerBlock* m_ptrHead = nullptr;
    PointerBlock* m_ptrTail = nullptr;
    int m_k = 3;
    bool m_valid = false;
};

#endif
