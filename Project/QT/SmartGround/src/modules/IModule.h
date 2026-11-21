#ifndef IMODULE_H
#define IMODULE_H
#include <QString>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QPointer>
#include <QEasingCurve>
#include <QVariantAnimation>
#include <map>
#include <vector>
#include <functional>
#include "engine/AnimFrame.h"
#include "render/GraphElements.h"
#include "modules/GraphModuleUtils.h"
#include "modules/ModuleFields.h"

class QPropertyAnimation;

class IModule {
public:
    bool m_animated = true;
    virtual ~IModule();

    virtual QString getModuleName() const = 0;
    virtual QString getDefaultInput() const = 0;
    virtual void buildLayout(QGraphicsScene* scene, const QString& inputData) = 0;
    virtual std::vector<AnimFrame> runAlgorithm() = 0;
    virtual void renderFrame(const AnimFrame& frame) = 0;
    virtual void reset() = 0;

    virtual void releaseSceneItems() {}

    virtual bool supportsGraphOptions() const { return false; }
    virtual bool supportsDirected() const { return true; }
    virtual bool supportsBidirectional() const { return true; }
    virtual bool isCartesian() const { return false; }
    virtual GraphModuleUtils::GraphOptions getGraphOptions() const { return GraphModuleUtils::GraphOptions(); }
    virtual void setGraphOptions(const GraphModuleUtils::GraphOptions& options) { Q_UNUSED(options) }

    virtual QString moduleSettingsTitle() const { return QString(); }
    virtual QVector<ModuleField> moduleSettingsFields() const { return QVector<ModuleField>(); }
    virtual void applyModuleSetting(const QString& key, const ModuleField& field) { Q_UNUSED(key); Q_UNUSED(field); }
    virtual void moduleSettingsChanged() {}

    static void setScriptTextVisible(bool on);
    static bool scriptTextVisible();
    static int scriptTextMarker();
    static QGraphicsTextItem* createScriptText(QGraphicsScene* scene, const QString& text);
    static QGraphicsTextItem* createStatusText(QGraphicsScene* scene, const QString& text);
    void setScriptText(QGraphicsTextItem* item, const QString& text);
    void setScriptText(QGraphicsTextItem* item, const QString& text, const QColor& color);
    void clearScriptText(QGraphicsTextItem* item);
    void setStatusText(QGraphicsTextItem* item, const QString& text);

    static int frameDelayMs(const AnimFrame& frame);
    static int frameDelayMsForText(const QString& text);

    virtual QString getInputHint() const { return QString("请输入数据"); }

    virtual std::map<int, QPointF> captureNodePositions() const { return {}; }
    virtual void restoreNodePositions(const std::map<int, QPointF>& positions) { Q_UNUSED(positions) }

    virtual QString getCategoryName() const { return QString("其他"); }
    virtual QString getComplexityNote() const { return QString(); }
    virtual QString getTeachingTip() const { return QString(); }

    virtual int getStepCost() const { return 1; }

    virtual bool validateInput(const QString& inputData, QString& errorMessage) const {
        Q_UNUSED(inputData);
        Q_UNUSED(errorMessage);
        return true;
    }

    virtual void onCanvasDoubleClick(const QPointF& scenePos) { Q_UNUSED(scenePos); }
    virtual void onCanvasBackgroundClick() {}

    virtual void applyTheme() {}

    virtual bool supportsLiveEditing() const { return false; }
    virtual QString editableRowFor(ArrayBlock* block) const;
    virtual QString editableRowFor(GraphNode* node) const;
    virtual QString editableRowFor(GraphEdge* edge) const;
    virtual void onLiveDataChanged(const QString& row, int index, int value);
    virtual bool onSlotEdited(const QString& key, const QString& text);
    void applyLiveEdit(const QString& key, const QString& text);
    void setLiveEditCallback(std::function<void()> cb) { m_liveEditCallback = cb; }
    void enableLiveEditing();
    void refreshLiveSlots();
    void clearLiveSlots();
    QString makeLiveKey(const QString& row, int index) const;
    QString liveScope() const;
    int liveValue(const QString& row, int index, int computed) const;
    bool hasLiveEdits() const { return m_liveEditCount > 0; }
    int liveEditCount() const { return m_liveEditCount; }
    void clearLiveEditCount() { m_liveEditCount = 0; }

    void registerArraySlot(ArrayBlock* block, const QString& row, int dataIndex);
    void registerNodeSlot(int nodeId, const QString& row, int dataIndex);
    void registerEdgeSlot(int edgeIndex, const QString& row, int dataIndex);
    int liveIndexOfArray(const ArrayBlock* block) const;
    int liveIndexOfNode(int nodeId) const;
    int liveIndexOfEdge(int edgeIndex) const;

    void beginFrame() {
        purgeTransientItems();
        stopAllAnims();
        completeVisibleEdges();
    }
    void finalizeFrame() { resolveVisualCollisions(); resolveScriptTextOverlaps(); syncPointerTips(); }

protected:
    QGraphicsScene* m_scene = nullptr;

