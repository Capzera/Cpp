#ifndef TREESUBTREESIZEMODULE_H
#define TREESUBTREESIZEMODULE_H
#include "TreeDpBase.h"
#include <QObject>
#include <QGraphicsTextItem>
#include <map>
#include <vector>

class TreeSubtreeSizeModule : public QObject, public TreeDpBase {
    Q_OBJECT
public:
    TreeSubtreeSizeModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    QString getInputHint() const override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    std::vector<AnimFrame> runAlgorithm() override;
    void renderFrame(const AnimFrame& frame) override;
    void reset() override;
    void releaseSceneItems() override;
    std::map<int, QPointF> captureNodePositions() const override { return captureTreePositions(); }
    void restoreNodePositions(const std::map<int, QPointF>& positions) override { restoreTreePositions(positions); }

private:
    std::vector<AnimFrame> m_frames;
    std::map<int, std::vector<int>> m_children;
    std::vector<ArrayBlock*> m_orderBlocks;
    QGraphicsTextItem* m_mathText = nullptr;
    QGraphicsTextItem* m_orderTitle = nullptr;
    int m_maxId = 0;
    bool m_valid = false;
};

#endif
