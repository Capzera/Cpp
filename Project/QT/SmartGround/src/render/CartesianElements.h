#ifndef CARTESIANELEMENTS_H
#define CARTESIANELEMENTS_H
#include <QGraphicsObject>
#include <QColor>
#include <QFont>
#include <QMap>
#include <QPainterPath>
#include <QPolygonF>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QStyleOptionGraphicsItem>
#include <functional>
#include <map>
#include "core/CartesianExpression.h"

class CartesianGridItem;
class CartesianPointItem;
class CartesianSegmentItem;
class CartesianCurveItem;

class CartesianEngine : public QObject {
    Q_OBJECT
public:
    enum ActiveTool {
        Select,
        Point,
        Line
    };

    struct Settings {
        QColor pointColor = QColor(52, 120, 214);
        QColor segmentColor = QColor(90, 100, 118);
        QColor curveColor = QColor(214, 96, 77);
        qreal pointSize = 7.0;
        int pointShape = 0;
        bool showPointName = true;
        bool showGrid = true;
        bool showAxisNumber = true;
        bool showEquation = false;
    };

    explicit CartesianEngine(QObject* parent = nullptr);
    ~CartesianEngine() override;

    void attachScene(QGraphicsScene* scene);
    void detachScene();
    QGraphicsScene* scene() const { return m_scene; }

    QPointF toScene(double x, double y) const;
    QPointF toScene(const QPointF& math) const;
    QPointF toMath(const QPointF& scene) const;
    QPointF snapScene(const QPointF& scene) const;
    QPointF snapMath(double x, double y) const;

    qreal snapTolerance() const { return m_snapTolerance; }
    void setSnapTolerance(qreal value);
    bool snapEnabled() const { return m_snapEnabled; }
    void setSnapEnabled(bool on);
    QPointF snappedMath(const QPointF& math) const;
    bool isSnapped(const QPointF& math) const;

    qreal scale() const { return m_scale; }
    void setScale(qreal value);
    qreal step() const { return m_step; }
    void setStep(qreal value);
    QPointF originScene() const { return QPointF(m_originX, m_originY); }
    void setOriginScene(const QPointF& origin);

    bool zoomBy(qreal factor, const QPointF& anchorScene);
    void syncStepToScale();
    static QVector<qreal> stepLadder();

    const Settings& settings() const { return m_settings; }
    void setSettings(const Settings& settings);
    void updateSettings();

    CartesianGridItem* grid() const { return m_grid; }

    int addPoint(double x, double y, const QString& name);
    int pointCount() const { return m_points.size(); }
    CartesianPointItem* pointAt(int index) const;
    CartesianPointItem* pointAtScene(const QPointF& scenePos) const;
    QVector<CartesianPointItem*> points() const { return m_points; }

    void removePoint(int index);
    void clearObjects();
    void removeCurve(const QString& name);
    int linkPoints(CartesianPointItem* a, CartesianPointItem* b);
    int addFunction(const QString& name, const QString& expression, const QColor& color);
    void clearFunctions();

    QVector<CartesianSegmentItem*> segments() const { return m_segments; }
    QVector<CartesianCurveItem*> curves() const { return m_curves; }

    int curveCount() const { return m_curves.size(); }
    CartesianCurveItem* curveAt(int index) const;

    QString nextPointName();
    QString uniqueFunctionName(const QString& base) const;

    void refreshAll();
    void updateViewport(const QRectF& viewportSceneRect);

    bool isEnabled() const { return m_enabled; }
    void setEnabled(bool on);

    ActiveTool activeTool() const { return m_activeTool; }
    void setActiveTool(ActiveTool tool);
    bool pointNamesEnabled() const { return m_settings.showPointName; }
    CartesianPointItem* linkStart() const { return m_linkStart; }
    void setLinkStart(CartesianPointItem* point);
    CartesianPointItem* draggingPoint() const { return m_draggingPoint; }
    void setDraggingPoint(CartesianPointItem* point);

signals:
    void objectsChanged();
    void pointMoved(int index, double x, double y);
    void pointDoubleClicked(int index);
    void segmentDoubleClicked(int index);
    void curveDoubleClicked(int index);
    void functionChanged();
    void statusMessage(const QString& text);

private:
    friend class CartesianPointItem;
    friend class CartesianSegmentItem;
    friend class CartesianCurveItem;

    void rebuildSegments();

    QGraphicsScene* m_scene = nullptr;
    CartesianGridItem* m_grid = nullptr;
    QVector<CartesianPointItem*> m_points;
    QVector<CartesianSegmentItem*> m_segments;
    QVector<CartesianCurveItem*> m_curves;
    Settings m_settings;
    qreal m_scale = 60.0;
    qreal m_step = 1.0;
    qreal m_originX = 0.0;
    qreal m_originY = 0.0;
    qreal m_snapTolerance = 0.28;
    bool m_snapEnabled = true;
    bool m_enabled = false;
    int m_nextPointSerial = 0;
    ActiveTool m_activeTool = Select;
    CartesianPointItem* m_linkStart = nullptr;
    CartesianPointItem* m_draggingPoint = nullptr;
};

