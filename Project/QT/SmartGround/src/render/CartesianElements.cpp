#include "CartesianElements.h"
#include "core/CartesianExpression.h"
#include "ui/UiTheme.h"
#include "ui/UiFonts.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QPainter>
#include <QPainterPath>
#include <QMap>
#include <QLineF>
#include <cmath>

namespace {

const double kEpsilon = 1e-9;

QColor axisColor() {
    return QColor(126, 138, 156);
}

QColor gridColor() {
    return QColor(224, 230, 240);
}

QColor gridMinorColor() {
    return QColor(240, 243, 249);
}

QString formatAxisNumber(double value) {
    if (std::abs(value) < kEpsilon) return QString("0");
    QString text = QString::number(value, 'f', 6);
    if (text.contains('.')) {
        while (text.endsWith('0')) text.chop(1);
        if (text.endsWith('.')) text.chop(1);
    }
    if (text == "-0") text = "0";
    return text;
}

double pointToSegmentDistance(const QPointF& point, const QPointF& a, const QPointF& b) {
    const double dx = b.x() - a.x();
    const double dy = b.y() - a.y();
    const double lengthSquared = dx * dx + dy * dy;
    if (lengthSquared < 1e-9) {
        const double px = point.x() - a.x();
        const double py = point.y() - a.y();
        return std::sqrt(px * px + py * py);
    }
    double t = ((point.x() - a.x()) * dx + (point.y() - a.y()) * dy) / lengthSquared;
    t = qBound(0.0, t, 1.0);
    const double cx = a.x() + t * dx;
    const double cy = a.y() + t * dy;
    const double px = point.x() - cx;
    const double py = point.y() - cy;
    return std::sqrt(px * px + py * py);
}

}  // namespace

CartesianEngine::CartesianEngine(QObject* parent) : QObject(parent) {}

CartesianEngine::~CartesianEngine() {
    detachScene();
}

void CartesianEngine::attachScene(QGraphicsScene* scene) {
    if (m_scene == scene) return;
    detachScene();
    m_scene = scene;
    if (!m_scene) return;

    m_grid = new CartesianGridItem(this);
    m_grid->setZValue(-1000);
    m_scene->addItem(m_grid);
    refreshAll();
}

void CartesianEngine::detachScene() {
    clearObjects();
    if (m_grid) {
        if (m_scene && m_grid->scene() == m_scene) m_scene->removeItem(m_grid);
        delete m_grid;
        m_grid = nullptr;
    }
    m_scene = nullptr;
}

void CartesianEngine::setEnabled(bool on) {
    if (m_enabled == on) return;
    m_enabled = on;
    if (m_grid) m_grid->setVisible(on && m_settings.showGrid);
}

void CartesianEngine::setActiveTool(ActiveTool tool) {
    if (m_activeTool == tool) return;
    m_activeTool = tool;
    if (m_linkStart) {
        m_linkStart->setHighlighted(false);
        m_linkStart = nullptr;
    }
    m_draggingPoint = nullptr;
}

void CartesianEngine::setLinkStart(CartesianPointItem* point) {
    if (m_linkStart == point) return;
    if (m_linkStart) m_linkStart->setHighlighted(false);
    m_linkStart = point;
    if (m_linkStart) m_linkStart->setHighlighted(true);
}

void CartesianEngine::setDraggingPoint(CartesianPointItem* point) {
    m_draggingPoint = point;
}

void CartesianEngine::setSelectedPoints(const QVector<CartesianPointItem*>& points) {
    for (CartesianPointItem* point : m_selectedPoints) {
        if (point) point->setSelected(false);
    }
    m_selectedPoints.clear();
    for (CartesianPointItem* point : points) {
        if (!point) continue;
        if (m_selectedPoints.contains(point)) continue;
        m_selectedPoints.push_back(point);
        point->setSelected(true);
    }
    emit pointSelectionChanged(m_selectedPoints.size());
}

void CartesianEngine::toggleSelectedPoint(CartesianPointItem* point) {
    if (!point) return;
    if (m_selectedPoints.contains(point)) {
        m_selectedPoints.removeAll(point);
        point->setSelected(false);
    } else {
        m_selectedPoints.push_back(point);
        point->setSelected(true);
    }
    emit pointSelectionChanged(m_selectedPoints.size());
}

void CartesianEngine::clearPointSelection() {
    if (m_selectedPoints.isEmpty()) return;
    for (CartesianPointItem* point : m_selectedPoints) {
        if (point) point->setSelected(false);
    }
    m_selectedPoints.clear();
    emit pointSelectionChanged(0);
}

