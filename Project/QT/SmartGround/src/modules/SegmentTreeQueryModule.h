#ifndef SEGMENTTREEQUERYMODULE_H
#define SEGMENTTREEQUERYMODULE_H
#include "SegmentTreeEditBase.h"

class SegmentTreeQueryModule : public SegmentTreeEditBase {
    Q_OBJECT
public:
    SegmentTreeQueryModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    QString getInputHint() const override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    std::vector<AnimFrame> runAlgorithm() override;
    void renderFrame(const AnimFrame& frame) override;

private:
    void buildTree(int node, int l, int r);
    void pushUp(int node);
    void pushDown(int node, int l, int r);
    long long querySum(int node, int l, int r, int lt, int rt);
    void parseQuery();

    ArrayBlock* m_ansBlock = nullptr;
    int m_lt = 0;
    int m_rt = 0;
    long long m_answer = 0;
};

#endif
