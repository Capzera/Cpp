#ifndef FREESTRUCTUREBASE_H
#define FREESTRUCTUREBASE_H
#include "IModule.h"
#include <QObject>
#include <QGraphicsTextItem>
#include <vector>

class FreeStructureBase : public QObject, public IModule {
    Q_OBJECT
public:
    QString getModuleName() const override = 0;
    QString getDefaultInput() const override = 0;
    QString getInputHint() const override = 0;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    std::vector<AnimFrame> runAlgorithm() override;
    void renderFrame(const AnimFrame& frame) override;
    void reset() override;
    void releaseSceneItems() override;

protected:
    void showHint(const QString& message);
    void parseKeyValues(const QString& inputData, std::vector<QPair<QString, QString>>& out);

    QGraphicsTextItem* m_mathText = nullptr;
    std::vector<AnimFrame> m_frames;
    bool m_valid = false;
};

#endif