bool CartesianEngine::isPointSelected(const CartesianPointItem* point) const {
    return point && m_selectedPoints.contains(const_cast<CartesianPointItem*>(point));
}

QVector<qreal> CartesianEngine::stepLadder() {
    QVector<qreal> ladder;
    ladder << 100.0 << 50.0 << 20.0 << 10.0 << 5.0 << 2.0 << 1.0
           << 0.5 << 0.2 << 0.1 << 0.05 << 0.02 << 0.01;
    return ladder;
}

void CartesianEngine::setScale(qreal value) {
    const qreal clamped = qBound(0.02, value, 20000.0);
    if (qAbs(clamped - m_scale) < kEpsilon) return;
    m_scale = clamped;
    syncStepToScale();
    refreshAll();
}

void CartesianEngine::setStep(qreal value) {
    const qreal clamped = qBound(1e-4, value, 1e6);
    if (qAbs(clamped - m_step) < kEpsilon) return;
    m_step = clamped;
    if (m_grid) m_grid->update();
}

void CartesianEngine::syncStepToScale() {
    const QVector<qreal> ladder = stepLadder();
    qreal chosen = ladder.last();
    for (qreal candidate : ladder) {
        if (candidate * m_scale <= 62.0) {
            chosen = candidate;
            break;
        }
    }
    if (chosen != m_step) {
        m_step = chosen;
        if (m_grid) m_grid->update();
    }
}
bool CartesianEngine::zoomBy(qreal factor, const QPointF& anchorScene) {
    const qreal oldScale = m_scale;
    const qreal newScale = qBound(0.02, oldScale * factor, 20000.0);
    if (qAbs(newScale - oldScale) < kEpsilon) return false;

    const QPointF mathAnchor = toMath(anchorScene);
    m_scale = newScale;
    m_originX = anchorScene.x() - mathAnchor.x() * m_scale;
    m_originY = anchorScene.y() + mathAnchor.y() * m_scale;

    syncStepToScale();
    refreshAll();
    return true;
}

void CartesianEngine::setOriginScene(const QPointF& origin) {
    if (qAbs(origin.x() - m_originX) < kEpsilon && qAbs(origin.y() - m_originY) < kEpsilon) return;
    m_originX = origin.x();
    m_originY = origin.y();
    refreshAll();
}

QPointF CartesianEngine::toScene(double x, double y) const {
    return QPointF(m_originX + x * m_scale, m_originY - y * m_scale);
}

QPointF CartesianEngine::toScene(const QPointF& math) const {
    return toScene(math.x(), math.y());
}

QPointF CartesianEngine::toMath(const QPointF& scene) const {
    return QPointF((scene.x() - m_originX) / m_scale, (m_originY - scene.y()) / m_scale);
}

QPointF CartesianEngine::snapMath(double x, double y) const {
    return QPointF(std::round(x / m_step) * m_step, std::round(y / m_step) * m_step);
}

void CartesianEngine::setSnapTolerance(qreal value) {
    const qreal clamped = qBound(0.0, value, 0.5);
    if (qAbs(clamped - m_snapRatio) < kEpsilon) return;
    m_snapRatio = clamped;
}

void CartesianEngine::setSnapEnabled(bool on) {
    if (m_snapEnabled == on) return;
    m_snapEnabled = on;
}

QPointF CartesianEngine::snappedMath(const QPointF& math) const {
    if (!m_snapEnabled || m_step <= 0.0) return math;
    const QPointF nearest = snapMath(math.x(), math.y());
    const double dx = nearest.x() - math.x();
    const double dy = nearest.y() - math.y();
    if (std::hypot(dx, dy) > snapTolerance()) return math;
    return nearest;
}

bool CartesianEngine::isSnapped(const QPointF& math) const {
    if (!m_snapEnabled || m_step <= 0.0) return false;
    const QPointF nearest = snapMath(math.x(), math.y());
    const double dx = nearest.x() - math.x();
    const double dy = nearest.y() - math.y();
    return std::hypot(dx, dy) <= snapTolerance();
}

QPointF CartesianEngine::snapScene(const QPointF& scene) const {
    const QPointF math = toMath(scene);
    return toScene(snappedMath(math));
}

void CartesianEngine::setSettings(const Settings& settings) {
    m_settings = settings;
    updateSettings();
}

