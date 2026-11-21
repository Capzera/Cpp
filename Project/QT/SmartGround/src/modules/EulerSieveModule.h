#ifndef EULERSIEVEMODULE_H
#define EULERSIEVEMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <QObject>
#include <QGraphicsTextItem>
#include <map>
#include <vector>

class EulerSieveModule : public QObject, public IModule {
    Q_OBJECT
public:
    EulerSieveModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    QString getInputHint() const override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    std::vector<AnimFrame> runAlgorithm() override;
    void renderFrame(const AnimFrame& frame) override;
    void reset() override;
    void releaseSceneItems() override;

    const std::map<int, ArrayBlock*>& cells() const { return m_cells; }
    const std::vector<ArrayBlock*>& primeBlocks() const { return m_primeBlocks; }

private:
    static const int kColumns = 10;
    void applyCross(ArrayBlock* block);
    void flushPendingCross();
    void stopCrossFlow();
    void resetGrid();
    std::vector<AnimFrame> m_frames;
    std::map<int, ArrayBlock*> m_cells;
    std::vector<ArrayBlock*> m_primeBlocks;
    std::vector<ArrayBlock*> m_pendingCross;
    std::vector<int> m_primeList;
    int m_crossDone = 0;
    QVariantAnimation* m_crossAnim = nullptr;
    QGraphicsTextItem* m_mathText = nullptr;
    QGraphicsTextItem* m_primeTitle = nullptr;
    QGraphicsTextItem* m_scanText = nullptr;
    QGraphicsTextItem* m_ruleText = nullptr;
    PointerBlock* m_ptrI = nullptr;
    PointerBlock* m_ptrP = nullptr;
    int m_n = 0;
    int m_rows = 0;
    bool m_valid = false;
};

#endif
