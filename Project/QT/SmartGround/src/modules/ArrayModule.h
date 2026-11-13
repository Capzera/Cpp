#ifndef ARRAYMODULE_H
#define ARRAYMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <vector>
#include <QObject>
class ArrayModule : public QObject, public IModule {
Q_OBJECT
public:
ArrayModule();
virtual ~ArrayModule() = default;
QString getModuleName() const override = 0;
    QString getDefaultInput() const override = 0;
    QString getInputHint() const override;
void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
std::vector<AnimFrame> runAlgorithm() override = 0;
void renderFrame(const AnimFrame& frame) override;
void reset() override;

void releaseSceneItems() override;
protected:
std::vector<int> m_data;
std::vector<ArrayBlock*> m_blocks;
std::vector<AnimFrame> m_frames;
};
#endif
