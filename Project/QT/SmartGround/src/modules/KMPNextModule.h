#ifndef KMPNEXTMODULE_H
#define KMPNEXTMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <vector>
#include <map>
#include <QObject>
#include <QGraphicsRectItem>
class KMPNextModule : public QObject, public IModule {
    Q_OBJECT
public:
    KMPNextModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    std::vector<AnimFrame> runAlgorithm() override;
    void renderFrame(const AnimFrame& frame) override;
    void reset() override;

    void releaseSceneItems() override;
    QString getInputHint() const override;
private:
    std::vector<AnimFrame> m_frames;
    std::map<int, QGraphicsTextItem*> m_charLabels;
    std::map<int, QGraphicsRectItem*> m_charCells;
    std::map<int, ArrayBlock*> m_piBlocks;
    PointerBlock* m_ptrI;
    PointerBlock* m_ptrJ;
    QGraphicsTextItem* m_mathText;
    QString m_pattern;
    std::vector<int> m_pi;
    int m_startX;
    int m_cellW;
    bool m_valid;
};
#endif
