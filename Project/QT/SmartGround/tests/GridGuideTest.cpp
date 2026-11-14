#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QImage>
#include <cstdio>
#include "ui/MainWindow.h"
#include "ui/UiTheme.h"
#include "ui/widgets/MyCheckBox.h"
#include "render/SceneGrid.h"

static int countDifferent(const QImage& img, const QColor& base, int tol) {
    int n = 0;
    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            QColor c = img.pixelColor(x, y);
            if (std::abs(c.red() - base.red()) > tol ||
                std::abs(c.green() - base.green()) > tol ||
                std::abs(c.blue() - base.blue()) > tol) ++n;
        }
    }
    return n;
}

static int failures = 0;

static void check(bool ok, const char* label, const QString& detail) {
    printf("[%s] %s %s\n", ok ? "PASS" : "FAIL", label, detail.toUtf8().constData());
    if (!ok) ++failures;
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    GridGuideScene scene;
    scene.setBackgroundBrush(Qt::black);
    scene.setSceneRect(0, 0, 600, 400);
    QGraphicsView view(&scene);
    view.resize(600, 400);
    view.show();
    view.resetTransform();

    SceneGrid::Config cfg;
    cfg.enabled = true;
    cfg.showGuides = true;
    cfg.cell = 56.0;
    SceneGrid::setConfig(&scene, cfg);
    QApplication::processEvents();

    QImage img = view.viewport()->grab().toImage();
    const int gridOn = countDifferent(img, QColor(0, 0, 0), 6);
    check(gridOn > 300, "grid renders when enabled", QString("nonBlackPixels=%1").arg(gridOn));

    check(scene.items().isEmpty(), "grid adds no scene items", QString("items=%1").arg(scene.items().size()));

    SceneGrid::Config off = cfg;
    off.enabled = false;
    off.showGuides = false;
    SceneGrid::setConfig(&scene, off);
    QApplication::processEvents();
    img = view.viewport()->grab().toImage();
    const int gridOff = countDifferent(img, QColor(0, 0, 0), 6);
    check(gridOff < gridOn / 2, "grid hidden when disabled",
          QString("on=%1 off=%2").arg(gridOn).arg(gridOff));

    SceneGrid::setConfig(&scene, cfg);
    QApplication::processEvents();
    QGraphicsRectItem* rect = scene.addRect(0, 0, 100, 100);
    rect->setBrush(Qt::red);
    rect->setPen(Qt::NoPen);
    scene.clear();
    QApplication::processEvents();
    img = view.viewport()->grab().toImage();
    const int gridAfterClear = countDifferent(img, QColor(0, 0, 0), 6);
    check(gridAfterClear > 300, "grid survives scene clear", QString("nonBlackPixels=%1").arg(gridAfterClear));

    MainWindow w;
    w.resize(1400, 900);
    w.show();
    app.processEvents();
    QGraphicsScene* appScene = w.findChild<QGraphicsScene*>();
    check(appScene != nullptr, "main window scene found", QString("found=%1").arg(appScene != nullptr));
    check(dynamic_cast<GridGuideScene*>(appScene) != nullptr, "main window uses GridGuideScene",
          QString("isGridScene=%1").arg(dynamic_cast<GridGuideScene*>(appScene) != nullptr));

    QGraphicsView* appView = appScene->views().isEmpty() ? nullptr : appScene->views().first();
    check(appView != nullptr, "main window view found", QString("found=%1").arg(appView != nullptr));
    if (appView) {
        QImage appImg = appView->viewport()->grab().toImage();
        const int appNonBg = countDifferent(appImg, UiTheme::canvasBg(), 3);
        check(appNonBg > 1000, "main canvas shows grid guides",
              QString("nonBackgroundPixels=%1").arg(appNonBg));
    }

    MyCheckBox* gridToggle = nullptr;
    for (MyCheckBox* c : w.findChildren<MyCheckBox*>()) {
        if (c->text().contains("网格")) gridToggle = c;
    }
    check(gridToggle != nullptr, "grid toggle checkbox found",
          QString("found=%1").arg(gridToggle != nullptr));

    if (gridToggle && appView) {
        const QColor bg = UiTheme::canvasBg();
        gridToggle->setChecked(true);
        app.processEvents();
        QImage onImg = appView->viewport()->grab().toImage();
        const int onPixels = countDifferent(onImg, bg, 3);

        gridToggle->setChecked(false);
        app.processEvents();
        QImage offImg = appView->viewport()->grab().toImage();
        const int offPixels = countDifferent(offImg, bg, 3);

        check(onPixels > offPixels + 500, "grid toggle changes grid visibility",
              QString("on=%1 off=%2").arg(onPixels).arg(offPixels));

        gridToggle->setChecked(true);
        app.processEvents();
    }

    printf("\n%s failures=%d\n", failures == 0 ? "ALL PASS" : "FAILED", failures);
    return failures == 0 ? 0 : 1;
}
