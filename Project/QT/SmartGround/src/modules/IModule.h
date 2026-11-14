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
    virtual GraphModuleUtils::GraphOptions getGraphOptions() const { return GraphModuleUtils::GraphOptions(); }
    virtual void setGraphOptions(const GraphModuleUtils::GraphOptions& options) { Q_UNUSED(options) }

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
    }

protected:
    QGraphicsScene* m_scene = nullptr;

    void releaseAnim(QObject* target, const char* property);
    void stopAnimFor(QObject* target);
    void stopAllAnims();
    void purgeDeadAnims();

    void playSwapAnim(ArrayBlock* b1, ArrayBlock* b2, int val1, int val2);
    void playShiftAnim(ArrayBlock* src, ArrayBlock* dst, int val, QColor targetColor = Qt::white, bool pulseDst = false);
    void playExtractAnim(ArrayBlock* src, ArrayBlock* dst, int val);
    void playPointerMove(PointerBlock* ptr, QPointF targetPos);
    void playPointerMove(PointerBlock* ptr, QPointF targetPos, int duration, const QEasingCurve& curve);
    void playScaleAnim(ArrayBlock* block);
    void playPointersSync(std::vector<PointerBlock*> ptrs, int startX, int yPos = 350, int spacing = 60);
    void playPointerArrange(std::vector<PointerBlock*> ptrs, std::vector<QGraphicsItem*> anchors, double yOffset = 42.0);
    void layoutPointerSlots(std::vector<PointerBlock*> ptrs, std::vector<QGraphicsItem*> anchors, double yOffset = 42.0);
    void playValueFlight(QPointF startPos, ArrayBlock* target, int value, QColor targetColor = Qt::white, bool pulseTarget = false);
    void playDualValueFlight(QPointF startA, QPointF startB, ArrayBlock* target, int valueA, int valueB, QColor targetColor = Qt::white);
    void playMultiValueFlight(const std::vector<QPointF>& sources, const std::vector<int>& values,
                              ArrayBlock* target, int total, QColor targetColor = Qt::white);
    void playTagDrop(QPointF startPos, QPointF endPos, const QString& text, QColor color, int duration = 420);
    void playRowShift(std::vector<QGraphicsItem*> items, QPointF delta, int duration = 460);
    void playScalePulse(QObject* item, qreal peak = 1.28, int duration = 300);
    void purgeTransientItems();
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
    std::vector<QPointer<FloatingText>> m_transientItems;
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
};
#endif
