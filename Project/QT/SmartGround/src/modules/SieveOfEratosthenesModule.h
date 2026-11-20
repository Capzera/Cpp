#ifndef SIEVEOFERATOSTHENESMODULE_H
#define SIEVEOFERATOSTHENESMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <QObject>
#include <QGraphicsTextItem>
#include <map>
#include <vector>

class SieveOfEratosthenesModule : public QObject, public IModule {
    Q_OBJECT
public:
    SieveOfEratosthenesModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    QString getInputHint() const override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    std::vector<AnimFrame> runAlgorithm() override;
    void renderFrame(const AnimFrame& frame) override;
    void reset() override;
    void releaseSceneItems() override;

private:
    static const int kColumns = 10;
    void applyCross(ArrayBlock* block);
    void flushPendingCross();
    void stopCrossFlow();
    std::vector<AnimFrame> m_frames;
    std::map<int, ArrayBlock*> m_cells;
    std::vector<ArrayBlock*> m_primeBlocks;
    std::vector<ArrayBlock*> m_pendingCross;
    int m_crossDone = 0;
    QVariantAnimation* m_crossAnim = nullptr;
    QGraphicsTextItem* m_mathText = nullptr;
    QGraphicsTextItem* m_primeTitle = nullptr;
    QGraphicsTextItem* m_scanText = nullptr;
    int m_n = 0;
    int m_rows = 0;
    bool m_valid = false;
};

#endif
