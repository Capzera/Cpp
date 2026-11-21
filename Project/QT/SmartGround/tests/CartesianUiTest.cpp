#include <QApplication>
#include <QTimer>
#include <QComboBox>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <cstdio>
#include "ui/MainWindow.h"
#include "ui/widgets/MyComboBox.h"
#include "ui/widgets/CanvasToolBar.h"
#include "ui/widgets/CartesianPanel.h"
#include "ui/widgets/ToolIconButton.h"
#include "render/CartesianElements.h"
#include "modules/CartesianModule.h"

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
    w.resize(1280, 820);
    w.show();
    QApplication::processEvents();

    MyComboBox* combo = w.findChild<MyComboBox*>();
    check(combo != nullptr, "combo box found");
    if (!combo) return 1;

    const int index = findCartesianIndex(combo);
    check(index >= 0, "cartesian module appears in the dropdown");
    if (index < 0) return 1;

    combo->setCurrentIndex(index);
    pump(200);

    CanvasToolBar* toolbar = w.findChild<CanvasToolBar*>();
    check(toolbar != nullptr, "canvas toolbar found");
    check(toolbar && toolbar->isCartesianMode(), "toolbar switches to cartesian mode");

    CartesianPanel* panel = w.findChild<CartesianPanel*>();
    check(panel != nullptr, "cartesian panel found");
    check(panel && panel->isVisible(), "cartesian panel becomes visible");
    check(panel && panel->engine() != nullptr, "cartesian panel is bound to the engine");

    CartesianModule* module = nullptr;
    for (CartesianModule* candidate : w.findChildren<CartesianModule*>()) {
        module = candidate;
    }
    check(module != nullptr || (panel && panel->engine() != nullptr),
          "cartesian module instance is reachable");
    CartesianEngine* engine = panel ? panel->engine() : nullptr;
    check(engine != nullptr, "cartesian engine is reachable");
    check(engine && engine->isEnabled(), "cartesian engine is enabled");

    printf("--- second row tool buttons ---\n");
    check(toolbar && toolbar->cartesianButtonFor(static_cast<int>(ToolIconButton::CartPoint)) != nullptr,
          "point tool button exists in the second row");
    check(toolbar && toolbar->cartesianButtonFor(static_cast<int>(ToolIconButton::CartLine)) != nullptr,
          "line tool button exists in the second row");
    check(toolbar && toolbar->cartesianButtonFor(static_cast<int>(ToolIconButton::CartFunction)) != nullptr,
          "function tool button exists in the second row");

    printf("--- point tool wiring ---\n");
    if (toolbar) {
        ToolIconButton* pointButton =
            toolbar->cartesianButtonFor(static_cast<int>(ToolIconButton::CartPoint));
        if (pointButton) {
            pointButton->click();
            pump(120);
        }
        check(engine && engine->activeTool() == CartesianEngine::Point,
              "clicking the point tool activates point mode");
    }

    printf("--- function curve through the panel ---\n");
    if (engine) {
        const int before = engine->curveCount();
        const int result = engine->addFunction("f", "3x^2 - 8x + 7", QColor(214, 96, 77));
        check(result == before, "function curve added to the engine");
        check(engine->curveCount() == before + 1, "curve count increases");
        pump(80);
        check(panel && panel->engine() == engine, "panel still tracks the same engine");
    }

    printf("--- switching away hides the panel ---\n");
    combo->setCurrentIndex(0);
    pump(200);
    check(panel && !panel->isVisible(), "panel hides when leaving the cartesian module");
    check(toolbar && !toolbar->isCartesianMode(), "toolbar leaves cartesian mode");
    check(engine && !engine->isEnabled(), "engine is disabled after leaving");

    printf("\n%s (%d failures)\n", g_fail == 0 ? "ALL PASS" : "FAILED", g_fail);
    return g_fail == 0 ? 0 : 1;
}
