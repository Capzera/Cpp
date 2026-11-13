#ifndef KMPMATCHMODULE_H
#define KMPMATCHMODULE_H
#include "IModule.h"
#include <QObject>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <map>
#include <vector>

class KMPMatchModule : public QObject, public IModule {
    Q_OBJECT
public:
    KMPMatchModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    QString getInputHint() const override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    std::vector<AnimFrame> runAlgorithm() override;
    void renderFrame(const AnimFrame& frame) override;
    void reset() override;
    void releaseSceneItems() override;

private:
    void layoutPatternRow(double offset);
    void syncPatternRow(int i, int j);
    void refreshPatternHighlight();

    std::vector<AnimFrame> m_frames;
    std::vector<QGraphicsRectItem*> m_sCells;
    std::vector<QGraphicsRectItem*> m_pCells;
    std::vector<QGraphicsTextItem*> m_sChars;
    std::vector<QGraphicsTextItem*> m_pChars;
    std::map<int, ArrayBlock*> m_piBlocks;
    std::vector<ArrayBlock*> m_resultBlocks;
    QGraphicsTextItem* m_mathText = nullptr;
    QGraphicsTextItem* m_pTitle = nullptr;
    PointerBlock* m_ptrI = nullptr;
    PointerBlock* m_ptrJ = nullptr;
    QString m_s;
    QString m_p;
    std::vector<int> m_pi;
    int m_n = 0;
    int m_m = 0;
    int m_resultCount = 0;
    bool m_valid = false;
    double m_cellW = 52.0;
    double m_sX = 150.0;
    double m_pX = 150.0;
    double m_sY = 140.0;
    double m_pY = 280.0;
    double m_piY = 430.0;
    double m_patternOffset = 0.0;
};

#endif