void CartesianEngine::updateSettings() {
    if (m_grid) m_grid->setVisible(m_enabled && m_settings.showGrid);
    for (CartesianPointItem* point : m_points) {
        point->setColor(m_settings.pointColor);
        point->setSize(m_settings.pointSize);
        point->setShape(m_settings.pointShape);
        point->setNameHidden(!m_settings.showPointName);
        point->update();
    }
    for (CartesianSegmentItem* segment : m_segments) {
        segment->setColor(m_settings.segmentColor);
        segment->setEquationVisible(m_settings.showEquation);
        segment->update();
    }
    for (CartesianCurveItem* curve : m_curves) {
        curve->setEquationVisible(m_settings.showEquation);
        curve->update();
    }
    emit objectsChanged();
}

QString CartesianEngine::nextPointName() {
    QSet<QString> used;
    for (const CartesianPointItem* point : m_points) used.insert(point->name());
    for (const QString& candidate : pointNamePool()) {
        if (!used.contains(candidate)) return candidate;
    }
    return QString("P");
}

QStringList CartesianEngine::pointNamePool() {
    static const char* letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    QStringList pool;
    for (int i = 0; i < 26; ++i) pool.append(QString(QLatin1Char(letters[i])));
    for (int i = 0; i < 26; ++i) {
        for (int j = 0; j < 26; ++j) {
            pool.append(QString(QLatin1Char(letters[i])) + QString(QChar('a' + j)));
        }
    }
    return pool;
}

QString CartesianEngine::uniqueFunctionName(const QString& base) const {
    QString candidate = base;
    int serial = 0;
    while (true) {
        bool exists = false;
        for (const CartesianCurveItem* curve : m_curves) {
            if (curve->name() == candidate) {
                exists = true;
                break;
            }
        }
        if (!exists) return candidate;
        ++serial;
        candidate = base + QString(QChar('a' + ((serial - 1) % 26)));
    }
}

int CartesianEngine::addPoint(double x, double y, const QString& name) {
    if (!m_scene) return -1;
    const QPointF placed = snappedMath(QPointF(x, y));

    CartesianPointItem* point = new CartesianPointItem(this, m_points.size());
    point->setZValue(20);
    point->setColor(m_settings.pointColor);
    point->setSize(m_settings.pointSize);
    point->setShape(m_settings.pointShape);
    point->setNameHidden(!m_settings.showPointName);
    point->setName(name.isEmpty() ? nextPointName() : name);
    point->setMathPos(placed.x(), placed.y());
    m_scene->addItem(point);
    m_points.push_back(point);

    emit objectsChanged();
    return m_points.size() - 1;
}

CartesianPointItem* CartesianEngine::pointAt(int index) const {
    if (index < 0 || index >= m_points.size()) return nullptr;
    return m_points.at(index);
}

CartesianPointItem* CartesianEngine::pointAtScene(const QPointF& scenePos) const {
    const qreal tolerance = 14.0;
    CartesianPointItem* best = nullptr;
    qreal bestDistance = tolerance;
    for (CartesianPointItem* point : m_points) {
        const QPointF delta = point->pos() - scenePos;
        const qreal distance = std::hypot(delta.x(), delta.y());
        if (distance <= bestDistance) {
            bestDistance = distance;
            best = point;
        }
    }
    return best;
}

CartesianCurveItem* CartesianEngine::curveAt(int index) const {
    if (index < 0 || index >= m_curves.size()) return nullptr;
    return m_curves.at(index);
}

void CartesianEngine::removePoint(int index) {
    if (index < 0 || index >= m_points.size()) return;
    CartesianPointItem* point = m_points.at(index);
    m_selectedPoints.removeAll(point);
    for (int i = m_segments.size() - 1; i >= 0; --i) {
        CartesianSegmentItem* segment = m_segments.at(i);
        if (segment->start() == point || segment->end() == point) {
            if (m_scene) m_scene->removeItem(segment);
            delete segment;
            m_segments.removeAt(i);
        }
    }
    if (m_scene) m_scene->removeItem(point);
    delete point;
    m_points.removeAt(index);
    for (int i = 0; i < m_points.size(); ++i) m_points.at(i)->setIndex(i);
    for (int i = 0; i < m_segments.size(); ++i) m_segments.at(i)->setIndex(i);
    emit pointSelectionChanged(m_selectedPoints.size());
    emit objectsChanged();
}

void CartesianEngine::clearObjects() {
    for (CartesianSegmentItem* segment : m_segments) {
        if (m_scene) m_scene->removeItem(segment);
        delete segment;
    }
    m_segments.clear();
    for (CartesianCurveItem* curve : m_curves) {
        if (m_scene) m_scene->removeItem(curve);
        delete curve;
    }
    m_curves.clear();
    for (CartesianPointItem* point : m_points) {
        if (m_scene) m_scene->removeItem(point);
        delete point;
    }
    m_points.clear();
    m_selectedPoints.clear();
    emit pointSelectionChanged(0);
    emit objectsChanged();
}

