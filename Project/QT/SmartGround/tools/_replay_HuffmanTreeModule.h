#ifndef HUFFMANTREEMODULE_H
#define HUFFMANTREEMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <QObject>
#include <QGraphicsTextItem>
#include <functional>
#include <map>
#include <set>
#include <vector>

struct HuffmanNodeData {
    int id = 0;
    long long weight = 0;
    int left = -1;
    int right = -1;
    int parent = -1;
    bool original = true;
    GraphNode* treeItem = nullptr;
    GraphNode* queueItem = nullptr;
};

struct HuffmanSimState {
    std::map<int, int> levels;
    std::set<int> inTree;
    std::vector<int> entries;
    std::vector<int> queue;
    std::map<int, double> xs;
    std::map<int, int> compOrder;
    std::vector<int> roots;
};

struct HuffmanMergeStep {
    int a = 0;
    int b = 0;
    int parent = 0;
    int kind = 0;
    bool aLeaf = false;
    bool bLeaf = false;
    int promoted = -1;
    int promoteDelta = 0;
    HuffmanSimState mid;
    HuffmanSimState promo;
    HuffmanSimState after;
};

struct HuffmanCodeRow {
    int leafId = 0;
    int originalIndex = 0;
    long long weight = 0;
    QString code;
    int codeLength = 0;
};

struct HuffmanExtent {
    double left = 0.0;
    double right = 0.0;
};

struct HuffmanLayoutInfo {
    HuffmanExtent extent;
    double leftOffset = 0.0;
    double rightOffset = 0.0;
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
    static constexpr double kCellW = 64.0;
    static constexpr double kCellH = 44.0;
    static constexpr double kArrayY = 150.0;
    static constexpr double kArrayLeft = 56.0;
    static constexpr double kHoldY = 86.0;
    static constexpr double kCenterX = 296.0;
    static constexpr double kLeafSpan = 58.0;
    static constexpr double kTreeGap = 16.0;
    static constexpr double kQueueSpan = 60.0;
    static constexpr double kTreeBottomY = 552.0;
    static constexpr double kLevelH = 54.0;
    static constexpr double kQueueY = 664.0;
    static constexpr double kTableX = 624.0;
    static constexpr double kTableCol1 = 74.0;
    static constexpr double kTableCol2 = 112.0;
    static constexpr double kTableCol3 = 56.0;
    static constexpr double kTableRowH = 32.0;
    static constexpr double kTableTop = 300.0;
    static constexpr double kWplCellW = 64.0;
    static constexpr double kWplCellH = 42.0;
    static constexpr double kWplY = 712.0;

    HuffmanNodeData* nodeAt(int id);
    const HuffmanNodeData* nodeAt(int id) const;
    bool subtreeHas(int nodeId, int leafId) const;
    void computeTree();
    void computeCodes();
    void simulateStates();
    void computeStateLayout(HuffmanSimState& state) const;
    void liftSubtree(int id, int delta, std::map<int, int>& levels) const;
    double subtreeWidth(int id, std::map<int, double>& memo) const;
    HuffmanLayoutInfo subtreeLayout(int id, std::map<int, HuffmanLayoutInfo>& memo) const;
    HuffmanExtent subtreeExtent(int id, std::map<int, HuffmanExtent>& memo) const;
    double childSeparation(const HuffmanExtent& left, const HuffmanExtent& right) const;
    void layoutComponent(int id, double rootX, std::map<int, double>& xs,
                         std::map<int, double>& memo) const;
    void layoutComponentSym(int id, double rootX, std::map<int, double>& xs,
                            std::map<int, HuffmanLayoutInfo>& memo) const;
    std::vector<int> componentRoots(const HuffmanSimState& state) const;
    QPointF treePosOf(int nodeId, const HuffmanSimState& state) const;
    QPointF queueSlotPos(int index, int count) const;
    QPointF queuePosOf(int nodeId, const std::vector<int>& order) const;
    void applyTreePositions(const HuffmanSimState& state, int duration);
    void applyQueuePositions(const std::vector<int>& order, int duration);
    void buildCodeTable();
    void clearCodeTable();
    void clearWplRow();
    void setLeafCode(int leafId, const QString& code);
    void revealPathLabels(int leafId);
    GraphNode* makeTreeNode(int id);
    GraphNode* makeQueueNode(int id);
    void syncTreeNodesToState(const HuffmanSimState& state);
    void revealEdgesFor(int parentId);

    std::vector<AnimFrame> m_frames;
    std::vector<HuffmanNodeData> m_nodes;
    std::vector<HuffmanMergeStep> m_plan;
    std::vector<HuffmanSimState> m_states;
    std::vector<int> m_sortedLeaves;
    std::map<int, int> m_compRank;
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
    std::map<int, QGraphicsTextItem*> m_hoverTexts;

    QGraphicsTextItem* m_mathText = nullptr;
    QGraphicsTextItem* m_noteText = nullptr;
    QGraphicsTextItem* m_phaseText = nullptr;
    QGraphicsTextItem* m_resultText = nullptr;
    QGraphicsTextItem* m_arrayTitle = nullptr;
    QGraphicsTextItem* m_tableTitle = nullptr;
    QGraphicsTextItem* m_tableHead1 = nullptr;
    QGraphicsTextItem* m_tableHead2 = nullptr;
    QGraphicsTextItem* m_tableHead3 = nullptr;
    QGraphicsTextItem* m_wplTitle = nullptr;

    bool m_valid = false;
    int m_leafCount = 0;
    long long m_wplTotal = 0;
};

#endif
