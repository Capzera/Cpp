#ifndef SEGMENTTREEEDITBASE_H
#define SEGMENTTREEEDITBASE_H
#include "IModule.h"
#include "SegmentTreeCommon.h"
#include <QObject>
#include <QGraphicsTextItem>
#include <map>
#include <vector>

class SegmentTreeEditBase : public QObject, public IModule {
    Q_OBJECT
public:
    QString getModuleName() const override = 0;
    QString getDefaultInput() const override = 0;
    QString getInputHint() const override = 0;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    void reset() override;
    void releaseSceneItems() override;
    bool supportsLiveEditing() const override { return true; }
    void onLiveDataChanged(const QString& row, int index, int value) override;

protected:
    std::vector<AnimFrame> m_frames;
    void buildTreeScene(const QString& title, const QString& subtitle);
    void resetTreeScene();
    void highlightNode(int node, const QColor& color, const QColor& fill);
    void clearHighlights();
    void setNodeValue(int node, const QString& text);
    void setNodeTag(int node, const QString& text, const QColor& color);
    void clearNodeTag(int node);
    void setArrayValue(const QString& row, int slot, int value, const QColor& color);
    void showArrayValue(const QString& row, int slot, bool show);
    void setArrayColor(const QString& row, int slot, const QColor& color);
    ArrayBlock* arrayBlock(const QString& row, int slot);
    QPointF nodePos(int node) const;
    GraphNode* nodeItem(int node) const;
    void movePointerTo(int node);
    void setFormula(const QString& text);

    std::vector<SegmentTreeCommon::NodeInfo> m_nodes;
    std::map<int, GraphNode*> m_treeNodes;
    std::map<QString, std::map<int, ArrayBlock*>> m_arrays;
    std::map<int, int> m_nodeIntervalL;
    std::map<int, int> m_nodeIntervalR;
    QGraphicsTextItem* m_mathText = nullptr;
    PointerBlock* m_ptrNode = nullptr;
    std::vector<int> m_arr;
    std::vector<long long> m_tr;
    std::vector<long long> m_add;
    std::vector<long long> m_mul;
    std::vector<SegmentTreeCommon::Operation> m_ops;
    std::vector<int> m_nodeIds;
    int m_n = 0;
    int m_mod = 100;
    int m_slotCount = 0;
    bool m_valid = false;
    double m_nodeY = 230.0;
    double m_arrY = 120.0;
    std::map<QString, double> m_rowY;
    double m_slotW = 54.0;
    double m_slotStartX = 110.0;
};

#endif