int CartesianEngine::linkPoints(CartesianPointItem* a, CartesianPointItem* b) {
    if (!m_scene || !a || !b || a == b) return -1;
    CartesianSegmentItem* segment = new CartesianSegmentItem(this, m_segments.size(), a, b);
    segment->setZValue(10);
    segment->setColor(m_settings.segmentColor);
    m_scene->addItem(segment);
    m_segments.push_back(segment);
    segment->refreshGeometry();
    emit objectsChanged();
    return m_segments.size() - 1;
}

void CartesianEngine::rebuildSegments() {
    for (CartesianSegmentItem* segment : m_segments) {
        if (m_scene) m_scene->removeItem(segment);
        delete segment;
    }
    m_segments.clear();
}

void CartesianEngine::refreshSegmentsFor(const CartesianPointItem* point) {
    if (!point) return;
    for (CartesianSegmentItem* segment : m_segments) {
        if (segment->start() != point && segment->end() != point) continue;
        segment->refreshGeometry();
    }
}

void CartesianEngine::removeSegment(int index) {
    if (index < 0 || index >= m_segments.size()) return;
    CartesianSegmentItem* segment = m_segments.at(index);
    if (m_scene && segment->scene() == m_scene) m_scene->removeItem(segment);
    delete segment;
    m_segments.removeAt(index);
    for (int i = 0; i < m_segments.size(); ++i) m_segments.at(i)->setIndex(i);
    emit objectsChanged();
}

int CartesianEngine::addFunction(const QString& name, const QString& expression, const QColor& color) {
    if (!m_scene) return -1;
    CartesianCurveItem* curve = new CartesianCurveItem(this, m_curves.size());
    curve->setZValue(5);
    curve->setColor(color);
    curve->setName(name.isEmpty() ? uniqueFunctionName(QStringLiteral("f")) : name);
    if (!curve->setExpression(expression)) {
        delete curve;
        return -1;
    }
    m_scene->addItem(curve);
    m_curves.push_back(curve);
    emit functionChanged();
    emit objectsChanged();
    return m_curves.size() - 1;
}

void CartesianEngine::removeCurve(const QString& name) {
    for (int i = 0; i < m_curves.size(); ++i) {
        if (m_curves.at(i)->name() != name) continue;
        CartesianCurveItem* curve = m_curves.takeAt(i);
        if (m_scene) m_scene->removeItem(curve);
        delete curve;
        for (int k = 0; k < m_curves.size(); ++k) m_curves.at(k)->setIndex(k);
        emit functionChanged();
        emit objectsChanged();
        return;
    }
}

void CartesianEngine::clearFunctions() {
    for (CartesianCurveItem* curve : m_curves) {
        if (m_scene) m_scene->removeItem(curve);
        delete curve;
    }
    m_curves.clear();
    emit functionChanged();
    emit objectsChanged();
}

void CartesianEngine::refreshAll() {
    for (CartesianPointItem* point : m_points) {
        point->setPos(toScene(point->x(), point->y()));
        point->update();
    }
    for (CartesianSegmentItem* segment : m_segments) {
        segment->refreshGeometry();
    }
    for (CartesianCurveItem* curve : m_curves) {
        curve->update();
    }
    if (m_grid) m_grid->update();
    emit objectsChanged();
}

void CartesianEngine::updateViewport(const QRectF& viewportSceneRect) {
    if (!m_grid) return;
    m_grid->setViewport(viewportSceneRect);
    const QPointF topLeft = toMath(viewportSceneRect.topLeft());
    const QPointF bottomRight = toMath(viewportSceneRect.bottomRight());
    const double minX = std::min(topLeft.x(), bottomRight.x());
    const double maxX = std::max(topLeft.x(), bottomRight.x());
    const double minY = std::min(topLeft.y(), bottomRight.y());
    const double maxY = std::max(topLeft.y(), bottomRight.y());
    for (CartesianCurveItem* curve : m_curves) {
        curve->setVisibleRange(minX, maxX, minY, maxY);
    }
}

// ---------------------------------------------------------------- grid

CartesianGridItem::CartesianGridItem(CartesianEngine* engine, QGraphicsItem* parent)
    : QGraphicsObject(parent), m_engine(engine) {
    setAcceptedMouseButtons(Qt::NoButton);
    setZValue(-1000);
}

