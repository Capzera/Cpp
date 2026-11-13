#include "SceneGrid.h"
#include "GraphElements.h"
#include "ui/UiTheme.h"
#include <QPainter>
#include <QGraphicsScene>
#include <cmath>

SceneGrid::Config SceneGrid::s_config;

void SceneGrid::setConfig(QGraphicsScene* scene, const Config& config) {
    s_config = config;
    if (!scene) return;
    for (QGraphicsItem* item : scene->items()) {
        if (dynamic_cast<GridGuideLayer*>(item)) {
            item->update();
            return;
        }
    }
}

SceneGrid::Config SceneGrid::currentConfig() {
    return s_config;
}

void SceneGrid::attach(QGraphicsScene* scene, const Config& config) {
    if (!scene) return;
    s_config = config;
    for (QGraphicsItem* item : scene->items()) {
        if (dynamic_cast<GridGuideLayer*>(item)) {
            item->update();
            return;
        }
    }
    GridGuideLayer* layer = new GridGuideLayer(scene);
    layer->setZValue(-100);
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

void SceneGrid::drawGuides(QPainter* painter, const QGraphicsScene* scene, const Config& config) {
    if (!painter || !scene) return;
    QRectF bounds = scene->itemsBoundingRect().adjusted(-300, -300, 300, 300);
    qreal step = config.cell > 0 ? config.cell : 56.0;

    painter->setPen(QPen(UiTheme::canvasGrid(), 1, Qt::DotLine));
    for (qreal x = std::floor(bounds.left() / step) * step; x <= bounds.right(); x += step) {
        painter->drawLine(QPointF(x, bounds.top()), QPointF(x, bounds.bottom()));
    }
    for (qreal y = std::floor(bounds.top() / step) * step; y <= bounds.bottom(); y += step) {
        painter->drawLine(QPointF(bounds.left(), y), QPointF(bounds.right(), y));
    }
}

GridGuideLayer::GridGuideLayer(QGraphicsScene* scene) {
    setZValue(-100);
    setAcceptedMouseButtons(Qt::NoButton);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
}

void GridGuideLayer::refresh() {
    update();
}

QRectF GridGuideLayer::boundingRect() const {
    if (!scene()) return QRectF();
    return scene()->itemsBoundingRect().adjusted(-600, -600, 600, 600);
}

void GridGuideLayer::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    SceneGrid::Config cfg = SceneGrid::currentConfig();
    if (!cfg.showGuides) return;
    painter->setRenderHint(QPainter::Antialiasing);
    SceneGrid::drawGuides(painter, scene(), cfg);
}
