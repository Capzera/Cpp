#ifndef LISMODULE_H
#define LISMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <vector>
#include <map>
#include <QObject>
class LISModule : public QObject, public IModule {
    Q_OBJECT
public:
    LISModule();
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
    std::vector<AnimFrame> m_frames;
    std::map<int, ArrayBlock*> m_arrBlocks;
    std::map<int, ArrayBlock*> m_dpBlocks;
    std::map<int, ArrayBlock*> m_tailBlocks;
    PointerBlock* m_ptrI;
    PointerBlock* m_ptrJ;
    QGraphicsTextItem* m_mathText;
    int m_startX;
    std::vector<int> m_data;
    bool m_valid;
};
#endif