void CartesianGridItem::setViewport(const QRectF& rect) {
    prepareGeometryChange();
    m_viewport = rect.adjusted(-600.0, -600.0, 600.0, 600.0);
    update();
}

QRectF CartesianGridItem::boundingRect() const {
    return m_viewport;
}

void CartesianGridItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                              QWidget* widget) {
    Q_UNUSED(widget);
    if (!m_engine) return;

    const CartesianEngine::Settings& settings = m_engine->settings();
    const qreal step = m_engine->step();
    const qreal scale = m_engine->scale();

    QRectF area;
    if (option && !option->exposedRect.isEmpty()) {
        area = option->exposedRect;
    } else {
        area = m_viewport;
    }
    if (area.isEmpty()) return;

    painter->setRenderHint(QPainter::Antialiasing, true);

    const QPointF mathTopLeft = m_engine->toMath(area.topLeft());
    const QPointF mathBottomRight = m_engine->toMath(area.bottomRight());
    const double x0 = mathTopLeft.x();
    const double x1 = mathBottomRight.x();
    const double y0 = mathBottomRight.y();
    const double y1 = mathTopLeft.y();

    const double majorStep = step;
    const double minorStep = step / 5.0;

    if (minorStep * scale > 6.0) {
        painter->setPen(QPen(gridMinorColor(), 1.0));
        const double startX = std::floor(x0 / minorStep) * minorStep;
        for (double x = startX; x <= x1; x += minorStep) {
            const double remainder = std::fmod(x, majorStep);
            if (std::abs(remainder) < minorStep * 0.5 ||
                std::abs(remainder - majorStep) < minorStep * 0.5) continue;
            const double sx = m_engine->toScene(x, 0).x();
            painter->drawLine(QPointF(sx, area.top()), QPointF(sx, area.bottom()));
        }
        const double startY = std::floor(y0 / minorStep) * minorStep;
        for (double y = startY; y <= y1; y += minorStep) {
            const double remainder = std::fmod(y, majorStep);
            if (std::abs(remainder) < minorStep * 0.5 ||
                std::abs(remainder - majorStep) < minorStep * 0.5) continue;
            const double sy = m_engine->toScene(0, y).y();
            painter->drawLine(QPointF(area.left(), sy), QPointF(area.right(), sy));
        }
    }

    painter->setPen(QPen(gridColor(), 1.0));
    const double startMajorX = std::floor(x0 / majorStep) * majorStep;
    for (double x = startMajorX; x <= x1; x += majorStep) {
        const double sx = m_engine->toScene(x, 0).x();
        painter->drawLine(QPointF(sx, area.top()), QPointF(sx, area.bottom()));
    }
    const double startMajorY = std::floor(y0 / majorStep) * majorStep;
    for (double y = startMajorY; y <= y1; y += majorStep) {
        const double sy = m_engine->toScene(0, y).y();
        painter->drawLine(QPointF(area.left(), sy), QPointF(area.right(), sy));
    }

    const QPointF origin = m_engine->toScene(0.0, 0.0);
    painter->setPen(QPen(axisColor(), 1.6));
    painter->drawLine(QPointF(origin.x(), area.top()), QPointF(origin.x(), area.bottom()));
    painter->drawLine(QPointF(area.left(), origin.y()), QPointF(area.right(), origin.y()));

    if (!settings.showAxisNumber) return;

    QFont font = UiFonts::mono(8, false);
    painter->setFont(font);
    const QFontMetrics metrics(font);

    painter->setPen(QColor(142, 152, 168));
    for (double x = startMajorX; x <= x1; x += majorStep) {
        if (std::abs(x) < majorStep * 0.5) continue;
        const QPointF anchor = m_engine->toScene(x, 0);
        const QString text = formatAxisNumber(x);
        const double w = metrics.horizontalAdvance(text);
        painter->drawText(QPointF(anchor.x() - w / 2.0, anchor.y() + 13.0), text);
    }
    for (double y = startMajorY; y <= y1; y += majorStep) {
        if (std::abs(y) < majorStep * 0.5) continue;
        const QPointF anchor = m_engine->toScene(0, y);
        const QString text = formatAxisNumber(y);
        const double h = metrics.height();
        painter->drawText(QPointF(anchor.x() + 5.0, anchor.y() - h / 2.0 - 1.0), text);
    }
}

// ---------------------------------------------------------------- point

CartesianPointItem::CartesianPointItem(CartesianEngine* engine, int index, QGraphicsItem* parent)
    : QGraphicsObject(parent), m_engine(engine), m_index(index) {
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::NoButton);
    setCursor(Qt::PointingHandCursor);
}

