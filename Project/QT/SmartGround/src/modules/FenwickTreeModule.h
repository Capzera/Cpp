#ifndef FENWICKTREEMODULE_H
#define FENWICKTREEMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <vector>
#include <map>
#include <QObject>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>

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

    static int lowbit(int x);
    void movePointerTo(int index);
    void refreshCoverBands();
    void clearAllStates();
    ArrayBlock* aBlock(int index) const;
    ArrayBlock* tBlock(int index) const;

    std::vector<AnimFrame> m_frames;
    std::vector<int> m_a;
    std::vector<int> m_t;
    std::vector<Operation> m_ops;
    std::vector<ArrayBlock*> m_aBlocks;
    std::vector<ArrayBlock*> m_tBlocks;
    std::vector<QGraphicsRectItem*> m_coverBands;
    QGraphicsTextItem* m_mathText = nullptr;
    QGraphicsTextItem* m_accText = nullptr;
    PointerBlock* m_ptrIndex = nullptr;
    int m_n = 0;
    bool m_valid = false;
    double m_cellW = 60.0;
    double m_arrY = 210.0;
    double m_treeY = 360.0;
    double m_bandY = 272.0;
    double m_startX = 140.0;
};

#endif
