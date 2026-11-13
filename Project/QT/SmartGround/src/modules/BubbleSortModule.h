#ifndef BUBBLESORTMODULE_H
#define BUBBLESORTMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <vector>
#include <QObject>
class BubbleSortModule : public QObject, public IModule {
Q_OBJECT
public:
BubbleSortModule();
QString getModuleName() const override;
QString getDefaultInput() const override;
void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
std::vector<AnimFrame> runAlgorithm() override;
    void renderFrame(const AnimFrame& frame) override;
    void reset() override;
    void releaseSceneItems() override;
    QString getInputHint() const override;
    bool supportsLiveEditing() const override { return true; }
private:
std::vector<int> m_data;
std::vector<ArrayBlock*> m_blocks;
std::vector<AnimFrame> m_frames;
};
#endif
