#ifndef CARTESIANMODULE_H
#define CARTESIANMODULE_H
#include "FreeStructureBase.h"
#include "render/CartesianElements.h"
#include <QObject>
#include <QPointer>

class CartesianModule : public FreeStructureBase {
    Q_OBJECT
public:
    CartesianModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    QString getInputHint() const override;

    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    std::vector<AnimFrame> runAlgorithm() override;
    void renderFrame(const AnimFrame& frame) override;
    void reset() override;
    void releaseSceneItems() override;

    CartesianEngine* engine() const { return m_engine; }
    bool isCartesian() const override { return true; }

    QString moduleSettingsTitle() const override;
    QVector<ModuleField> moduleSettingsFields() const override;
    void applyModuleSetting(const QString& key, const ModuleField& field) override;

    void setViewportRect(const QRectF& rect);
    void setViewScale(qreal scale);

signals:
    void objectsChanged();
    void statusMessage(const QString& text);
    void requestPointEditor(int index);
    void requestSegmentEditor(int index);
    void requestFunctionEditor(int index);
    void pointInspected(int index);
    void segmentInspected(int index);
    void curveInspected(int index);

private:
    CartesianEngine* m_engine = nullptr;
    QRectF m_viewportRect = QRectF(-600, -400, 1200, 800);
};

#endif
