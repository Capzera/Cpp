#ifndef SCENEGRID_H
#define SCENEGRID_H
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPointF>
#include <QLineF>
#include <QPolygonF>

class GraphNode;

class SceneGrid {
public:
    struct Config {
        qreal cell = 56.0;
        bool enabled = true;
        bool showGuides = false;
        qreal magnetRange = 16.0;
    };

    static void attach(QGraphicsScene* scene, const Config& config);
    static void setConfig(QGraphicsScene* scene, const Config& config);
    static Config currentConfig();

    static QPointF snapToCell(const QPointF& pos, const Config& config);

    static QPointF magneticAlign(QGraphicsScene* scene, const GraphNode* mover, const QPointF& proposed, const Config& config);

    static QPolygonF hexagonAt(const QPointF& center, qreal radius);

    static void drawGuides(QPainter* painter, const QGraphicsScene* scene, const Config& config);

private:
    static Config s_config;
};

class GridGuideLayer : public QGraphicsItem {
public:
    explicit GridGuideLayer(QGraphicsScene* scene);
    void refresh();
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;

private:
    QList<QLineF> m_guides;
};

#endif
