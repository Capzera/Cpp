#ifndef CIRCULARQUEUEMODULE_H
#define CIRCULARQUEUEMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <QObject>
#include <QGraphicsTextItem>
#include <vector>

class CircularQueueModule : public QObject, public IModule {
    Q_OBJECT
public:
    CircularQueueModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    QString getInputHint() const override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    std::vector<AnimFrame> runAlgorithm() override;
    void renderFrame(const AnimFrame& frame) override;
    void reset() override;
    void releaseSceneItems() override;

private:
    static constexpr int kCapacity = 6;

    ArrayBlock* cellAt(int index) const;
    void aimQueuePointers(int headIndex, int tailIndex);

    std::vector<AnimFrame> m_frames;
    std::vector<int> m_operations;
    std::vector<ArrayBlock*> m_cells;
    QGraphicsTextItem* m_mathText = nullptr;
    QGraphicsTextItem* m_noteText = nullptr;
    PointerBlock* m_ptrHead = nullptr;
    PointerBlock* m_ptrTail = nullptr;
    bool m_valid = false;
};

#endif
