#ifndef GRAPHELEMENTS_H
#define GRAPHELEMENTS_H
#include <QGraphicsObject>
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QPainter>
#include <QPainterPath>
#include <QFontMetrics>
#include <QPointer>
#include <vector>
#include <memory>

class GraphEdge;
class PointerBlock;

struct EditorGuard {
    std::shared_ptr<bool> tearingDown = std::make_shared<bool>(false);
};
class GraphNode : public QGraphicsObject {
    Q_OBJECT
    Q_PROPERTY(QColor highlightColor READ getHighlightColor WRITE setHighlightColor)
    Q_PROPERTY(qreal scale READ scale WRITE setScale)
public:
    enum { Type = UserType + 1 };
    static constexpr double kRadius = 20.0;
    int type() const override { return Type; }
    int id, d, l, value;
    bool in_stack;
    QColor color, highlightColor;
    QString subLabel;
    QString customValueText;
    QString centerValueText;
    QString valueText;
    QString tagText;
    QColor tagColor;
    std::vector<QPointer<GraphEdge>> edgeList;
    std::vector<QPointer<PointerBlock>> pointerList;
    GraphNode(int _id);
    GraphNode(int _id, int _val);
    ~GraphNode() override;
    void addEdge(GraphEdge *edge);
    void removeEdge(GraphEdge *edge);
    void removeAllEdges();
    void addPointer(PointerBlock *pointer);
    void removePointer(PointerBlock *pointer);
    void setSubLabel(const QString& text) { if (subLabel == text) return; prepareGeometryChange(); subLabel = text; update(); }
    QString subLabelText() const { return subLabel; }
    void setCustomValueText(const QString& text) { if (customValueText == text) return; prepareGeometryChange(); customValueText = text; update(); }
    void setCenterText(const QString& text) { if (centerValueText == text) return; prepareGeometryChange(); centerValueText = text; update(); }
    QString centerText() const { return centerValueText; }
    void setValueText(const QString& text) { if (valueText == text) return; prepareGeometryChange(); valueText = text; update(); }
    QString belowText() const { return valueText; }
    void setTag(const QString& text, const QColor& c) { if (tagText == text) return; prepareGeometryChange(); tagText = text; tagColor = c; update(); }
    qreal scale() const { return m_scale; }
    void setScale(qreal s) { if (qFuzzyCompare(m_scale, s)) return; prepareGeometryChange(); m_scale = s; update(); }
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    void updateData(int _d, int _l);
    QColor getHighlightColor() const { return highlightColor; }
    void setHighlightColor(const QColor& c) { highlightColor = c; update(); }
    void setEditable(bool on);
    bool isEditable() const { return m_editable; }
    bool isEditing() const { return !m_editor.isNull(); }
    QGraphicsTextItem* editorItem() const { return m_editor.data(); }
    void beginEdit();
    void commitEdit();
    void cancelEdit();
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
signals:
    void valueEdited(int id, const QString& text);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
private:
    bool m_dragging = false;
    QPointF m_dragOrigin;
    QPointF m_pressScenePos;
    bool m_editable = false;
    qreal m_scale = 1.0;
    QPointer<QGraphicsTextItem> m_editor;
    std::shared_ptr<EditorGuard> m_guard = std::make_shared<EditorGuard>();
};
class GraphEdge : public QGraphicsObject {
    Q_OBJECT
    Q_PROPERTY(qreal drawProgress READ drawProgress WRITE setDrawProgress)
    Q_PROPERTY(qreal beamProgress READ beamProgress WRITE setBeamProgress)
    Q_PROPERTY(bool beamReversed READ beamReversed WRITE setBeamReversed)
public:
    enum { Type = UserType + 2 };
    int type() const override { return Type; }
    QPointer<GraphNode> source, dest;
    bool is_directed;    bool is_bidirectional;
    int weight;
    bool has_weight;
    QColor color;
    QString pathLabel;
    QColor pathLabelColor = QColor(70, 80, 95);
    GraphEdge(GraphNode *s, GraphNode *d, bool dir, bool bi = false);
    ~GraphEdge() override;
    void detach();
    void setWeight(int w);
    void removeWeight();
    void setColor(const QColor& c) { color = c; update(); }
    void setPathLabel(const QString& text) { if (pathLabel == text) return; prepareGeometryChange(); pathLabel = text; update(); }
    void setPathLabelColor(const QColor& c) { pathLabelColor = c; update(); }
    void setPenWidth(qreal w) { if (m_penWidth == w) return; m_penWidth = w; update(); }
    void setBendOffset(const QPointF& off) { if (m_bendOffset == off) return; prepareGeometryChange(); m_bendOffset = off; update(); }
    void repointDest(GraphNode* newDest);
    qreal penWidth() const { return m_penWidth; }
    qreal drawProgress() const { return m_drawProgress; }
    void setDrawProgress(qreal p) {
        if (qFuzzyCompare(m_drawProgress, p)) return;
        prepareGeometryChange();
        m_drawProgress = p;
        update();
    }
    QPointF scenePointAt(qreal t) const;
    QPointF sceneTangentAt(qreal t) const;
    qreal beamProgress() const { return m_beamProgress; }
    void setBeamProgress(qreal p);
    void clearBeam() { setBeamProgress(-1.0); }
    bool beamReversed() const { return m_beamReversed; }
    void setBeamReversed(bool on) { if (m_beamReversed == on) return; prepareGeometryChange(); m_beamReversed = on; update(); }
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void adjust();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    QPointF weightAnchor() const;
    QPointF pathLabelAnchor() const;
    void setEditable(bool on);
    bool isEditable() const { return m_editable; }
    bool isEditing() const { return !m_editor.isNull(); }
    void beginEdit();
    void commitEdit();
    void cancelEdit();
    int key() const;
    static QPointF quadPointPublic(const QPointF& p0, const QPointF& c, const QPointF& p1, qreal t) {
        return quadPoint(p0, c, p1, t);
    }
    static QPointF quadSubPathPointPublic(const QPointF& p0, const QPointF& p1, const QPointF& p2,
                                          qreal from, qreal to) {
        QPointF a0, a1, a2, b0, b1, b2;
        splitQuad(p0, p1, p2, from, a0, a1, a2, b0, b1, b2);
        const qreal span = 1.0 - from;
        if (span < 1e-6) return b0;
        const qreal v = qBound<qreal>(0.0, (to - from) / span, 1.0);
        QPointF c0, c1, c2, d0, d1, d2;
        splitQuad(b0, b1, b2, v, c0, c1, c2, d0, d1, d2);
        const qreal mt = 1.0 - v;
        return b0 * (mt * mt) + b1 * (2 * mt * v) + b2 * (v * v);
    }
    QPointF trimmedStartPublic(qreal radius) const { return trimmedStart(radius); }
    QPointF trimmedEndPublic(qreal radius) const { return trimmedEnd(radius); }
    QPointF controlPointPublic() const { return controlPoint(); }
signals:
    void weightEdited(int weight);
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
private:
    QPointF controlPoint() const;
    QPointF curvePoint(qreal t) const;
    static QPointF quadPoint(const QPointF& p0, const QPointF& c, const QPointF& p1, qreal t);
    static void splitQuad(const QPointF& p0, const QPointF& p1, const QPointF& p2, qreal s,
                          QPointF& outL0, QPointF& outL1, QPointF& outL2,
                          QPointF& outR0, QPointF& outR1, QPointF& outR2);
    static void quadSubPath(const QPointF& p0, const QPointF& p1, const QPointF& p2,
                            qreal from, qreal to, QPainterPath& out);
    QPointF curveTangent(qreal t) const;
    QPointF trimmedStart(qreal radius) const;
    QPointF trimmedEnd(qreal radius) const;
    QPointF m_bendOffset;
    QPointF m_lastScenePos;
    bool m_dragging;
    bool m_hovered;
    bool m_editable = false;
    qreal m_penWidth = 2.0;
    qreal m_drawProgress = 1.0;
    qreal m_beamProgress = -1.0;
    bool m_beamReversed = false;
    QPointer<QGraphicsTextItem> m_editor;
    std::shared_ptr<EditorGuard> m_guard = std::make_shared<EditorGuard>();
};
class StackBlock : public QGraphicsObject {
    Q_OBJECT
public:
    enum { Type = UserType + 3 };
    int type() const override { return Type; }
    int id;
    StackBlock(int _id);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
};
class ArrayBlock : public QGraphicsObject {
    Q_OBJECT
public:
    enum { Type = UserType + 4 };
    int type() const override { return Type; }
    int id;
    int value;
    bool showValue;
    QColor color;
    QString customLabel;
    bool showLabel;
    QString customValue;
    ArrayBlock(int _id, int _val, QString _label = "");
    ~ArrayBlock() override;
    void setShowLabel(bool on) { showLabel = on; update(); }
    void setCustomLabel(const QString& text) { customLabel = text; update(); }
    void setStruck(bool on) { if (m_struck == on) return; prepareGeometryChange(); m_struck = on; update(); }
    bool isStruck() const { return m_struck; }
    QString labelText() const { return customLabel; }
    void setCustomValue(const QString& text) { prepareGeometryChange(); customValue = text; showValue = true; update(); }
    void setCellSize(double w, double h) { prepareGeometryChange(); m_cellW = w; m_cellH = h; update(); }
    void setValueFontSize(int size) { if (m_valueFontSize == size) return; prepareGeometryChange(); m_valueFontSize = size; update(); }
    int valueFontSize() const { return m_valueFontSize; }
    double cellWidth() const { return m_cellW; }
    double cellHeight() const { return m_cellH; }
    QPointF centerPos() const { return pos() + QPointF(m_cellW / 2.0, m_cellH / 2.0); }
    void setEditable(bool on);
    bool isEditable() const { return m_editable; }
    bool isEditing() const { return !m_editor.isNull(); }
    void beginEdit();
    void commitEdit();
    void cancelEdit();
    QString displayText() const;
    QRectF bodyRect() const { return QRectF(0.0, 0.0, m_cellW, m_cellH); }
    QRectF indexRect() const { return QRectF(0.0, -kIndexGap - kIndexHeight, m_cellW, kIndexHeight); }
    static constexpr double kIndexGap = 6.0;
    static constexpr double kIndexHeight = 16.0;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
signals:
    void valueEdited(int id, const QString& text);
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
private:
    bool m_editable = false;
    bool m_struck = false;
    int m_valueFontSize = 14;
    double m_cellW = 60.0;
    double m_cellH = 40.0;
    QPointer<QGraphicsTextItem> m_editor;
    std::shared_ptr<EditorGuard> m_guard = std::make_shared<EditorGuard>();
};
class FloatingText : public QGraphicsObject {
    Q_OBJECT
public:
    enum { Type = UserType + 5 };
    int type() const override { return Type; }
    QString text;
    int fontSize = 14;
    bool centered = false;
    FloatingText(const QString& t);
    void setCentered(bool on) { if (centered == on) return; prepareGeometryChange(); centered = on; update(); }
    void setFontSize(int size) { if (fontSize == size) return; prepareGeometryChange(); fontSize = size; update(); }
    void setTextColor(const QColor& c) { if (textColor == c) return; prepareGeometryChange(); textColor = c; update(); }
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
private:
    QColor textColor = QColor(20, 30, 45);
};
class PointerBlock : public QGraphicsObject {
    Q_OBJECT
public:
    enum { Type = UserType + 6 };
    enum Direction { Up, Down, Left, Right };
    int type() const override { return Type; }
    QString label;
    QColor color;
    PointerBlock(const QString& _label, QColor _color);
    void setLabel(const QString& text) { if (label == text) return; prepareGeometryChange(); label = text; update(); }
    void setAnchorNode(GraphNode* node, bool syncNow = true);
    void setAnchorItem(QGraphicsItem* item, bool syncNow = true);
    void clearAnchor() { m_anchor = nullptr; m_anchorObj = nullptr; m_anchorObjSet = false; m_anchorItem = nullptr; m_tipTarget = nullptr; m_tipTargetObj = nullptr; m_tipTargetObjSet = false; }
    GraphNode* anchorNode() const { return m_anchor.data(); }
    QGraphicsItem* anchorItem() const {
        if (m_anchorObjSet && m_anchorObj.isNull()) return nullptr;
        if (m_anchorItem) return m_anchorItem;
        return static_cast<QGraphicsItem*>(m_anchor.data());
    }
    void syncToAnchor();
    QPointF anchoredPos() const;
    void setTipTarget(QGraphicsItem* item, QPointF sceneTip);
    QGraphicsItem* tipTarget() const {
        if (m_tipTargetObjSet && m_tipTargetObj.isNull()) return nullptr;
        return m_tipTarget;
    }
    QPointF tipScenePos() const {
        if (m_tipTargetObjSet && m_tipTargetObj.isNull()) return m_tipLocal;
        if (m_tipTarget) return m_tipTarget->mapToScene(m_tipLocal);
        return m_tipLocal;
    }
    bool isPlaced() const { return m_placed; }
    void setAnchorOffset(QPointF offset) { m_anchorOffset = offset; update(); }
    void refreshLabelOffset();
    QPointF anchorOffset() const { return m_anchorOffset; }
    void setDirection(Direction dir) { if (m_direction == dir) { if (!m_labelCustom) m_labelOffset = defaultLabelOffset(); return; } prepareGeometryChange(); m_direction = dir; if (!m_labelCustom) m_labelOffset = defaultLabelOffset(); update(); }
    Direction direction() const { return m_direction; }
    void setLabelAbove(bool on) { if (m_labelAbove == on) return; prepareGeometryChange(); m_labelAbove = on; if (!m_labelCustom) m_labelOffset = defaultLabelOffset(); update(); }
    bool labelAbove() const { return m_labelAbove; }
    void setShaftLength(double len) { if (qFuzzyCompare(m_shaftLen, len)) return; prepareGeometryChange(); m_shaftLen = len; update(); }
    double shaftLength() const { return m_shaftLen; }
    void setLabelOffset(QPointF off) { if (m_labelOffset == off) return; prepareGeometryChange(); m_labelOffset = off; m_labelCustom = true; update(); }
    QPointF labelOffset() const { return m_labelOffset; }
    void setLabelToRight(double gap = 6.0) {
        prepareGeometryChange();
        const double s = (m_direction == Right) ? -1.0 : 1.0;
        m_labelOffset = QPointF(s * (10.0 + gap + labelWidth() / 2.0), -10.0);
        m_labelCustom = true;
        update();
    }
    void setLabelAtTail(double gap = 8.0) {
        prepareGeometryChange();
        m_labelOffset = tailLabelOffset(gap);
        m_labelCustom = true;
        update();
    }
    double labelWidth() const;
    double labelBaseX() const { return m_labelOffset.x(); }
    double labelBaseY() const { return m_labelOffset.y(); }
    int labelRow() const { return m_labelRow; }
    void setLabelRow(int row);
    double labelRowOffsetFor(int row) const {
        const double sign = (m_direction == Down) ? -1.0 : 1.0;
        return sign * row * kLabelRowGap;
    }
    double labelRowOffset() const { return labelRowOffsetFor(m_labelRow); }
    QPointF labelShift() const { return m_labelShift; }
    void setLabelShift(QPointF s) { if (m_labelShift == s) return; prepareGeometryChange(); m_labelShift = s; update(); }
    void resetLabelShift() { if (m_labelShift.isNull()) return; prepareGeometryChange(); m_labelShift = QPointF(0, 0); update(); }
    double effectiveLabelX() const { return m_labelOffset.x() + m_labelShift.x(); }
    double effectiveLabelY() const { return m_labelOffset.y() + labelRowOffset() + m_labelShift.y(); }
    QRectF labelRect() const {
        const double w = labelWidth();
        return QRectF(effectiveLabelX() - w / 2.0, effectiveLabelY(), w, 20.0);
    }
    double labelLeft() const { return effectiveLabelX() - labelWidth() / 2.0; }
    double labelRight() const { return effectiveLabelX() + labelWidth() / 2.0; }
    double labelTop() const { return effectiveLabelY(); }
    double labelBottom() const { return effectiveLabelY() + 20.0; }
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
private:
    QPointer<GraphNode> m_anchor;
    QPointer<QObject> m_anchorObj;
    bool m_anchorObjSet = false;
    QGraphicsItem* m_anchorItem = nullptr;
    QGraphicsItem* m_tipTarget = nullptr;
    QPointer<QObject> m_tipTargetObj;
    bool m_tipTargetObjSet = false;
    QPointF m_tipLocal;
    QPointF m_anchorOffset = QPointF(0, 34);
    Direction m_direction = Up;
    bool m_labelAbove = false;
    bool m_labelCustom = false;
    bool m_placed = false;
    double m_shaftLen = 26.0;
    QPointF m_labelOffset = QPointF(0, 0);
    QPointF m_labelShift = QPointF(0, 0);
    int m_labelRow = 0;

