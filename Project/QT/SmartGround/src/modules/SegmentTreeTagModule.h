#ifndef SEGMENTTREETAGMODULE_H
#define SEGMENTTREETAGMODULE_H
#include "SegmentTreeEditBase.h"

class SegmentTreeTagModule : public SegmentTreeEditBase {
    Q_OBJECT
public:
    SegmentTreeTagModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    QString getInputHint() const override;
    std::vector<AnimFrame> runAlgorithm() override;
    void renderFrame(const AnimFrame& frame) override;

private:
    void buildTree(int node, int l, int r);
    void pushUp(int node);
    void pushDown(int node, int l, int r);
    void changeMul(int node, int l, int r, int lt, int rt, long long k);
    void changeAdd(int node, int l, int r, int lt, int rt, long long k);
};

#endif
