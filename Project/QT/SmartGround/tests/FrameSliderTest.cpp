#include <QApplication>
#include <QTimer>
#include <QEventLoop>
#include "ui/MainWindow.h"
#include "ui/widgets/MyComboBox.h"
#include "ui/widgets/MyButton.h"
#include "ui/widgets/MySlider.h"
#include "modules/QuickSortModule.h"

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

static MySlider* findSlider(MainWindow& w) {
    return w.findChild<MySlider*>();
}

static MyComboBox* findCombo(MainWindow& w) {
    return w.findChild<MyComboBox*>();
}

static void clickButton(MainWindow& w, const QString& text) {
    for (MyButton* b : w.findChildren<MyButton*>()) {
        if (b->text() == text) { b->click(); return; }
    }
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    MainWindow w;
    w.resize(1400, 860);
    w.show();
    pump(400);

    MySlider* slider = findSlider(w);
    check("slider: exists in main window", slider != nullptr);
    if (!slider) {
        printf("\n%s  fail=%d\n", "HAS FAILURE", g_fail);
        return 1;
    }
    check("slider: disabled before frames", !slider->isEnabled());
    check("slider: compact width", slider->width() <= 260);
    w.grab().save("ui_slider_start.png");

    MyComboBox* combo = findCombo(w);
    for (int i = 1; i < combo->count(); ++i) {
        if (combo->itemText(i).contains("快速排序")) { combo->setCurrentIndex(i); break; }
    }
    pump(300);

    clickButton(w, "绘制可视化效果");
    pump(700);

    check("slider: enabled after frames", slider->isEnabled());
    check("slider: range matches frame count", slider->maximum() > 20);

    int total = slider->maximum();
    slider->setValue(total);
    pump(300);
    check("slider: can seek to last frame", slider->value() == total);
    w.grab().save("ui_slider_end.png");

    slider->setValue(total / 2);
    pump(300);
    check("slider: can seek to middle frame", slider->value() == total / 2);
    w.grab().save("ui_slider_mid.png");

    slider->setValue(0);
    pump(200);
    check("slider: can seek back to start", slider->value() == 0);

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}
