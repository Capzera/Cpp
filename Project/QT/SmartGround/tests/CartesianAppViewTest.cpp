#include <QApplication>
#include <QTimer>
#include <QGraphicsScene>
#include <QPainter>
#include <QLabel>
#include <cstdio>
#include "ui/MainWindow.h"
#include "ui/widgets/MyComboBox.h"
#include "ui/widgets/CartesianPanel.h"
#include "ui/widgets/MyTextInput.h"
#include "ui/widgets/MyButton.h"
#include "render/CartesianElements.h"
#include "canvas/CanvasView.h"

static int g_fail = 0;

static void check(bool ok, const QString& name) {
    printf("[%s] %s\n", ok ? "PASS" : "FAIL", qPrintable(name));
    if (!ok) g_fail++;
}

static void pump(int ms) {
    QEventLoop loop;
    QTimer::singleShot(ms, &loop, &QEventLoop::quit);
    loop.exec();
}

static int findCartesianIndex(MyComboBox* combo) {
    for (int i = 0; i < combo->count(); ++i) {
        if (combo->itemText(i).contains("笛卡尔坐标系")) return i;
    }
    return -1;
}

int main(int argc, char** argv) {
    setvbuf(stdout, nullptr, _IONBF, 0);
    QApplication app(argc, argv);

    MainWindow w;
    w.resize(1400, 860);
    w.show();
    QApplication::processEvents();

    MyComboBox* combo = w.findChild<MyComboBox*>();
    combo->setCurrentIndex(findCartesianIndex(combo));
    pump(400);

    CartesianPanel* panel = w.findChild<CartesianPanel*>();
    CartesianEngine* engine = panel ? panel->engine() : nullptr;
    CanvasView* view = w.findChild<CanvasView*>();
    check(engine != nullptr, "engine present");
    check(view != nullptr, "view present");
    if (!engine || !view) return 1;

    printf("engine origin=%f,%f scale=%f step=%f\n",
           engine->originScene().x(), engine->originScene().y(),
           engine->scale(), engine->step());

    MyTextInput* fnInput = panel->findChild<MyTextInput*>("cartesianFunctionInput");
    MyButton* addButton = panel->findChild<MyButton*>("cartesianFunctionAdd");
    check(fnInput != nullptr, "function input present");
    check(addButton != nullptr, "function add button present");

    fnInput->setText("3x^2 - 8x + 7");
    pump(60);
    addButton->click();
    pump(300);

    printf("curveCount=%d\n", engine->curveCount());
    check(engine->curveCount() == 1, "curve added through the real UI path");

    CartesianCurveItem* curve = engine->curveAt(0);
    if (curve) {
        printf("runs=%d pts=%d bounds=%f,%f %fx%f\n",
               curve->visibleRunCount(), curve->visiblePointCount(),
               curve->visibleBounds().x(), curve->visibleBounds().y(),
               curve->visibleBounds().width(), curve->visibleBounds().height());
    }

    const QPointF tl = view->mapToSceneF(QPointF(0, 0));
    const QPointF br = view->mapToSceneF(QPointF(view->viewport()->width(), view->viewport()->height()));
    printf("viewport scene = %f,%f %fx%f\n", tl.x(), tl.y(), br.x() - tl.x(), br.y() - tl.y());

    engine->updateViewport(QRectF(tl, br).normalized());
    pump(120);

    QImage img(view->viewport()->size(), QImage::Format_ARGB32);
    img.fill(Qt::white);
    QPainter p(&img);
    view->render(&p);
    p.end();

    int curvePixels = 0;
    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            const QColor c = img.pixelColor(x, y);
            if (c.red() > 170 && c.green() < 150 && c.blue() < 130) ++curvePixels;
        }
    }
    printf("view curve pixels = %d\n", curvePixels);
    check(curvePixels > 200, "the curve is actually visible in the app view");
    img.save("D:/Program/Cpp/Project/QT/SmartGround/tests/build/cartesian_app_view.png");

    printf("\n%s (%d failures)\n", g_fail == 0 ? "ALL PASS" : "FAILED", g_fail);
    return g_fail == 0 ? 0 : 1;
}
