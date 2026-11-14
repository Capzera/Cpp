#ifndef DIFFERENCEMODULE_H
#define DIFFERENCEMODULE_H
#include "TwoRowArrayModule.h"
#include <vector>

class DifferenceModule : public TwoRowArrayModule {
    Q_OBJECT
public:
    DifferenceModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    QString getInputHint() const override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    std::vector<AnimFrame> runAlgorithm() override;
    void renderFrame(const AnimFrame& frame) override;
    void onLiveDataChanged(const QString& row, int index, int value) override;

private:
    struct Query { int l, r, x; };
    std::vector<int> m_dataA;
    std::vector<int> m_dataD;
    std::vector<Query> m_queries;
};
#endif