    QPointF defaultLabelOffset() const;
    QPointF tailLabelOffset(double gap) const;
    static constexpr double kLabelRowGap = 22.0;
};

class LinkLine : public QGraphicsObject {
    Q_OBJECT
public:
    enum { Type = UserType + 8 };
    int type() const override { return Type; }
    QPointF start;
    QPointF end;
    QColor color = QColor(120, 130, 150);
    qreal width = 2.0;
    qreal bend = 0.0;
    bool dashed = false;
    bool arrow = false;
    LinkLine(QPointF s, QPointF e);
    void setPoints(QPointF s, QPointF e);
    void setStyle(const QColor& c, qreal w, bool dash, bool hasArrow);
    void setBend(qreal b) { if (bend == b) return; prepareGeometryChange(); bend = b; update(); }
    QPointF control() const;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
};

class Ripple : public QGraphicsObject {
    Q_OBJECT
public:
    enum { Type = UserType + 9 };
    int type() const override { return Type; }
    QColor color = QColor(120, 170, 230);
    Ripple(QPointF center, QColor c);
    void setRadius(qreal r);
    qreal radius() const { return m_radius; }
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
private:
    QPointF m_center;
    qreal m_radius = 8.0;
};
class RectRipple : public QGraphicsObject {
    Q_OBJECT
public:
    enum { Type = UserType + 12 };
    int type() const override { return Type; }
    QColor color = QColor(120, 170, 230);
    RectRipple(QPointF center, QSizeF size, QColor c);
    void setGrow(qreal g);
    qreal grow() const { return m_grow; }
    void setBaseSize(QSizeF s);
    QSizeF baseSize() const { return m_base; }
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;
private:
    QPointF m_center;
    QSizeF m_base = QSizeF(60.0, 40.0);
    qreal m_grow = 0.0;
};