QRectF CartesianPointItem::boundingRect() const {
    const qreal pad = 28.0;
    return QRectF(-pad, -pad, pad * 2.0, pad * 2.0);
}

void CartesianPointItem::setMathPos(double x, double y) {
    if (qAbs(m_x - x) < 1e-9 && qAbs(m_y - y) < 1e-9) return;
    m_x = x;
    m_y = y;
    if (m_engine) setPos(m_engine->toScene(x, y));
    update();
    if (m_engine) {
        m_engine->refreshSegmentsFor(this);
        emit m_engine->pointMoved(m_index, x, y);
    }
}

void CartesianPointItem::setName(const QString& name) {
    if (m_name == name) return;
    m_name = name;
    update();
}

void CartesianPointItem::setNameHidden(bool hidden) {
    if (m_nameHidden == hidden) return;
    m_nameHidden = hidden;
    update();
}

void CartesianPointItem::setColor(const QColor& color) {
    if (m_color == color) return;
    m_color = color;
    update();
}

void CartesianPointItem::setSize(qreal size) {
    if (qAbs(m_size - size) < kEpsilon) return;
    m_size = size;
    update();
}

void CartesianPointItem::setShape(int shape) {
    if (m_shape == shape) return;
    m_shape = shape;
    update();
}

QPainterPath CartesianPointItem::shape() const {
    QPainterPath path;
    const qreal r = m_size + 4.0;
    path.addEllipse(QPointF(0, 0), r, r);
    return path;
}

void CartesianPointItem::setHighlighted(bool on) {
    if (m_highlighted == on) return;
    m_highlighted = on;
    update();
}

void CartesianPointItem::setSelected(bool on) {
    if (m_selected == on) return;
    m_selected = on;
    update();
}

void CartesianPointItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                               QWidget* widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setRenderHint(QPainter::Antialiasing, true);

    const qreal r = m_size;

    if (m_highlighted) {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(m_color.red(), m_color.green(), m_color.blue(), 60));
        painter->drawEllipse(QPointF(0, 0), r * 2.2, r * 2.2);
    }

    if (m_selected) {
        painter->setPen(QPen(m_color.darker(120), 1.6, Qt::SolidLine, Qt::RoundCap));
        painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(QPointF(0, 0), r + 5.0, r + 5.0);
    }

    painter->setPen(QPen(m_color.darker(125), 1.6));
    painter->setBrush(m_color);
    switch (m_shape) {
    case 1:
        painter->drawRect(QRectF(-r, -r, r * 2.0, r * 2.0));
        break;
    case 2: {
        QPainterPath diamond;
        diamond.moveTo(0, -r * 1.28);
        diamond.lineTo(r * 1.28, 0);
        diamond.lineTo(0, r * 1.28);
        diamond.lineTo(-r * 1.28, 0);
        diamond.closeSubpath();
        painter->drawPath(diamond);
        break;
    }
    case 3: {
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(m_color, 2.4, Qt::SolidLine, Qt::RoundCap));
        painter->drawLine(QPointF(-r, -r), QPointF(r, r));
        painter->drawLine(QPointF(-r, r), QPointF(r, -r));
        break;
    }
    default:
        painter->drawEllipse(QPointF(0, 0), r, r);
        break;
    }

    if (!m_nameHidden && !m_name.isEmpty()) {
        paintLabel(painter, QPointF(r + 5.0, -r - 1.0), m_name, m_color.darker(140));
    }
}

void CartesianPointItem::paintLabel(QPainter* painter, const QPointF& anchor, const QString& text,
                                    const QColor& color) {
    QFont font = UiFonts::forText(text, 10, true);
    painter->setFont(font);
    const QFontMetrics metrics(font);
    const QRectF box(anchor.x(), anchor.y() - metrics.height() + 3.0,
                     metrics.horizontalAdvance(text) + 5.0, metrics.height());
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(255, 255, 255, 216));
    painter->drawRoundedRect(box, 3, 3);
    painter->setPen(color);
    painter->drawText(box, Qt::AlignCenter, text);
}

void CartesianPointItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event) {
    setCursor(Qt::PointingHandCursor);
    update();
    QGraphicsObject::hoverEnterEvent(event);
}

void CartesianPointItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event) {
    setCursor(Qt::ArrowCursor);
    update();
    QGraphicsObject::hoverLeaveEvent(event);
}

// ---------------------------------------------------------------- segment

CartesianSegmentItem::CartesianSegmentItem(CartesianEngine* engine, int index,
                                             CartesianPointItem* a, CartesianPointItem* b,
                                             QGraphicsItem* parent)
    : QGraphicsObject(parent), m_engine(engine), m_index(index), m_a(a), m_b(b) {
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    setAcceptedMouseButtons(Qt::NoButton);
    setZValue(10);
}

