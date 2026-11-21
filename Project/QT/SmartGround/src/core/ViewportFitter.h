#ifndef VIEWPORTFITTER_H
#define VIEWPORTFITTER_H
#include <QGraphicsScene>
#include <QGraphicsView>

class ViewportFitter {
public:
    struct Options {
        qreal margin = 70.0;
        qreal minScale = 0.25;
        qreal maxScale = 2.40;
        bool keepAspect = true;
        bool centerAfterFit = true;
    };

    static void fitScene(QGraphicsView* view, QGraphicsScene* scene, const Options& options);

    static void fitToItems(QGraphicsView* view, QGraphicsScene* scene, const Options& options);

    static void expandSceneRect(QGraphicsScene* scene, const Options& options);

    static QRectF contentRect(QGraphicsScene* scene);

    static void setInfiniteCanvas(QGraphicsScene* scene, bool on);
    static bool infiniteCanvas(const QGraphicsScene* scene);
};

#endif
