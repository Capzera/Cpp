#ifndef STRINGHASHMODULE_H
#define STRINGHASHMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <QObject>
#include <QGraphicsTextItem>
#include <vector>

class StringHashModule : public QObject, public IModule {
    Q_OBJECT
public:
    StringHashModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    QString getInputHint() const override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    std::vector<AnimFrame> runAlgorithm() override;
    void renderFrame(const AnimFrame& frame) override;
    void reset() override;
    void releaseSceneItems() override;

private:
    static constexpr long long kP = 131;
    static constexpr long long kMod = 998244353;

    static long long getHash(const std::vector<long long>& h, int l, int r, const std::vector<long long>& p);

    ArrayBlock* powerCell(int i) const;
    ArrayBlock* hashCell(int table, int i) const;
    ArrayBlock* charCell(int table, int i) const;

    std::vector<AnimFrame> m_frames;
    QString m_s1;
    QString m_s2;
    int m_n1 = 0;
    int m_n2 = 0;

    std::vector<ArrayBlock*> m_powCells;
    std::vector<ArrayBlock*> m_s1Cells;
    std::vector<ArrayBlock*> m_h1Cells;
    std::vector<ArrayBlock*> m_s2Cells;
    std::vector<ArrayBlock*> m_h2Cells;

    QGraphicsTextItem* m_mathText = nullptr;
    QGraphicsTextItem* m_noteText = nullptr;
    QGraphicsTextItem* m_resultText = nullptr;
    QGraphicsTextItem* m_s1Label = nullptr;
    QGraphicsTextItem* m_s2Label = nullptr;
    QGraphicsTextItem* m_pLabel = nullptr;
    QGraphicsTextItem* m_h1Label = nullptr;
    QGraphicsTextItem* m_h2Label = nullptr;

    PointerBlock* m_ptrLen = nullptr;
    PointerBlock* m_ptrP1 = nullptr;
    PointerBlock* m_ptrP2 = nullptr;
    PointerBlock* m_ptrI = nullptr;
    PointerBlock* m_ptrJ = nullptr;
    bool m_valid = false;
};

#endif
