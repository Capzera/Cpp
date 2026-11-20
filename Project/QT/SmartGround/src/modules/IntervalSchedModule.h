#ifndef INTERVALSCHEDMODULE_H
#define INTERVALSCHEDMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <QObject>
#include <QGraphicsTextItem>
#include <vector>

class SegmentBar;

class IntervalSchedModule : public QObject, public IModule {
    Q_OBJECT
public:
    IntervalSchedModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    QString getInputHint() const override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    std::vector<AnimFrame> runAlgorithm() override;
    void renderFrame(const AnimFrame& frame) override;
    void reset() override;
    void releaseSceneItems() override;

private:
    struct Segment {
        int start = 0;
        int end = 0;
        SegmentBar* bar = nullptr;
        QGraphicsTextItem* nameText = nullptr;
    };

    std::vector<AnimFrame> m_frames;
    std::vector<Segment> m_segments;
    QGraphicsTextItem* m_mathText = nullptr;
    QGraphicsTextItem* m_scanText = nullptr;
    QGraphicsTextItem* m_resultText = nullptr;
    bool m_valid = false;
};

#endif
