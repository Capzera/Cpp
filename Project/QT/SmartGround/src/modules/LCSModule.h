#ifndef LCSMODULE_H
#define LCSMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <QObject>
#include <QGraphicsTextItem>
#include <QPointer>
#include <QVector>
#include <vector>

class LCSModule : public QObject, public IModule {
    Q_OBJECT
public:
    LCSModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    QString getInputHint() const override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    std::vector<AnimFrame> runAlgorithm() override;
    void renderFrame(const AnimFrame& frame) override;
    void reset() override;
    void releaseSceneItems() override;

private:
    ArrayBlock* dpCell(int i, int j) const;

    std::vector<AnimFrame> m_frames;
    QString m_a;
    QString m_b;
    std::vector<std::vector<ArrayBlock*>> m_dpGrid;
    std::vector<ArrayBlock*> m_aCells;
    std::vector<ArrayBlock*> m_bCells;
    std::vector<ArrayBlock*> m_traceCells;
    std::vector<QGraphicsTextItem*> m_colTags;
    std::vector<QGraphicsTextItem*> m_rowTags;
    QPointer<PathTrail> m_tracePath;
    QVector<QPointF> m_tracePoints;    QGraphicsTextItem* m_mathText = nullptr;
    QGraphicsTextItem* m_noteText = nullptr;
    QGraphicsTextItem* m_resultText = nullptr;
    QGraphicsTextItem* m_traceLabel = nullptr;
    PointerBlock* m_ptrI = nullptr;
    PointerBlock* m_ptrJ = nullptr;
    QString m_lcsResult;
    double m_gridX = 260.0;
    double m_gridY = 290.0;
    double m_cellW = 50.0;
    double m_cellH = 34.0;
    double m_strCellW = 50.0;
    double m_strCellH = 34.0;
    int m_n = 0;
    int m_m = 0;
    bool m_valid = false;
};

#endif
