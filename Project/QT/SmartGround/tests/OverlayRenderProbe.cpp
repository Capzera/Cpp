#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QPainter>
#include <QImage>
#include <QElapsedTimer>
#include <cstdio>

class ProbeView : public QGraphicsView {
public:
    explicit ProbeView(QGraphicsScene* scene) : QGraphicsView(scene) {}
    QGraphicsScene* overlay = nullptr;
    int strategy = 0;
protected:
    void drawForeground(QPainter* painter, const QRectF& rect) override {
        QGraphicsView::drawForeground(painter, rect);
        if (!overlay) return;
        painter->save();
        painter->setTransform(transform(), false);
        painter->setClipRect(viewport()->rect());
        if (strategy == 1) {
            QRectF src = overlay->itemsBoundingRect();
            overlay->render(painter, src, src, Qt::IgnoreAspectRatio);
        } else if (strategy == 2) {
            overlay->render(painter, QRectF(), overlay->sceneRect(), Qt::IgnoreAspectRatio);
        }
        painter->restore();
    }
};

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    for (int strategy = 1; strategy <= 2; ++strategy) {
        QGraphicsScene base;
        base.setSceneRect(-200, -200, 800, 600);
        base.setBackgroundBrush(Qt::white);
        QGraphicsRectItem* r = base.addRect(-50, -50, 100, 100);
        r->setBrush(Qt::red);

        QGraphicsScene overlay;
        QGraphicsEllipseItem* e = overlay.addEllipse(-30, -30, 60, 60);
        e->setBrush(Qt::blue);

        ProbeView view(&base);
        view.overlay = &overlay;
        view.strategy = strategy;
        view.resize(400, 300);
        view.setBackgroundBrush(Qt::white);
        view.show();
        QApplication::processEvents();
        QImage img = view.grab().toImage();
        QColor center = img.pixelColor(200, 150);
        QColor offcenter = img.pixelColor(215, 150);
        printf("strategy %d: center=%s off=%s\n", strategy,
               center.name().toUtf8().constData(), offcenter.name().toUtf8().constData());
        img.save(QString("probe_strategy_%1.png").arg(strategy));
    }

    QGraphicsScene base;
    base.setSceneRect(-200, -200, 800, 600);
    QGraphicsScene overlay;
    for (int i = 0; i < 400; ++i) {
        QGraphicsEllipseItem* e = overlay.addEllipse(i * 3, i * 2, 20, 20);
        e->setBrush(Qt::blue);
    }
    ProbeView view(&base);
    view.overlay = &overlay;
    view.strategy = 1;
    view.resize(800, 600);
    view.show();
    QApplication::processEvents();
    QElapsedTimer timer;
    timer.start();
    for (int i = 0; i < 30; ++i) view.grab();
    printf("overlay render 400 items x30 frames: %lld ms\n", timer.elapsed());
    return 0;
}
