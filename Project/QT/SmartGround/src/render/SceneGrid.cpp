#include "SceneGrid.h"
#include "GraphElements.h"
#include "ui/UiTheme.h"
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <cmath>

SceneGrid::Config SceneGrid::s_config;

void SceneGrid::setConfig(QGraphicsScene* scene, const Config& config) {
    s_config = config;
    if (!scene) return;
    if (GridGuideScene* guided = dynamic_cast<GridGuideScene*>(scene)) {
        guided->refresh();
    }
}

SceneGrid::Config SceneGrid::currentConfig() {
    return s_config;
}

QPointF SceneGrid::snapToCell(const QPointF& pos, const Config& config) {
    if (!config.enabled || config.cell <= 0) return pos;
    qreal step = config.cell;
    return QPointF(std::round(pos.x() / step) * step, std::round(pos.y() / step) * step);
}

QPointF SceneGrid::magneticAlign(QGraphicsScene* scene, const GraphNode* mover, const QPointF& proposed, const Config& config) {
    if (!scene || !mover || !config.enabled) return proposed;
    QPointF result = proposed;
    qreal range = config.magnetRange;
    qreal step = config.cell > 0 ? config.cell : 56.0;

    qreal bestDx = range + 1;
    qreal bestDy = range + 1;
    QPointF candidate = proposed;

    for (QGraphicsItem* item : scene->items()) {
        GraphNode* other = dynamic_cast<GraphNode*>(item);
        if (!other || other == mover) continue;
        QPointF op = other->pos();

        qreal dx = op.x() - proposed.x();
        if (std::abs(dx) < range && std::abs(dx) < std::abs(bestDx)) {
            bestDx = std::abs(dx);
            candidate.setX(op.x());
        }
        qreal dy = op.y() - proposed.y();
        if (std::abs(dy) < range && std::abs(dy) < std::abs(bestDy)) {
            bestDy = std::abs(dy);
            candidate.setY(op.y());
        }
    }

    if (bestDx > range && bestDy > range) {
        return snapToCell(proposed, config);
    }
    (void)step;
    return candidate;
}

QPolygonF SceneGrid::hexagonAt(const QPointF& center, qreal radius) {
    QPolygonF poly;
    for (int i = 0; i < 6; ++i) {
        qreal angle = M_PI / 6.0 + i * M_PI / 3.0;
        poly << QPointF(center.x() + radius * std::cos(angle), center.y() + radius * std::sin(angle));
    }
    return poly;
}

void SceneGrid::drawGuides(QPainter* painter, const QRectF& bounds, const Config& config) {
    if (!painter) return;
    if (!config.enabled || !config.showGuides) return;
    qreal step = config.cell > 0 ? config.cell : 56.0;
    if (step <= 0.0) return;

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(QPen(UiTheme::canvasGrid(), 1, Qt::DotLine));
    for (qreal x = std::floor(bounds.left() / step) * step; x <= bounds.right(); x += step) {
        painter->drawLine(QPointF(x, bounds.top()), QPointF(x, bounds.bottom()));
    }
    for (qreal y = std::floor(bounds.top() / step) * step; y <= bounds.bottom(); y += step) {
        painter->drawLine(QPointF(bounds.left(), y), QPointF(bounds.right(), y));
    }
    painter->restore();
}

GridGuideScene::GridGuideScene(QObject* parent) : QGraphicsScene(parent) {
    setBackgroundBrush(UiTheme::canvasBg());
}

void GridGuideScene::refresh() {
    if (views().isEmpty()) {
        setSceneRect(sceneRect());
        return;
    }
    for (QGraphicsView* view : views()) {
        view->viewport()->update();
    }
}

void GridGuideScene::drawBackground(QPainter* painter, const QRectF& rect) {
    QGraphicsScene::drawBackground(painter, rect);
    SceneGrid::Config cfg = SceneGrid::currentConfig();
    SceneGrid::drawGuides(painter, rect, cfg);
}
