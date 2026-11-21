#include <QApplication>
#include <QTimer>
#include <QGraphicsScene>
#include <QPainter>
#include <QLabel>
#include <QKeyEvent>
#include <cstdio>
#include "ui/MainWindow.h"
#include "ui/widgets/MyComboBox.h"
#include "ui/widgets/CanvasToolBar.h"
#include "ui/widgets/CartesianPanel.h"
#include "ui/widgets/ToolIconButton.h"
#include "ui/widgets/MyTextInput.h"
#include "ui/widgets/MyButton.h"
#include "render/CartesianElements.h"
#include "render/SceneGrid.h"
#include "canvas/CanvasView.h"
#include "canvas/ToolController.h"

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

static bool widgetVisible(MainWindow& w, const QString& name) {
    for (QWidget* widget : w.findChildren<QWidget*>()) {
        if (widget->objectName() == name) return widget->isVisible();
    }
    return false;
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

    CanvasToolBar* toolbar = w.findChild<CanvasToolBar*>();
    CartesianPanel* panel = w.findChild<CartesianPanel*>();
    CanvasView* view = w.findChild<CanvasView*>();
    ToolController* tc = w.findChild<ToolController*>();
    CartesianEngine* engine = panel ? panel->engine() : nullptr;

    printf("--- issue 1: point tool is reachable and visible ---\n");
    check(toolbar != nullptr, "toolbar present");
    ToolIconButton* pointButton =
        toolbar->cartesianButtonFor(static_cast<int>(ToolIconButton::CartPoint));
    check(pointButton != nullptr, "point tool button exists");
    check(pointButton && pointButton->isVisible(), "point tool button is visible");
    check(pointButton && pointButton->width() > 0 && pointButton->height() > 0,
          "point tool button has a real size");
    if (pointButton) {
        QWidget* row = pointButton->parentWidget();
        QWidget* mainRow = toolbar->findChild<QWidget*>("canvasToolMainRow");
        check(row && mainRow && !row->geometry().intersects(mainRow->geometry()),
              "the cartesian row does not overlap the main tool row");
        check(row && row->geometry().y() >= mainRow->geometry().height(),
              "the cartesian row sits below the main tool row");
        printf("  row = %d,%d %dx%d   mainRow = %d,%d %dx%d\n",
               row->geometry().x(), row->geometry().y(),
               row->geometry().width(), row->geometry().height(),
               mainRow->geometry().x(), mainRow->geometry().y(),
               mainRow->geometry().width(), mainRow->geometry().height());
    }

    printf("--- issue 1b: the point tool is auto-selected on entry ---\n");
    check(tc && tc->currentTool() == ToolController::CartPoint,
          "the point tool is active right after switching to the cartesian module");
    check(engine && engine->activeTool() == CartesianEngine::Point,
          "the engine is in point mode right after switching");

    printf("--- issue 2: algorithm input is hidden in cartesian mode ---\n");
    check(!widgetVisible(w, "myTextEdit") && !widgetVisible(w, "myTextEditRoot"),
          "the algorithm text editor is hidden");
    check(!widgetVisible(w, "gridToggle"), "the alignment-grid toggle is hidden");
    check(!widgetVisible(w, "scriptTextToggle"), "the script-text toggle is hidden");
    int visibleAlgoButtons = 0;
    for (MyButton* b : w.findChildren<MyButton*>()) {
        const QString t = b->text();
        if ((t.contains("构建") || t.contains("播放") || t.contains("下一步") ||
             t.contains("上一步") || t.contains("自动")) && b->isVisible()) {
            ++visibleAlgoButtons;
            printf("    still visible: '%s'\n", t.toUtf8().constData());
        }
    }
    check(visibleAlgoButtons == 0, "no algorithm build/playback buttons remain visible");
    check(panel && panel->isVisible(), "the cartesian object list panel is visible");

    printf("--- issue 3: the alignment grid is off in cartesian mode ---\n");
    const SceneGrid::Config cfg = SceneGrid::currentConfig();
    printf("  SceneGrid enabled=%d showGuides=%d\n", cfg.enabled ? 1 : 0, cfg.showGuides ? 1 : 0);
    check(!cfg.enabled, "SceneGrid is disabled while in cartesian mode");
    check(!cfg.showGuides, "SceneGrid guides are hidden while in cartesian mode");

    printf("--- issue 4: functions actually draw ---\n");
    MyTextInput* fnInput = panel->findChild<MyTextInput*>("cartesianFunctionInput");
    MyButton* addButton = panel->findChild<MyButton*>("cartesianFunctionAdd");
    check(fnInput != nullptr, "function input present");
    check(addButton != nullptr, "function draw button present");

    fnInput->setText("3x^2 - 8x + 7");
    pump(60);
    addButton->click();
    pump(300);
    check(engine && engine->curveCount() == 1, "the curve was created");

    if (engine && view && engine->curveCount() > 0) {
        CartesianCurveItem* curve = engine->curveAt(0);
        check(curve && curve->visibleRunCount() > 0, "the curve has a drawn polyline");
        const QPointF tl = view->mapToSceneF(QPointF(0, 0));
        const QPointF br = view->mapToSceneF(QPointF(view->viewport()->width(),
                                                      view->viewport()->height()));
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
        printf("  visible curve pixels = %d\n", curvePixels);
        check(curvePixels > 300, "the curve is visible on the canvas");
    }

    printf("--- issue 4b: the f(x) = prefix is accepted ---\n");
    if (engine) {
        const int before = engine->curveCount();
        const bool ok = engine->addFunction("g", "f(x) = x^2 + 1", QColor(52, 120, 214)) >= 0;
        check(ok, "an expression written as 'f(x) = ...' still parses");
        check(engine->curveCount() == before + 1, "the prefixed curve was added");
    }

    printf("--- issue 5: guidance and shortcuts ---\n");
    check(toolbar && toolbar->cartesianButtonFor(static_cast<int>(ToolIconButton::CartLine)) != nullptr,
          "the line tool button exists");
    check(toolbar && toolbar->cartesianButtonFor(static_cast<int>(ToolIconButton::CartFunction)) != nullptr,
          "the function tool button exists");
    bool hasGuide = false;
    for (QWidget* widget : panel->findChildren<QWidget*>()) {
        if (widget->objectName() != "cartesianGuideCard") continue;
        QString all;
        for (QLabel* label : widget->findChildren<QLabel*>()) all += label->text();
        printf("  guide text = %s\n", qPrintable(all));
        hasGuide = all.contains("画点") && all.contains("连线") && all.contains("函数");
        break;
    }
    check(hasGuide, "the panel shows a step-by-step plotting guide");

    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_S, Qt::NoModifier);
    app.sendEvent(&w, &keyPress);
    pump(120);
    check(tc && tc->currentTool() == ToolController::CartLine, "S switches to the line tool");
    check(engine && engine->activeTool() == CartesianEngine::Line, "the engine follows into line mode");

    QKeyEvent keyPress2(QEvent::KeyPress, Qt::Key_P, Qt::NoModifier);
    app.sendEvent(&w, &keyPress2);
    pump(120);
    check(tc && tc->currentTool() == ToolController::CartPoint, "P switches back to the point tool");

    printf("\n%s (%d failures)\n", g_fail == 0 ? "ALL PASS" : "FAILED", g_fail);
    return g_fail == 0 ? 0 : 1;
}
