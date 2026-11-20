#include "render/GraphElements.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QImage>
#include <QPainter>
#include <cstdio>
#include <cmath>

static QImage render(QGraphicsScene* scene, QSizeF size) {
    QImage img(size.toSize(), QImage::Format_ARGB32);
    img.fill(Qt::white);
    QPainter p(&img);
    scene->render(&p, QRectF(0, 0, size.width(), size.height()),
                  QRectF(0, 0, size.width(), size.height()));
    return img;
}

static int maxDiff(const QImage& a, const QImage& b, double cx, double cy, int radius) {
    const int px = int(cx), py = int(cy);
    int best = 0;
    for (int dy = -radius; dy <= radius; ++dy) {
        for (int dx = -radius; dx <= radius; ++dx) {
            const int x = px + dx, y = py + dy;
            if (x < 0 || y < 0 || x >= a.width() || y >= a.height()) continue;
            const QRgb ca = a.pixel(x, y);
            const QRgb cb = b.pixel(x, y);
            const int d = std::abs(qRed(ca) - qRed(cb)) + std::abs(qGreen(ca) - qGreen(cb))
                        + std::abs(qBlue(ca) - qBlue(cb));
            if (d > best) best = d;
        }
    }
    return best;
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QGraphicsScene scene;
    GraphNode* a = new GraphNode(1);
    a->setPos(0, 0);
    scene.addItem(a);
    GraphNode* b = new GraphNode(2);
    b->setPos(300, 200);
    scene.addItem(b);
    GraphEdge* e = new GraphEdge(a, b, true);
    e->setBendOffset(QPointF(0, -90));
    e->setColor(QColor(60, 60, 60));
    scene.addItem(e);

    const qreal radius = 22.0;
    const QPointF p0 = e->trimmedStartPublic(radius);
    const QPointF p1 = e->trimmedEndPublic(radius);
    const QPointF c = e->controlPointPublic();

    const QSizeF canvas(420, 320);
    int gaps = 0;
    int overshoot = 0;
    int samples = 0;
    int tailSamples = 0;

    e->setBeamProgress(-1.0);
    const QImage base = render(&scene, canvas);

    for (int step = 1; step <= 19; ++step) {
        const qreal head = step / 20.0;
        const qreal tail = qMax(0.0, head - 0.34);
        e->setBeamProgress(head);
        const QImage withBeam = render(&scene, canvas);

        for (int i = 0; i <= 60; ++i) {
            const qreal t = tail + (head - tail) * (i / 60.0);
            const QPointF pt = GraphEdge::quadPointPublic(p0, c, p1, t);
            ++samples;
            if (maxDiff(base, withBeam, pt.x(), pt.y(), 5) < 24) ++gaps;
        }

        for (int i = 1; i <= 20; ++i) {
            const qreal t = head + 0.04 + (1.0 - head - 0.04) * (i / 20.0);
            if (t > 0.995) break;
            const QPointF pt = GraphEdge::quadPointPublic(p0, c, p1, t);
            ++tailSamples;
            if (maxDiff(base, withBeam, pt.x(), pt.y(), 5) >= 24) ++overshoot;
        }
    }

    printf("beam arc samples=%d gaps=%d | beyond-head samples=%d residual=%d\n",
           samples, gaps, tailSamples, overshoot);
    const bool ok = (gaps == 0 && overshoot == 0);
    printf("PASS: %s\n", ok ? "yes" : "no");
    return ok ? 0 : 1;
}