void CartesianSegmentItem::refreshGeometry() {
    if (m_a && m_b) {
        const QPointF p1 = m_a->pos();
        const QPointF p2 = m_b->pos();
        const QRectF rect = QRectF(p1, p2).normalized().adjusted(-40, -40, 40, 40);
        prepareGeometryChange();
        setPos(rect.topLeft());
        m_localRect = QRectF(0, 0, rect.width(), rect.height());
    }
    update();
}

QRectF CartesianSegmentItem::boundingRect() const {
    return m_localRect;
}

bool CartesianSegmentItem::hitTest(const QPointF& scenePos, qreal tolerance) const {
    if (!m_a || !m_b) return false;
    return pointToSegmentDistance(scenePos, m_a->pos(), m_b->pos()) <= tolerance;
}

void CartesianSegmentItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                                  QWidget* widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (!m_a || !m_b || !m_engine) return;

    painter->setRenderHint(QPainter::Antialiasing, true);
    const QPointF p1 = m_a->pos() - pos();
    const QPointF p2 = m_b->pos() - pos();

    QPen pen(m_color, 2.0, Qt::SolidLine, Qt::RoundCap);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->drawLine(p1, p2);

    if (m_equationVisible) {
        const QString text = CartesianExpression::describeSegment(m_a->x(), m_a->y(), m_b->x(), m_b->y());
        if (!text.isEmpty()) {
            QFont font = UiFonts::mono(9, true);
            painter->setFont(font);
            const QFontMetrics metrics(font);
            const QPointF mid((p1.x() + p2.x()) / 2.0, (p1.y() + p2.y()) / 2.0);
            const QRectF box(mid.x() + 8.0, mid.y() - 24.0,
                             metrics.horizontalAdvance(text) + 8.0, metrics.height() + 2.0);
            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor(255, 255, 255, 232));
            painter->drawRoundedRect(box, 4, 4);
            painter->setPen(m_color.darker(125));
            painter->drawText(box, Qt::AlignCenter, text);
        }
    }
}

void CartesianSegmentItem::setEquationVisible(bool on) {
    if (m_equationVisible == on) return;
    m_equationVisible = on;
    update();
}

QString CartesianSegmentItem::equationText() const {
    if (!m_a || !m_b) return QString();
    return CartesianExpression::describeSegment(m_a->x(), m_a->y(), m_b->x(), m_b->y());
}

QString CartesianSegmentItem::segmentSummary() const {
    if (!m_a || !m_b) return QString();
    const double dx = m_b->x() - m_a->x();
    const double dy = m_b->y() - m_a->y();
    const double length = std::hypot(dx, dy);
    return QString("长度 %1").arg(CartesianExpression::formatNumber(length, 4));
}

void CartesianSegmentItem::setColor(const QColor& color) {
    if (m_color == color) return;
    m_color = color;
    update();
}

// ---------------------------------------------------------------- curve

CartesianCurveItem::CartesianCurveItem(CartesianEngine* engine, int index, QGraphicsItem* parent)
    : QGraphicsObject(parent), m_engine(engine), m_index(index) {
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    setAcceptedMouseButtons(Qt::NoButton);
    setZValue(5);
}

QRectF CartesianCurveItem::boundingRect() const {
    if (m_runs.isEmpty()) return QRectF(-64.0, -64.0, 128.0, 128.0);
    QRectF bounds = visibleBounds();
    return bounds.adjusted(-40.0, -40.0, 40.0, 40.0);
}

bool CartesianCurveItem::setExpression(const QString& expression) {
    const CartesianExpression::Result result = CartesianExpression::parse(expression);
    if (!result.ok) return false;
    m_expression = expression;
    m_result = result;
    rebuildPolyline();
    update();
    return true;
}

void CartesianCurveItem::setVisibleRange(double minX, double maxX) {
    setVisibleRange(minX, maxX, -1e9, 1e9);
}

void CartesianCurveItem::setVisibleRange(double minX, double maxX, double minY, double maxY) {
    if (qAbs(m_minX - minX) < kEpsilon && qAbs(m_maxX - maxX) < kEpsilon &&
        qAbs(m_minY - minY) < kEpsilon && qAbs(m_maxY - maxY) < kEpsilon) {
        return;
    }
    m_minX = minX;
    m_maxX = maxX;
    m_minY = minY;
    m_maxY = maxY;
    rebuildPolyline();
    update();
}

