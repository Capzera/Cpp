#ifndef HEAPADJUSTMODULE_H
#define HEAPADJUSTMODULE_H
#include "HeapTreeBase.h"
#include <QObject>

class HeapAdjustModule : public QObject, public HeapTreeBase {
    Q_OBJECT
public:
    HeapAdjustModule();
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
    std::vector<AnimFrame> m_frames;
    QGraphicsTextItem* m_mathText = nullptr;
    bool m_valid = false;
};

#endif
