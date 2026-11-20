#include <QApplication>
#include <QTimer>
#include <QEventLoop>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QImage>
#include <cstdio>
#include "ui/MainWindow.h"
#include "ui/widgets/MyComboBox.h"
#include "ui/widgets/MySegmentedControl.h"

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
    const QPointF g(w->mapToGlobal(QPoint(w->width() / 2, w->height() / 2)));
    QMouseEvent press(QEvent::MouseButtonPress, g, g, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(w, &press);
    QMouseEvent release(QEvent::MouseButtonRelease, g, g, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(w, &release);
    QApplication::processEvents();
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    MainWindow w;
    w.resize(1280, 860);
    w.show();
    pump(600);

    MySegmentedControl* picker = w.findChild<MySegmentedControl*>("comboStylePicker");
    MyComboBox* combo = w.findChild<MyComboBox*>();
    check("module combo exists", combo != nullptr);
    if (!combo) return 1;

    check("style picker removed from sidebar", picker == nullptr);
    check("only one visual style offered", MyComboBox::styleNames().size() == 1);
    check("style name is Vue", MyComboBox::styleName(ComboVisualStyle::VueLight) == "Vue 扁平");
    check("default style is VueLight",
          combo->visualStyle() == ComboVisualStyle::VueLight);

    clickWidget(combo);
    pump(300);
    QWidget* popup = nullptr;
    for (QWidget* top : QApplication::topLevelWidgets()) {
        if (top->isWindow() && top->windowType() == Qt::Popup && top != combo) { popup = top; break; }
    }
    check("popup opens", popup != nullptr);
    if (popup) {
        popup->grab().save("app_combo_popup.png");
        popup->close();
    }
    pump(150);

    w.grab().save("app_style_picker.png");

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}
