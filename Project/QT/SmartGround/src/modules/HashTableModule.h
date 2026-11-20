#ifndef HASHTABLEMODULE_H
#define HASHTABLEMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <QObject>
#include <QGraphicsTextItem>
#include <map>
#include <vector>

class HashTableModule : public QObject, public IModule {
    Q_OBJECT
public:
    HashTableModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    QString getInputHint() const override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    std::vector<AnimFrame> runAlgorithm() override;
    void renderFrame(const AnimFrame& frame) override;
    void reset() override;
    void releaseSceneItems() override;

private:
    static constexpr double kCellWidth = 66.0;
    static constexpr double kCellHeight = 42.0;

    double cellWidth() const { return kCellWidth; }
    double cellHeight() const { return kCellHeight; }

    std::vector<AnimFrame> m_frames;
    std::vector<ArrayBlock*> m_slotBlocks;
    std::vector<ArrayBlock*> m_keyBlocks;
    QGraphicsTextItem* m_mathText = nullptr;
    QGraphicsTextItem* m_probeText = nullptr;
    PointerBlock* m_probePointer = nullptr;
    std::vector<int> m_keys;
    int m_capacity = 7;
    bool m_valid = false;
};

#endif
