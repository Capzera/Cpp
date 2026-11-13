#include <QApplication>
#include <QTimer>
#include <QEventLoop>
#include "ui/MainWindow.h"
#include "ui/widgets/MyComboBox.h"
#include "ui/widgets/MyButton.h"
#include "ui/widgets/MyToggleSwitch.h"
#include "ui/widgets/MyCheckBox.h"

static void pump(int ms) {
    QEventLoop loop;
    QTimer::singleShot(ms, &loop, &QEventLoop::quit);
    loop.exec();
}

static void pickModule(MainWindow& w, const QString& keyword) {
    MyComboBox* combo = w.findChild<MyComboBox*>();
    for (int i = 1; i < combo->count(); ++i) {
        if (combo->itemText(i).contains(keyword)) { combo->setCurrentIndex(i); break; }
    }
    pump(250);
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
    w.grab().save("ui_start.png");

    pickModule(w, "广度优先");
    w.grab().save("ui_graph_panel.png");

    clickButton(w, "绘制可视化效果");
    pump(600);
    w.grab().save("ui_graph_built.png");

    MyToggleSwitch* toggle = w.findChild<MyToggleSwitch*>();
    if (toggle) {
        toggle->setChecked(true);
        pump(500);
        w.grab().save("ui_graph_undirected.png");
    }

    pickModule(w, "冒泡");
    w.grab().save("ui_sort.png");

    return 0;
}
