#ifndef FASTPOWMODULE_H
#define FASTPOWMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <QObject>
#include <QGraphicsTextItem>
#include <QVariantAnimation>
#include <map>
#include <vector>

class FastPowModule : public QObject, public IModule {
    Q_OBJECT
public:
    FastPowModule();
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
    static bool fitsExactPower(long long base, long long exp);
    void layoutPanels();
    double bitCellX(int bitIndex) const;
    void applyBase(long long value);
    void applyExp(long long value);
    void applyRes(long long value);
    std::vector<AnimFrame> m_frames;
    std::vector<ArrayBlock*> m_bitBlocks;
    ArrayBlock* m_baseBlock = nullptr;
    ArrayBlock* m_resBlock = nullptr;
    ArrayBlock* m_expBlock = nullptr;
    QGraphicsTextItem* m_mathText = nullptr;
    QGraphicsTextItem* m_bitTitle = nullptr;
    QGraphicsTextItem* m_stepText = nullptr;
    PointerBlock* m_ptrK = nullptr;
    std::vector<int> m_data;
    long long m_base = 0;
    long long m_exp = 0;
    long long m_mod = 0;
    bool m_hasMod = false;
    int m_bitCount = 0;
    bool m_valid = false;
};

#endif