    void releaseAnim(QObject* target, const char* property);
    void stopAnimFor(QObject* target);
    void stopAllAnims();
    void purgeDeadAnims();

    void playSwapAnim(ArrayBlock* b1, ArrayBlock* b2, int val1, int val2);
    void playNodeSwapAnim(GraphNode* n1, GraphNode* n2, int val1, int val2, int duration = 500);
    void playDualSwapAnim(ArrayBlock* b1, ArrayBlock* b2, GraphNode* n1, GraphNode* n2,
                          int val1, int val2, int duration = 500);
    void playShiftAnim(ArrayBlock* src, ArrayBlock* dst, int val, QColor targetColor = Qt::white, bool pulseDst = false);
    void playExtractAnim(ArrayBlock* src, ArrayBlock* dst, int val);
    void playPointerMove(PointerBlock* ptr, QPointF targetPos);
    void playPointerMove(PointerBlock* ptr, QPointF targetPos, int duration, const QEasingCurve& curve);
    void aimPointerOutside(PointerBlock* ptr, ArrayBlock* cell, bool above);
    void aimPointerOutsideNode(PointerBlock* ptr, GraphNode* node, bool above);
    void retargetPointer(PointerBlock* ptr, QGraphicsItem* anchor);
    void retargetPointerToNode(PointerBlock* ptr, GraphNode* anchor);
    void retargetPointerTo(PointerBlock* ptr, QGraphicsItem* anchor, QPointF offset);
    void retargetPointerAcrossCells(PointerBlock* ptr, QGraphicsItem* anchor, QPointF offset);
    void retargetPointerWithDirection(PointerBlock* ptr, QGraphicsItem* anchor, QPointF offset,
                                      PointerBlock::Direction dir);
    void playScaleAnim(ArrayBlock* block);
    void playPointersSync(std::vector<PointerBlock*> ptrs, int startX, int yPos = 350, int spacing = 60);
    void playPointerArrange(std::vector<PointerBlock*> ptrs, std::vector<QGraphicsItem*> anchors, double yOffset = 42.0);
    void resolveVisualCollisions();
    void resolveScriptTextOverlaps();
    void syncPointerTips();
    void separatePointerLabels(const std::vector<PointerBlock*>& ptrs);
    void layoutPointerSlots(std::vector<PointerBlock*> ptrs, std::vector<QGraphicsItem*> anchors, double yOffset = 42.0);
    QPointF pointerTipFor(PointerBlock* ptr, QGraphicsItem* anchor);
    QPointF pointerTipFor(PointerBlock* ptr, QGraphicsItem* anchor, PointerBlock::Direction dir);
    void animatePointerTo(PointerBlock* p, const QPointF& target);
    void playValueFlight(QPointF startPos, ArrayBlock* target, int value, QColor targetColor = Qt::white, bool pulseTarget = false);
    void playDualValueFlight(QPointF startA, QPointF startB, ArrayBlock* target, int valueA, int valueB, QColor targetColor = Qt::white);
    void playMultiValueFlight(const std::vector<QPointF>& sources, const std::vector<int>& values,
                              ArrayBlock* target, int total, QColor targetColor = Qt::white);
    void playTagDrop(QPointF startPos, QPointF endPos, const QString& text, QColor color, int duration = 420);
    void playRowShift(std::vector<QGraphicsItem*> items, QPointF delta, int duration = 460);
    void playScalePulse(QObject* item, qreal peak = 1.28, int duration = 300);
    void playNodeAppear(QObject* item, int duration = 420);
    void playEdgeGrow(GraphEdge* edge, int duration = 200);
    void playEdgeTrace(GraphEdge* edge, QColor color, int duration = 420);
    void playEdgeTraceFrom(GraphEdge* edge, QColor color, int fromNodeId, int duration = 420);
    void playNodeRipple(QObject* item, QColor color, qreal maxRadius = 40.0, int duration = 620);
    void playNodePop(QObject* item, qreal peak = 1.22, int duration = 340);
    void playEdgeSweep(GraphEdge* edge, QColor color, int duration = 460);
    void completeEdgeGrowth(std::vector<GraphEdge*>& edges);
    void completeVisibleEdges();
    void playEdgeTravel(GraphEdge* edge, QColor color, int duration = 520);
    void playEdgeTravelFrom(GraphEdge* edge, QColor color, int fromNodeId, int duration = 520);
    void startBeam(GraphEdge* edge, QColor color, int duration = 520);
    void startBeamFrom(GraphEdge* edge, QColor color, int fromNodeId, int duration = 520);
    void playDiscover(QObject* node, QColor color, qreal maxRadius = 46.0, int duration = 640);
    void playEdgeDiscover(GraphEdge* edge, QObject* node, QColor color, int duration = 520);
    void playNodeWake(QObject* node, QColor color, qreal maxRadius = 34.0, int duration = 520);
    void playFocusRing(QObject* node, QColor color, int duration = 420);
    void playValueArcFlight(QPointF start, QPointF end, int value, QColor color = QColor(20, 30, 45), int duration = 560);
    void playCellSweep(const std::vector<ArrayBlock*>& cells, QColor color, int stagger = 60, int duration = 320);
    void playCellLanding(ArrayBlock* cell, QColor color, int duration = 420);
    void playFadeIn(QObject* item, int duration = 360);
    void playFadeItem(QObject* item, qreal from, qreal to, int duration = 360);
    void playMoveTo(std::vector<QGraphicsObject*> items, std::vector<QPointF> targets, int duration = 440);
    void playValueDropInto(QPointF start, ArrayBlock* target, int value,
                           QColor color = QColor(20, 30, 45), int duration = 520);
    void playValueFlyOut(QPointF start, int value,
                         QColor color = QColor(20, 30, 45), int duration = 460);
    void playTextFlight(const QString& text, QPointF start, QPointF end,
                        int fontSize = 14, QColor color = QColor(20, 30, 45), int duration = 520,
                        std::function<void()> onArrive = nullptr);
    void playTextFlightAligned(const QString& text, QPointF start, QPointF endLeft,
                                int fontSize = 14, QColor color = QColor(20, 30, 45), int duration = 520,
                                std::function<void()> onArrive = nullptr);
    void playRipple(QPointF center, QColor color, qreal maxRadius, int duration = 620);
    void playCellRipple(ArrayBlock* cell, QColor color, qreal expandRatio = 1.0, int duration = 620);
    void playCellRippleAt(QPointF center, QSizeF size, QColor color, qreal expand = 10.0, int duration = 620);
    void playPathTrail(PathTrail* trail, QVector<QPointF> points, QColor color, int duration = 560);
    void playPathTrailAppend(PathTrail* trail, QPointF point, QColor color, int duration = 420);
    void playLinkRedirect(LinkLine* line, QPointF start, QPointF end, int duration = 460);
    void playBubbleFlight(QPointF start, QPointF end, int value, int duration = 520);
    void drawLabel(const QString& text, QPointF pos, QColor color = Qt::black, int size = 12);
    void drawCaption(const QString& text, QPointF pos, QColor color = Qt::black, int size = 11);
    void applyNodeHighlight(GraphNode* node, QColor color);
    void clearNodeHighlights();
    void attachEdge(GraphNode* node, GraphEdge* edge);
    void detachAllEdges(std::vector<GraphNode*>& nodes, std::vector<GraphEdge*>& edges);
    GraphEdge* createEdge(GraphNode* u, GraphNode* v, const GraphModuleUtils::GraphOptions& options,
                          const GraphModuleUtils::EdgeInput& edge, bool hasWeight);
    void playTextFlightTo(QGraphicsTextItem* target, const QString& text, QPointF start,
                          int duration = 520, std::function<void()> onArrive = nullptr);
    void playTextFlightFragment(QGraphicsTextItem* target, const QString& prefix, const QString& fragment,
                                QPointF start, int duration = 520, std::function<void()> onArrive = nullptr);
    void purgeTransientItems();
    void discardTransient(QGraphicsItem* item);
    QPropertyAnimation* acquireAnim(QObject* target, const char* property, int duration = 400, const QEasingCurve& curve = QEasingCurve::InOutQuad);

private:
    struct AnimKey {
        QPointer<QObject> target;
        QString property;
        bool operator<(const AnimKey& other) const {
            if (target.data() != other.target.data()) return target.data() < other.target.data();
            return property < other.property;
        }
    };
    std::map<AnimKey, QPointer<QPropertyAnimation>> m_animRegistry;
    std::map<QGraphicsItem*, QPointer<QVariantAnimation>> m_rowShiftAnims;
    std::map<GraphEdge*, QPointer<QVariantAnimation>> m_beamAnims;
    std::vector<QPointer<FloatingText>> m_transientItems;
    std::vector<QPointer<TracerDot>> m_transientDots;
    std::vector<QPointer<Ripple>> m_transientRipples;
    std::vector<QPointer<RectRipple>> m_transientRectRipples;
    std::vector<std::function<void()>> m_pendingCommits;
    std::map<QString, int> m_liveSlots;
    std::map<const ArrayBlock*, QString> m_arraySlotRow;
    std::map<const ArrayBlock*, int> m_arraySlotIndex;
    std::map<int, QString> m_nodeSlotRow;
    std::map<int, int> m_nodeSlotIndex;
    std::map<int, QString> m_edgeSlotRow;
    std::map<int, int> m_edgeSlotIndex;
    int m_liveEditCount = 0;
    std::function<void()> m_liveEditCallback;
    static bool s_scriptTextVisible;
    static const int kScriptTextMarker;
};
#endif
