#ifndef TRIEMODULE_H
#define TRIEMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <QObject>
#include <QGraphicsTextItem>
#include <QPointer>
#include <map>
#include <vector>

class TrieModule : public QObject, public IModule {
    Q_OBJECT
public:
    TrieModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    QString getInputHint() const override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    std::vector<AnimFrame> runAlgorithm() override;
    void renderFrame(const AnimFrame& frame) override;
    void reset() override;
    void releaseSceneItems() override;

    QString moduleSettingsTitle() const override;
    QVector<ModuleField> moduleSettingsFields() const override;
    void applyModuleSetting(const QString& key, const ModuleField& field) override;

    std::vector<AnimFrame> runInsertMode();
    std::vector<AnimFrame> runQueryMode();
    std::vector<AnimFrame> runOrderMode();

private:
    struct TrieNode {
        int id = 0;
        char ch = 0;
        int parent = -1;
        std::vector<int> children;
        bool isEnd = false;
        GraphNode* node = nullptr;
    };

    void createNodeItem(int id);
    void createEdgeItem(int childId);
    void layoutNode(int id, double x, double y);
    void rebuildLayout();
    double slotX(int index) const;
    int findChild(int parent, char ch) const;
    void ensureEndBlock(int index);
    void buildOrderRow();
    void clearOrderRow();
    void setCharRow(const QString& word, int focusIndex);
    void buildStaticTrie();

    std::vector<AnimFrame> m_frames;
    std::vector<TrieNode> m_nodes;
    std::map<int, GraphEdge*> m_edges;
    std::map<int, ArrayBlock*> m_endBlocks;
    std::map<int, char> m_nodeChars;
    std::vector<int> m_endNodeIds;
    QGraphicsTextItem* m_mathText = nullptr;
    QGraphicsTextItem* m_wordText = nullptr;
    QGraphicsTextItem* m_stateText = nullptr;
    QGraphicsTextItem* m_orderText = nullptr;
    QGraphicsTextItem* m_charRowLabel = nullptr;
    std::vector<QGraphicsTextItem*> m_orderItems;
    ArrayBlock* m_charBlocks = nullptr;
    std::vector<ArrayBlock*> m_charCells;
    PointerBlock* m_cursorPointer = nullptr;
    PointerBlock* m_indexPointer = nullptr;
    std::vector<QString> m_words;
    bool m_valid = false;
    int m_mode = 0;
    int m_staticNodeCount = 0;
    QString m_queryWord;
};

#endif
