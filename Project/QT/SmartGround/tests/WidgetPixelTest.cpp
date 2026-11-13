#include <QApplication>
#include <QPixmap>
#include <QPainter>
#include <QImage>
#include <QThread>
#include <cstdio>
#include "ui/widgets/MyToggleSwitch.h"
#include "ui/widgets/MyButton.h"
#include "ui/widgets/MyCheckBox.h"
#include "ui/UiTheme.h"
#include "render/GraphElements.h"
#include "render/SceneGrid.h"

static int failures = 0;
static void check(bool cond, const char* msg) {
    if (!cond) { printf("FAIL: %s\n", msg); failures++; }
    else printf("PASS: %s\n", msg);
}

static int countGreenish(const QImage& img) {
    int n = 0;
    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            QColor c = img.pixelColor(x, y);
            if (c.green() > 150 && c.red() < 130 && c.blue() < 130) n++;
        }
    }
    return n;
}

static int countGrayish(const QImage& img) {
    int n = 0;
    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            QColor c = img.pixelColor(x, y);
            if (std::abs(c.red() - c.green()) < 14 && std::abs(c.green() - c.blue()) < 14 && c.red() > 170 && c.red() < 235) n++;
        }
    }
    return n;
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    {
        MyToggleSwitch sw;
        sw.setOffText("有向图");
        sw.setOnText("无向图");
        sw.resize(96, 32);
        sw.setChecked(false);
        QPixmap pm = sw.grab();
        QImage off = pm.toImage();
        int offGray = countGrayish(off);
        int offGreen = countGreenish(off);
        printf("OFF: gray=%d green=%d\n", offGray, offGreen);
        check(offGray > 300, "toggle OFF renders gray track");
        check(offGreen < 50, "toggle OFF has no green");

        sw.setChecked(true);
        app.processEvents();
        for (int i = 0; i < 40; ++i) { app.processEvents(); QThread::msleep(8); }
        QImage on = sw.grab().toImage();
        int onGreen = countGreenish(on);
        int onGray = countGrayish(on);
        printf("ON: gray=%d green=%d\n", onGray, onGreen);
        check(onGreen > 300, "toggle ON renders iOS green track");
        check(onGreen > offGreen + 200, "toggle switches gray -> green");
    }

    {
        MyCheckBox cb("无向边加双向箭头");
        cb.setChecked(true);
        cb.setRatio(1.0);
        cb.resize(220, 26);
        QImage full = cb.grab().toImage();
        int fullDark = 0;
        for (int y = 0; y < full.height(); ++y)
            for (int x = 0; x < full.width(); ++x)
                if (full.pixelColor(x, y).lightness() < 140) fullDark++;

        cb.setRatio(0.0);
        QImage faded = cb.grab().toImage();
        int fadedDark = 0;
        for (int y = 0; y < faded.height(); ++y)
            for (int x = 0; x < faded.width(); ++x)
                if (faded.pixelColor(x, y).lightness() < 140) fadedDark++;
        printf("checkbox dark: full=%d faded=%d\n", fullDark, fadedDark);
        check(fadedDark < fullDark, "sub-option dims out when collapsed");
    }

    {
        MyButton solid("绘制");
        solid.setVariant(MyButton::Solid);
        solid.resize(120, 38);
        QImage img = solid.grab().toImage();
        QColor mid = img.pixelColor(8, 19);
        printf("solid button fill color=%s\n", mid.name().toUtf8().constData());
        check(mid.blue() > mid.red() && mid.blue() > 150, "solid button uses accent blue");
    }

    {
        QGraphicsScene scene;
        GraphNode* a = new GraphNode(1);
        GraphNode* b = new GraphNode(2);
        scene.addItem(a);
        scene.addItem(b);
        a->setPos(0, 0);
        b->setPos(220, 0);
        GraphEdge* e = new GraphEdge(a, b, true, false);
        scene.addItem(e);
        a->addEdge(e);
        b->addEdge(e);

        QImage img(300, 90, QImage::Format_ARGB32);
        img.fill(Qt::white);
        QPainter painter(&img);
        scene.render(&painter, QRectF(0, 0, 300, 90), QRectF(-30, -30, 280, 90));
        painter.end();

        int whiteRing = 0;
        int blackTri = 0;
        for (int y = 30; y < 60; ++y) {
            for (int x = 160; x < 200; ++x) {
                QColor c = img.pixelColor(x, y);
                if (c.red() > 235 && c.green() > 235 && c.blue() > 235) whiteRing++;
                if (c.red() < 70 && c.green() < 70 && c.blue() < 70) blackTri++;
            }
        }
        printf("arrow zone: whiteRing=%d blackTriangle=%d\n", whiteRing, blackTri);
        check(blackTri > 30, "arrowhead triangle is solid and prominent");
        check(whiteRing > 30, "arrowhead has white outline for contrast");
    }

    {
        SceneGrid::Config cfg;
        cfg.enabled = true;
        cfg.cell = 56.0;
        cfg.magnetRange = 18.0;
        QPointF snapped = SceneGrid::snapToCell(QPointF(70, 30), cfg);
        printf("snap: (70,30) -> (%.1f,%.1f)\n", snapped.x(), snapped.y());
        check(std::abs(snapped.x() - 56.0) < 0.01 && std::abs(snapped.y() - 56.0) < 0.01,
              "cell snapping rounds to 56px lattice");
    }

    printf("\n%d failures\n", failures);
    return failures == 0 ? 0 : 1;
}