class TracerDot : public QGraphicsObject {
    Q_OBJECT
public:
    enum { Type = UserType + 10 };
    int type() const override { return Type; }
    explicit TracerDot(QColor c);
    QColor color = QColor(120, 170, 230);
    void setColor(const QColor& c) { color = c; update(); }
    qreal radius() const { return m_radius; }
    void setRadius(qreal r) { if (qFuzzyCompare(m_radius, r)) return; prepareGeometryChange(); m_radius = r; update(); }
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
private:
    qreal m_radius = 6.0;
};

class PathTrail : public QGraphicsObject {
    Q_OBJECT
public:
    enum { Type = UserType + 11 };
    int type() const override { return Type; }
    explicit PathTrail(QColor c);
    QColor color = QColor(120, 170, 230);
    void setColor(const QColor& c) { color = c; update(); }
    void setPoints(const QVector<QPointF>& pts);
    void appendPoint(const QPointF& p);
    void clearPoints();
    int pointCount() const { return m_points.size(); }
    qreal progress() const { return m_progress; }
    void setProgress(qreal t);
    qreal trailWidth() const { return m_width; }
    void setTrailWidth(qreal w) { if (qFuzzyCompare(m_width, w)) return; prepareGeometryChange(); m_width = w; update(); }
    void setHeadRadius(qreal r) { if (qFuzzyCompare(m_headRadius, r)) return; prepareGeometryChange(); m_headRadius = r; update(); }
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
private:
    QVector<QPointF> m_points;
    QColor m_color;
    qreal m_progress = 0.0;
    qreal m_width = 5.0;
    qreal m_headRadius = 7.0;
};
#endif


