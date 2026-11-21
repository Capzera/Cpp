#include "ViewportFitter.h"

namespace {

const double kInfiniteExtent = 1.0e7;

bool& infiniteFlag() {
    static bool flag = false;
    return flag;
}

}  // namespace

QRectF ViewportFitter::contentRect(QGraphicsScene* scene) {
    if (!scene) return QRectF(-400, -300, 800, 600);
    QRectF rect = scene->itemsBoundingRect();
    if (rect.isNull() || rect.width() < 1.0 || rect.height() < 1.0) {
        return QRectF(-400, -300, 800, 600);
    }
    return rect;
}

void ViewportFitter::setInfiniteCanvas(QGraphicsScene* scene, bool on) {
    infiniteFlag() = on;
    if (!scene) return;
    if (on) {
        scene->setSceneRect(-kInfiniteExtent, -kInfiniteExtent,
                            kInfiniteExtent * 2.0, kInfiniteExtent * 2.0);
    }
}

bool ViewportFitter::infiniteCanvas(const QGraphicsScene*) {
    return infiniteFlag();
}

void ViewportFitter::expandSceneRect(QGraphicsScene* scene, const Options& options) {
    if (!scene) return;
    if (infiniteFlag()) {
        scene->setSceneRect(-kInfiniteExtent, -kInfiniteExtent,
                            kInfiniteExtent * 2.0, kInfiniteExtent * 2.0);
        return;
    }
    QRectF rect = contentRect(scene);
    scene->setSceneRect(rect.adjusted(-800, -800, 800, 800));
    Q_UNUSED(options);
}

void ViewportFitter::fitToItems(QGraphicsView* view, QGraphicsScene* scene, const Options& options) {
    if (!view || !scene) return;
    QRectF items = contentRect(scene);
    const QRectF target = items.adjusted(-options.margin, -options.margin, options.margin, options.margin);

    view->resetTransform();
    view->fitInView(target, options.keepAspect ? Qt::KeepAspectRatio : Qt::IgnoreAspectRatio);

    qreal scale = view->transform().m11();
    if (scale < options.minScale) {
        view->resetTransform();
        view->scale(options.minScale, options.minScale);
    } else if (scale > options.maxScale) {
        view->resetTransform();
        view->scale(options.maxScale, options.maxScale);
    }
    if (options.centerAfterFit) {
        view->centerOn(items.center());
    }
}

void ViewportFitter::fitScene(QGraphicsView* view, QGraphicsScene* scene, const Options& options) {
    expandSceneRect(scene, options);
    fitToItems(view, scene, options);
}
