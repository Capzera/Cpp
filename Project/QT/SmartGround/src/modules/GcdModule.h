#ifndef GCDMODULE_H
#define GCDMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <QObject>
#include <QGraphicsTextItem>
#include <vector>

class GcdModule : public QObject, public IModule {
    Q_OBJECT
public:
    GcdModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    QString getInputHint() const override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    std::vector<AnimFrame> runAlgorithm() override;
    void renderFrame(const AnimFrame& frame) override;
    void reset() override;
    void releaseSceneItems() override;

private:
    void refreshBoxes();

    std::vector<AnimFrame> m_frames;
    QGraphicsTextItem* m_mathText = nullptr;
    QGraphicsTextItem* m_aLabel = nullptr;
    QGraphicsTextItem* m_bLabel = nullptr;
    QGraphicsTextItem* m_rLabel = nullptr;
    QGraphicsTextItem* m_aValue = nullptr;
    QGraphicsTextItem* m_bValue = nullptr;
    QGraphicsTextItem* m_rValue = nullptr;
    ArrayBlock* m_aBox = nullptr;
    ArrayBlock* m_bBox = nullptr;
    ArrayBlock* m_rBox = nullptr;
    int m_a = 0;
    int m_b = 0;
    bool m_valid = false;
};

#endif
