#ifndef SEGMENTTREEBUILDMODULE_H
#define SEGMENTTREEBUILDMODULE_H
#include "IModule.h"
#include "SegmentTreeCommon.h"
#include <vector>
#include <map>
#include <QObject>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>

class SegmentTreeBuildModule : public QObject, public IModule {
    Q_OBJECT
public:
    SegmentTreeBuildModule();
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
    void refreshNodeLabels();
    QPointF arraySlotPos(int index) const;
    void movePointerTo(int node);
    ArrayBlock* trBlock(int nodeId) const;
    ArrayBlock* mulBlock(int nodeId) const;
    GraphNode* treeNode(int nodeId) const;

    std::vector<AnimFrame> m_frames;
    std::vector<SegmentTreeCommon::NodeInfo> m_nodes;
    std::map<int, GraphNode*> m_treeNodes;
    std::map<int, ArrayBlock*> m_trBlocks;
    std::map<int, ArrayBlock*> m_mulBlocks;
    std::map<int, QGraphicsRectItem*> m_aCells;
    std::vector<ArrayBlock*> m_aBlocks;
    QGraphicsTextItem* m_mathText = nullptr;
    PointerBlock* m_ptrNode = nullptr;
    std::vector<int> m_arr;
    std::vector<long long> m_tr;
    std::vector<long long> m_mul;
    int m_n = 0;
    int m_mod = 100;
    int m_slotCount = 0;
    bool m_valid = false;
    std::vector<int> m_nodeIds;
    double m_nodeY = 230.0;
    double m_trY = 560.0;
    double m_mulY = 638.0;
    double m_arrY = 120.0;
};

#endif
