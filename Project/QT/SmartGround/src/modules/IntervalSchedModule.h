#ifndef INTERVALSCHEDMODULE_H
#define INTERVALSCHEDMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <QObject>
#include <QGraphicsTextItem>
#include <QPointer>
#include <vector>

class SegmentBar;
class QGraphicsRectItem;

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

    QString moduleSettingsTitle() const override;
    QVector<ModuleField> moduleSettingsFields() const override;
    void applyModuleSetting(const QString& key, const ModuleField& field) override;

private:
    struct Segment {
        int start = 0;
        int end = 0;
        SegmentBar* bar = nullptr;
        QGraphicsTextItem* nameText = nullptr;
    };

    void buildResultSlot();
    void clearResultSlot();
    void setResultSlot(int index, const QString& label, QColor color);
    void clearBands();
    void addBand(double x1, double x2, double y, double height, QColor color);

    std::vector<AnimFrame> m_frames;
    std::vector<Segment> m_segments;
    std::vector<QGraphicsRectItem*> m_bands;
    std::vector<QGraphicsTextItem*> m_greedyLabels;
    std::vector<QGraphicsTextItem*> m_naiveLabels;
    QGraphicsTextItem* m_mathText = nullptr;
    QGraphicsTextItem* m_scanText = nullptr;
    QGraphicsTextItem* m_resultText = nullptr;
    QGraphicsTextItem* m_counterText = nullptr;
    QGraphicsTextItem* m_greedyTitle = nullptr;
    QGraphicsTextItem* m_naiveTitle = nullptr;
    bool m_valid = false;
    bool m_showCounterExample = true;
    bool m_counterPhase = false;
    double m_timelineLeft = 220.0;
    double m_timelineWidth = 520.0;
    double m_timelineScale = 1.0;
};

#endif
