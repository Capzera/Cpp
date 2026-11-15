#include <QApplication>
#include <QTimer>
#include <QEventLoop>
#include <QScreen>
#include <cstdio>
#include "ui/MainWindow.h"
#include "ui/widgets/MyComboBox.h"
#include "ui/widgets/CanvasToolBar.h"
#include "ui/widgets/ToolIconButton.h"
#include "canvas/CanvasView.h"

static void pump(int ms) {
    QEventLoop loop;
    QTimer::singleShot(ms, &loop, &QEventLoop::quit);
    loop.exec();
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    MainWindow w;
    w.resize(1200, 800);
    w.show();
    pump(400);

    CanvasToolBar* suite = w.findChild<CanvasToolBar*>("canvasToolBarHost");
    printf("tool suite found: %s\n", suite ? "yes" : "no");
    if (suite) {
        printf("suite main bar width = %d\n", suite->mainBarWidth());
        printf("suite geometry = %d,%d %dx%d\n", suite->x(), suite->y(),
               suite->width(), suite->height());
    }

    const QRect avail = QApplication::primaryScreen()->availableGeometry();
    printf("screen available = %dx%d\n", avail.width(), avail.height());

    const int winW = w.width();
    const int winH = w.height();
    printf("window = %dx%d\n", winW, winH);
    printf("sizeHint = %dx%d\n", w.sizeHint().width(), w.sizeHint().height());
    printf("minimumSizeHint = %dx%d\n", w.minimumSizeHint().width(), w.minimumSizeHint().height());

    w.grab().save("app_window.png");

    if (suite) {
        ToolIconButton* moreBtn = suite->findChild<ToolIconButton*>("canvasMoreToggle");
        if (moreBtn) {
            moreBtn->click();
            pump(150);
            printf("more panel open: %s\n", suite->isMorePanelOpen() ? "yes" : "no");
            w.grab().save("app_more_open.png");
            moreBtn->click();
            pump(150);
        }
        ToolIconButton* penBtn = suite->findChild<ToolIconButton*>("canvasTool_2");
        if (penBtn) {
            penBtn->click();
            pump(120);
            penBtn->click();
            pump(150);
            printf("pen tool panel open: %s\n", suite->isToolPanelOpen() ? "yes" : "no");
            w.grab().save("app_pen_settings.png");
            penBtn->click();
            pump(120);
        }
    }

    int failures = 0;
    const int targetScreen = 1920;
    if (w.sizeHint().width() > targetScreen) {
        printf("FAIL: natural window width %d exceeds %d\n", w.sizeHint().width(), targetScreen);
        failures++;
    } else {
        printf("PASS: natural window width %d fits a %d screen\n", w.sizeHint().width(), targetScreen);
    }
    if (w.minimumSizeHint().width() > targetScreen) {
        printf("FAIL: minimum window width %d exceeds %d\n", w.minimumSizeHint().width(), targetScreen);
        failures++;
    } else {
        printf("PASS: minimum window width %d fits a %d screen\n", w.minimumSizeHint().width(), targetScreen);
    }
    if (suite && suite->mainBarWidth() > 700) { printf("FAIL: tool suite too wide\n"); failures++; }
    else printf("PASS: tool suite compact\n");
    CanvasView* view = w.findChild<CanvasView*>("canvasView");
    if (suite && view && suite->parent() == view) {
        printf("PASS: tool suite floats over the canvas view\n");
    } else {
        printf("FAIL: tool suite is not floating over the canvas\n");
        failures++;
    }

    printf("\n%d failures\n", failures);
    return failures == 0 ? 0 : 1;
}
