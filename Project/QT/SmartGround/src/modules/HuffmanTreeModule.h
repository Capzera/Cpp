#ifndef HUFFMANTREEMODULE_H
#define HUFFMANTREEMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <QObject>
#include <QGraphicsTextItem>
#include <map>
#include <vector>

struct HuffmanNodeData {
    int id = 0;
    long long weight = 0;
    int left = -1;
    int right = -1;
    int parent = -1;
    bool original = true;
    int slot = -1;
    double x = 0.0;
    GraphNode* treeItem = nullptr;
    GraphNode* queueChip = nullptr;
};

struct HuffmanForest {
    std::map<int, int> rows;
    std::vector<int> queue;
};

struct HuffmanMergeStep {
    int a = 0;
    int b = 0;
    int parent = 0;
    int kind = 0;
    bool aLeaf = false;
    bool bLeaf = false;
    int liftedNode = -1;
    int liftDelta = 0;
    HuffmanForest before;
    HuffmanForest afterLift;
    HuffmanForest after;
};

struct HuffmanCodeRow {
    int leafId = 0;
    int originalIndex = 0;
    long long weight = 0;
    QString code;
    int codeLength = 0;
};

class HuffmanTreeModule : public QObject, public IModule {
    Q_OBJECT
public:
    HuffmanTreeModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    QString getInputHint() const override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    std::vector<AnimFrame> runAlgorithm() override;
    void renderFrame(const AnimFrame& frame) override;
    void reset() override;
    void releaseSceneItems() override;

private:
    static constexpr double kNodeR = 20.0;
    static constexpr double kCellW = 64.0;
    static constexpr double kCellH = 44.0;
    static constexpr double kArrayY = 150.0;
    static constexpr double kArrayLeft = 60.0;
    static constexpr double kCenterX = 296.0;
    static constexpr double kLeafSpan = 58.0;
    static constexpr double kQueueSpan = 62.0;
    static constexpr double kTreeBottomY = 574.0;
    static constexpr double kLevelH = 48.0;
    static constexpr double kQueueY = 664.0;
    static constexpr double kTableX = 624.0;
    static constexpr double kTableCol1 = 74.0;
    static constexpr double kTableCol2 = 112.0;
    static constexpr double kTableCol3 = 56.0;
    static constexpr double kTableRowH = 32.0;
    static constexpr double kTableTop = 300.0;
    static constexpr double kWplCellW = 64.0;
    static constexpr double kWplCellH = 42.0;
    static constexpr double kWplY = 736.0;

    HuffmanNodeData* nodeAt(int id);
    const HuffmanNodeData* nodeAt(int id) const;
    bool subtreeHas(int nodeId, int leafId) const;
    void collectSubtree(int id, std::vector<int>& out) const;
    void computeTree();
    void simulateStates();
    void computeCodes();
    void computeXPositions();
    void buildCodeTable();
    void clearCodeTable();
    void clearWplRow();
    void setLeafCode(int leafId, const QString& code);
    void revealPathLabels(int leafId);
    QPointF treePosOf(int id, const HuffmanForest& forest) const;
    QPointF queueSlotPos(int index) const;
    int queueIndexOf(int id, const std::vector<int>& queue) const;
    void applyTreeRows(const HuffmanForest& forest, int duration);
    void applyQueueOrder(const std::vector<int>& order, int duration);
    void hideQueueChips(const std::vector<int>& order);
    void completeEdgeGrowth();

    std::vector<AnimFrame> m_frames;
    std::vector<HuffmanNodeData> m_nodes;
    std::vector<HuffmanMergeStep> m_plan;
    std::vector<int> m_sortedLeaves;
    std::vector<ArrayBlock*> m_arrayCells;
    std::vector<HuffmanCodeRow> m_codeRows;
    std::vector<ArrayBlock*> m_codeFreqCells;
    std::vector<ArrayBlock*> m_codeCells;
    std::vector<ArrayBlock*> m_codeLenCells;
    std::vector<ArrayBlock*> m_wplCells;
    ArrayBlock* m_wplSumCell = nullptr;
    std::map<int, GraphNode*> m_treeNodes;
    std::map<int, GraphNode*> m_queueNodes;
    std::map<int, GraphEdge*> m_childEdges;
    std::map<int, QString> m_codeMap;

    QGraphicsTextItem* m_mathText = nullptr;
    QGraphicsTextItem* m_noteText = nullptr;
    QGraphicsTextItem* m_phaseText = nullptr;
    QGraphicsTextItem* m_resultText = nullptr;
    QGraphicsTextItem* m_arrayTitle = nullptr;
    QGraphicsTextItem* m_treeTitle = nullptr;
    QGraphicsTextItem* m_queueTitle = nullptr;
    QGraphicsTextItem* m_tableTitle = nullptr;
    QGraphicsTextItem* m_tableHead1 = nullptr;
    QGraphicsTextItem* m_tableHead2 = nullptr;
    QGraphicsTextItem* m_tableHead3 = nullptr;
    QGraphicsTextItem* m_wplTitle = nullptr;
    QGraphicsItem* m_treeFrame = nullptr;

    PointerBlock* m_ptrA = nullptr;
    PointerBlock* m_ptrB = nullptr;
    bool m_valid = false;
    int m_leafCount = 0;
    long long m_wplTotal = 0;
};

#endif
