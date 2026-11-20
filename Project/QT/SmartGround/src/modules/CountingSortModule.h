#ifndef COUNTINGSORTMODULE_H
#define COUNTINGSORTMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <QObject>
#include <QGraphicsTextItem>
#include <map>
#include <vector>

class CountingSortModule : public QObject, public IModule {
    Q_OBJECT
public:
    CountingSortModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    QString getInputHint() const override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    std::vector<AnimFrame> runAlgorithm() override;
    void renderFrame(const AnimFrame& frame) override;
    void reset() override;
    void releaseSceneItems() override;
    bool supportsLiveEditing() const override { return true; }

private:
    std::vector<AnimFrame> m_frames;
    std::vector<int> m_data;
    std::vector<ArrayBlock*> m_srcBlocks;
    std::vector<ArrayBlock*> m_countBlocks;
    std::vector<ArrayBlock*> m_prefixBlocks;
    std::vector<ArrayBlock*> m_outBlocks;
    PointerBlock* m_scanPointer = nullptr;
    QGraphicsTextItem* m_mathText = nullptr;
    QGraphicsTextItem* m_labelA = nullptr;
    QGraphicsTextItem* m_labelC = nullptr;
    QGraphicsTextItem* m_labelP = nullptr;
    QGraphicsTextItem* m_labelO = nullptr;
    int m_maxValue = 0;
    bool m_valid = false;
};

#endif
