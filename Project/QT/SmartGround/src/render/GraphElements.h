#ifndef GRAPHELEMENTS_H
#define GRAPHELEMENTS_H
#include <QGraphicsObject>
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QPainter>
#include <QFontMetrics>
#include <QPointer>
#include <vector>
class GraphEdge;
class PointerBlock;
class GraphNode : public QGraphicsObject {
    Q_OBJECT
    Q_PROPERTY(QColor highlightColor READ getHighlightColor WRITE setHighlightColor)
public:
    enum { Type = UserType + 1 };
    int type() const override { return Type; }
    int id, d, l, value;
    bool in_stack;
    QColor color, highlightColor;
    QString subLabel;
    QString customValueText;
    QString valueText;
    QString tagText;
    QColor tagColor;
    std::vector<GraphEdge*> edgeList;
    std::vector<PointerBlock*> pointerList;
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
    void setCustomValueText(const QString& text) { prepareGeometryChange(); customValueText = text; update(); }
    void setValueText(const QString& text) { if (valueText == text) return; prepareGeometryChange(); valueText = text; update(); }
    void setTag(const QString& text, const QColor& c) { if (tagText == text) return; prepareGeometryChange(); tagText = text; tagColor = c; update(); }
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    void updateData(int _d, int _l);
    QColor getHighlightColor() const { return highlightColor; }
    void setHighlightColor(const QColor& c) { highlightColor = c; update(); }
    void setEditable(bool on);
    bool isEditable() const { return m_editable; }
    bool isEditing() const { return !m_editor.isNull(); }
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
    QPointer<QGraphicsTextItem> m_editor;
};
class GraphEdge : public QGraphicsObject {
    Q_OBJECT
public:
    enum { Type = UserType + 2 };
    int type() const override { return Type; }
    GraphNode *source, *dest;
    bool is_directed;
    bool is_bidirectional;
    int weight;
    bool has_weight;
    QColor color;
    GraphEdge(GraphNode *s, GraphNode *d, bool dir, bool bi = false);
    ~GraphEdge() override;
    void detach();
    void setWeight(int w);
    void removeWeight();
    void setColor(const QColor& c) { color = c; update(); }
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
    void setEditable(bool on);
    bool isEditable() const { return m_editable; }
    bool isEditing() const { return !m_editor.isNull(); }
    void beginEdit();
    void commitEdit();
    void cancelEdit();
    int key() const;
signals:
    void weightEdited(int weight);
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
private:
    QPointF controlPoint() const;
    QPointF curvePoint(qreal t) const;
    QPointF curveTangent(qreal t) const;
    QPointF trimmedStart(qreal radius) const;
    QPointF trimmedEnd(qreal radius) const;
    QPointF m_bendOffset;
    QPointF m_lastScenePos;
    bool m_dragging;
    bool m_hovered;
    bool m_editable = false;
    QPointer<QGraphicsTextItem> m_editor;
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
    void setShowLabel(bool on) { showLabel = on; update(); }
    void setCustomLabel(const QString& text) { customLabel = text; update(); }
    QString labelText() const { return customLabel; }
    void setCustomValue(const QString& text) { prepareGeometryChange(); customValue = text; showValue = true; update(); }
    void setCellSize(double w, double h) { prepareGeometryChange(); m_cellW = w; m_cellH = h; update(); }
    double cellWidth() const { return m_cellW; }
    double cellHeight() const { return m_cellH; }
    void setEditable(bool on);
    bool isEditable() const { return m_editable; }
    bool isEditing() const { return !m_editor.isNull(); }
    void beginEdit();
    void commitEdit();
    void cancelEdit();
    QString displayText() const;
    QRectF bodyRect() const { return QRectF(0.0, 0.0, m_cellW, m_cellH); }
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
signals:
    void valueEdited(int id, const QString& text);
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
private:
    bool m_editable = false;
    double m_cellW = 60.0;
    double m_cellH = 40.0;
    QPointer<QGraphicsTextItem> m_editor;
};
class FloatingText : public QGraphicsObject {
    Q_OBJECT
public:
    enum { Type = UserType + 5 };
    int type() const override { return Type; }
    QString text;
    FloatingText(const QString& t);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
};
class PointerBlock : public QGraphicsObject {
    Q_OBJECT
public:
    enum { Type = UserType + 6 };
    int type() const override { return Type; }
    QString label;
    QColor color;
    PointerBlock(const QString& _label, QColor _color);
    void setLabel(const QString& text) { if (label == text) return; prepareGeometryChange(); label = text; update(); }
    void setAnchorNode(GraphNode* node, bool syncNow = true);
    void clearAnchor() { m_anchor = nullptr; }
    GraphNode* anchorNode() const { return m_anchor; }
    void syncToAnchor();
    void setAnchorOffset(QPointF offset) { m_anchorOffset = offset; update(); }
    QPointF anchorOffset() const { return m_anchorOffset; }
    double labelWidth() const;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
private:
    GraphNode* m_anchor = nullptr;
    QPointF m_anchorOffset = QPointF(0, 34);
};
#endif