class CartesianGridItem : public QGraphicsObject {
    Q_OBJECT
public:
    explicit CartesianGridItem(CartesianEngine* engine, QGraphicsItem* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;
    QRectF boundingRect() const override;

    void setEngine(CartesianEngine* engine) { m_engine = engine; }
    void setViewport(const QRectF& rect);

private:
    CartesianEngine* m_engine = nullptr;
    QRectF m_viewport = QRectF(-2000000, -2000000, 4000000, 4000000);
};

class CartesianPointItem : public QGraphicsObject {
    Q_OBJECT
public:
    explicit CartesianPointItem(CartesianEngine* engine, int index, QGraphicsItem* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    int index() const { return m_index; }
    void setIndex(int index) { m_index = index; }

    double x() const { return m_x; }
    double y() const { return m_y; }
    void setMathPos(double x, double y);

    QString name() const { return m_name; }
    void setName(const QString& name);

    bool nameHidden() const { return m_nameHidden; }
    void setNameHidden(bool hidden);

    QColor color() const { return m_color; }
    void setColor(const QColor& color);

    qreal size() const { return m_size; }
    void setSize(qreal size);

    int pointShape() const { return m_shape; }
    void setShape(int shape);

    bool highlighted() const { return m_highlighted; }
    void setHighlighted(bool on);

    void paintLabel(QPainter* painter, const QPointF& anchor, const QString& text, const QColor& color);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

private:
    CartesianEngine* m_engine = nullptr;
    int m_index = 0;
    double m_x = 0.0;
    double m_y = 0.0;
    QString m_name;
    bool m_nameHidden = false;
    QColor m_color = QColor(52, 120, 214);
    qreal m_size = 7.0;
    int m_shape = 0;
    bool m_highlighted = false;
};

class CartesianSegmentItem : public QGraphicsObject {
    Q_OBJECT
public:
    explicit CartesianSegmentItem(CartesianEngine* engine, int index,
                                 CartesianPointItem* a, CartesianPointItem* b,
                                 QGraphicsItem* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;
    QRectF boundingRect() const override;

    int index() const { return m_index; }
    void setIndex(int index) { m_index = index; }
    CartesianPointItem* start() const { return m_a; }
    CartesianPointItem* end() const { return m_b; }
    void refreshGeometry();
    bool hitTest(const QPointF& scenePos, qreal tolerance = 8.0) const;

    bool equationVisible() const { return m_equationVisible; }
    void setEquationVisible(bool on);

    QString equationText() const;
    QString segmentSummary() const;

    QColor color() const { return m_color; }
    void setColor(const QColor& color);

private:
    CartesianEngine* m_engine = nullptr;
    int m_index = 0;
    CartesianPointItem* m_a = nullptr;
    CartesianPointItem* m_b = nullptr;
    bool m_equationVisible = false;
    QColor m_color = QColor(90, 100, 118);
    QRectF m_localRect = QRectF(0, 0, 400, 400);
};

class CartesianCurveItem : public QGraphicsObject {
    Q_OBJECT
public:
    explicit CartesianCurveItem(CartesianEngine* engine, int index, QGraphicsItem* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;
    QRectF boundingRect() const override;

    int index() const { return m_index; }
    void setIndex(int index) { m_index = index; }
    QString name() const { return m_name; }
    void setName(const QString& name);

    QString expression() const { return m_expression; }
    bool setExpression(const QString& expression);

    QColor color() const { return m_color; }
    void setColor(const QColor& color);

    bool equationVisible() const { return m_equationVisible; }
    void setEquationVisible(bool on);

    void setVisibleRange(double minX, double maxX);
    void setVisibleRange(double minX, double maxX, double minY, double maxY);
    bool hitTest(const QPointF& scenePos, qreal tolerance = 6.0) const;

    int visibleRunCount() const { return m_runs.size(); }
    int visiblePointCount() const {
        int total = 0;
        for (const QPolygonF& run : m_runs) total += run.size();
        return total;
    }
    QRectF visibleBounds() const;
    QVector<QPointF> samplePoints(int maxCount) const;

private:
    void rebuildPolyline();

    CartesianEngine* m_engine = nullptr;
    int m_index = 0;
    QString m_name;
    QString m_expression;
    CartesianExpression::Result m_result;
    QColor m_color = QColor(214, 96, 77);
    bool m_equationVisible = false;
    double m_minX = -10.0;
    double m_maxX = 10.0;
    double m_minY = -10.0;
    double m_maxY = 10.0;
    QVector<QPolygonF> m_runs;
};

#endif