QRectF CartesianCurveItem::visibleBounds() const {
    if (m_runs.isEmpty()) return QRectF();
    double minX = 0.0, minY = 0.0, maxX = 0.0, maxY = 0.0;
    bool first = true;
    for (const QPolygonF& run : m_runs) {
        for (const QPointF& point : run) {
            if (first) {
                minX = maxX = point.x();
                minY = maxY = point.y();
                first = false;
                continue;
            }
            minX = qMin(minX, point.x());
            maxX = qMax(maxX, point.x());
            minY = qMin(minY, point.y());
            maxY = qMax(maxY, point.y());
        }
    }
    return QRectF(QPointF(minX, minY), QPointF(maxX, maxY));
}

QVector<QPointF> CartesianCurveItem::samplePoints(int maxCount) const {
    QVector<QPointF> points;
    for (const QPolygonF& run : m_runs) {
        for (const QPointF& point : run) {
            if (points.size() >= maxCount) return points;
            points.push_back(point);
        }
    }
    return points;
}

bool CartesianCurveItem::hitTest(const QPointF& scenePos, qreal tolerance) const {
    for (const QPolygonF& run : m_runs) {
        for (int i = 1; i < run.size(); ++i) {
            if (pointToSegmentDistance(scenePos, run.at(i - 1), run.at(i)) <= tolerance) {
                return true;
            }
        }
    }
    return false;
}

void CartesianCurveItem::rebuildPolyline() {
    m_runs.clear();
    if (!m_engine || !m_result.ok) return;

    const double span = m_maxX - m_minX;
    if (span <= 0.0) return;

    int segments = 720;
    if (span < 2.0) segments = 240;
    else if (span > 200.0) segments = 1400;

    const double padY = (m_maxY - m_minY) * 0.5 + 8.0;
    const double lowY = m_minY - padY;
    const double highY = m_maxY + padY;
    const double maxJump = (highY - lowY) * 2.0 + 16.0;

    QPolygonF current;
    double prevX = m_minX;
    double prevY = CartesianExpression::evaluate(m_result, prevX);
    bool prevInside = prevY >= lowY && prevY <= highY;

    auto flush = [&]() {
        if (current.size() >= 2) m_runs.push_back(current);
        current.clear();
    };

    for (int i = 1; i <= segments; ++i) {
        const double t = static_cast<double>(i) / segments;
        const double x = m_minX + span * t;
        const double y = CartesianExpression::evaluate(m_result, x);
        const bool inside = y >= lowY && y <= highY;

        if (!inside) {
            if (prevInside) {
                current << m_engine->toScene(prevX, prevY);
                flush();
            }
            prevInside = false;
            prevX = x;
            prevY = y;
            continue;
        }

        if (prevInside && std::abs(y - prevY) > maxJump) {
            flush();
        }

        if (prevInside) current << m_engine->toScene(prevX, prevY);
        current << m_engine->toScene(x, y);
        prevInside = true;
        prevX = x;
        prevY = y;
    }
    if (prevInside) current << m_engine->toScene(prevX, prevY);
    flush();

    prepareGeometryChange();
}

void CartesianCurveItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                               QWidget* widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (!m_engine || m_runs.isEmpty()) return;

    painter->setRenderHint(QPainter::Antialiasing, true);
    QPen pen(m_color, 2.4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    for (const QPolygonF& run : m_runs) {
        if (run.size() >= 2) painter->drawPolyline(run);
    }

    if (m_equationVisible && !m_runs.isEmpty() && !m_runs.first().isEmpty()) {
        const QString text = QString("%1(x) = %2").arg(m_name, m_expression);
        QFont font = UiFonts::mono(9, true);
        painter->setFont(font);
        const QFontMetrics metrics(font);
        const QPointF anchor = m_runs.first().first();
        const QRectF box(anchor.x() + 10.0, anchor.y() - 28.0,
                         metrics.horizontalAdvance(text) + 8.0, metrics.height() + 2.0);
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(255, 255, 255, 232));
        painter->drawRoundedRect(box, 4, 4);
        painter->setPen(m_color.darker(125));
        painter->drawText(box, Qt::AlignCenter, text);
    }
}

void CartesianCurveItem::setName(const QString& name) {
    if (m_name == name) return;
    m_name = name;
    update();
}

void CartesianCurveItem::setColor(const QColor& color) {
    if (m_color == color) return;
    m_color = color;
    update();
}

void CartesianCurveItem::setEquationVisible(bool on) {
    if (m_equationVisible == on) return;
    m_equationVisible = on;
    update();
}
