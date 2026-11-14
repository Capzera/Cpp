#ifndef FENWICKTREEMODULE_H
#define FENWICKTREEMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <vector>
#include <QObject>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QGraphicsLineItem>

class FenwickTreeModule : public QObject, public IModule {
    Q_OBJECT
public:
    FenwickTreeModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    QString getInputHint() const override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    std::vector<AnimFrame> runAlgorithm() override;
    void renderFrame(const AnimFrame& frame) override;
    void reset() override;
    void releaseSceneItems() override;
    bool supportsLiveEditing() const override { return true; }
    void onLiveDataChanged(const QString& row, int index, int value) override;

private:
    struct Operation {
        int op = 0;
        int x = 0;
        int delta = 0;
    };

    struct LevelSlot {
        int index = 0;
        int level = 0;
        int left = 0;
        int right = 0;
        ArrayBlock* block = nullptr;
    };

    static int lowbit(int x);
    static int levelOf(int x);

    void buildLevels();
    void layoutLevels();
    void drawGuides();
    void movePointerTo(int index);
    void clearAllStates();
    ArrayBlock* aBlock(int index) const;
    ArrayBlock* tBlock(int index) const;
    int levelOfBlock(const ArrayBlock* block) const;

    std::vector<AnimFrame> m_frames;
    std::vector<int> m_a;
    std::vector<int> m_t;
    std::vector<Operation> m_ops;
    std::vector<ArrayBlock*> m_aBlocks;
    std::vector<LevelSlot> m_levels;
    std::vector<QGraphicsItem*> m_guides;
    QGraphicsTextItem* m_mathText = nullptr;
    QGraphicsTextItem* m_accText = nullptr;
    QGraphicsTextItem* m_formulaText = nullptr;
    PointerBlock* m_ptrIndex = nullptr;
    int m_n = 0;
    int m_maxLevel = 0;
    bool m_valid = false;
    double m_cellW = 62.0;
    double m_cellH = 40.0;
    double m_levelStep = 88.0;
    double m_arrY = 520.0;
    double m_startX = 140.0;
};

#endif
