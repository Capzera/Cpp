#include <QApplication>
#include <QEventLoop>
#include <QTimer>
#include <cstdio>
#include "ui/MainWindow.h"
#include "ui/widgets/MyButton.h"
#include "ui/widgets/MyComboBox.h"

static int failures = 0;
static void check(bool cond, const char* msg) {
    if (!cond) { printf("FAIL: %s\n", msg); failures++; }
    else printf("PASS: %s\n", msg);
}

static void pump(int ms) {
    QEventLoop loop;
    QTimer::singleShot(ms, &loop, &QEventLoop::quit);
    loop.exec();
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    MainWindow w;
    w.resize(1400, 860);
    w.show();
    pump(300);

    MyComboBox* combo = w.findChild<MyComboBox*>();
    for (int i = 1; i < combo->count(); ++i) {
        if (combo->itemText(i).contains("冒泡")) { combo->setCurrentIndex(i); break; }
    }
    pump(200);

    MyButton* buildBtn = nullptr;
    MyButton* autoBtn = nullptr;
    for (MyButton* b : w.findChildren<MyButton*>()) {
        if (b->text() == "绘制可视化效果") buildBtn = b;
        if (b->text() == "自动播放") autoBtn = b;
    }
    check(buildBtn && autoBtn, "buttons found");

    buildBtn->click();
    pump(400);

    autoBtn->click();
    pump(300);
    check(autoBtn->text() == "暂停", "auto button switches to pause state");

    QLabel* status = nullptr;
    for (QLabel* l : w.findChildren<QLabel*>()) {
        if (l->objectName() == "statusLabel") status = l;
    }
    check(status != nullptr, "status label found");

    for (int i = 0; i < 4000; ++i) {
        pump(120);
        if (autoBtn->text() == "自动播放") break;
    }
    check(autoBtn->text() == "自动播放", "auto button returns to idle after playback ends");
    if (status) printf("status: %s\n", status->text().toUtf8().constData());
    check(status && status->text().contains("播放完毕"), "status reports playback finished, not restart");

    printf("\n%d failures\n", failures);
    return failures == 0 ? 0 : 1;
}
