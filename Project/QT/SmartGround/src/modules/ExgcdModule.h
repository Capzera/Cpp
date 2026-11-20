#ifndef EXGCDMODULE_H
#define EXGCDMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <QObject>
#include <QGraphicsTextItem>
#include <vector>

struct ExgcdStepRow {
    long long dividend = 0;
    long long divisor = 0;
    long long quotient = 0;
    long long remainder = 0;
};

struct ExgcdTerm {
    int index = 0;
    long long coef = 0;
};

struct ExgcdBackStep {
    int sourceRow = -1;
    QString fragment;
    QString newTerms;
    QString simplified;
    QString prefixText;
    int fragStart = 0;
    int fragPrefixLen = 0;
};

class ExgcdModule : public QObject, public IModule {
    Q_OBJECT
public:
    ExgcdModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    QString getInputHint() const override;
    QString moduleSettingsTitle() const override;
    QVector<ModuleField> moduleSettingsFields() const override;
    void applyModuleSetting(const QString& key, const ModuleField& field) override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    std::vector<AnimFrame> runAlgorithm() override;
    void renderFrame(const AnimFrame& frame) override;
    void reset() override;
    void releaseSceneItems() override;

private:
    void computeChain();
    void buildBackSteps();
    void buildLayoutBlocks();
    void clearLayoutBlocks();
    QString fragmentText(int index) const;
    QString renderTerms(const std::vector<ExgcdTerm>& terms, int substIndex, int* fragStart) const;

    std::vector<AnimFrame> m_frames;
    std::vector<ExgcdStepRow> m_rows;
    std::vector<long long> m_chain;
    std::vector<ExgcdBackStep> m_backSteps;
    std::vector<QGraphicsTextItem*> m_eqLines;
    std::vector<QGraphicsTextItem*> m_eqArrows;
    std::vector<QGraphicsTextItem*> m_leftLines;
    std::vector<QGraphicsTextItem*> m_rightLines;
    std::vector<QGraphicsTextItem*> m_backLines;
    QGraphicsTextItem* m_mathText = nullptr;
    QGraphicsTextItem* m_eqTitle = nullptr;
    QGraphicsTextItem* m_gcdTitle = nullptr;
    QGraphicsTextItem* m_gcdHeadLeft = nullptr;
    QGraphicsTextItem* m_gcdHeadRight = nullptr;
    QGraphicsTextItem* m_backTitle = nullptr;
    QGraphicsTextItem* m_noteText = nullptr;
    QGraphicsTextItem* m_resultText = nullptr;
    PointerBlock* m_ptrRow = nullptr;
    long long m_a = 0;
    long long m_b = 0;
    long long m_gcd = 0;
    long long m_x = 0;
    long long m_y = 0;
    int m_gcdIndex = -1;
    bool m_inverseMode = true;
    bool m_valid = false;
};

#endif
