#include <QApplication>
#include <QTimer>
#include <QEventLoop>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <cstdio>
#include "ui/widgets/CanvasToolBar.h"
#include "ui/widgets/ToolIconButton.h"
#include "canvas/CanvasView.h"
#include "canvas/ToolController.h"
#include "canvas/AnnotationScene.h"

static int g_fail = 0;

static void check(const char* name, bool ok) {
    printf("[%s] %s\n", ok ? "PASS" : "FAIL", name);
    if (!ok) g_fail++;
}

static void pump(int ms) {
    QEventLoop loop;
    QTimer::singleShot(ms, &loop, &QEventLoop::quit);
    loop.exec();
}

static void clickWidget(QWidget* w) {
    QPointF g = w->mapToGlobal(QPoint(w->width() / 2, w->height() / 2));
    QMouseEvent press(QEvent::MouseButtonPress, g, g, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(w, &press);
    QMouseEvent release(QEvent::MouseButtonRelease, g, g, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(w, &release);
    QApplication::processEvents();
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QWidget host;
    host.resize(1200, 800);
    host.show();

    AnnotationScene scene;
    CanvasView view(&scene);
    view.resize(1200, 800);
    view.show();

    ToolController controller(&scene, &view);
    CanvasToolBar bar(&controller, &host);
    bar.setHostWidget(&host);
    bar.resize(600, 60);
    bar.move(300, 40);
    pump(200);

    controller.setTool(ToolController::Eraser);
    pump(120);

    ToolIconButton* eraserButton = bar.toolButtonFor(static_cast<int>(ToolController::Eraser));
    check("eraser button found", eraserButton != nullptr);
    if (eraserButton) {
        clickWidget(eraserButton);
        pump(220);
    }

    QWidget* panel = bar.toolPanel();
    check("tool panel visible", panel && panel->isVisible());
    int panelH = 0;
    if (panel) {
        panelH = panel->height();
        printf("eraser panel height = %d\n", panelH);
        check("eraser panel is compact (< 160px)", panelH < 160);
        panel->grab().save("eraser_panel.png");
    }

    controller.setTool(ToolController::Pen);
    pump(150);
    if (panel) {
        const int penH = panel->height();
        printf("pen panel height = %d\n", penH);
        check("pen panel taller than eraser panel", penH > panelH);
    }

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}
