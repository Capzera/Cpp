#include <QApplication>
#include <QPixmap>
#include <cstdio>
#include "ui/MainWindow.h"

static int g_fail = 0;

static void check(const char* name, bool ok) {
    printf("[%s] %s\n", ok ? "PASS" : "FAIL", name);
    if (!ok) g_fail++;
}

class GroupAProbe : public MainWindow {
public:
    using MainWindow::MainWindow;
};

int main(int argc, char** argv) {
    setvbuf(stdout, nullptr, _IONBF, 0);
    QApplication app(argc, argv);
    GroupAProbe w;
    w.resize(1200, 800);
    w.show();
    app.processEvents();

    QStringList names = w.probeModuleNames();
    printf("module count = %d\n", names.size());
    check("module count == 38", names.size() == 38);
    check("has fenwick tree", names.contains("树状数组 BIT"));
    check("has complete knapsack", names.contains("完全背包"));
    check("has tarjan cut vertex", names.contains("Tarjan 割点与桥"));
    check("has hungarian", names.contains("匈牙利算法"));
    check("has heap adjust", names.contains("堆调整 up/down"));
    check("has heap sort", names.contains("堆排序"));

    const QStringList targets = {"树状数组 BIT", "完全背包", "Tarjan 割点与桥",
                                 "匈牙利算法", "堆调整 up/down", "堆排序"};
    for (const QString& target : targets) {
        w.probeSelectModule(target);
        app.processEvents();
        check(qPrintable(QString("selectable: %1").arg(target)), names.contains(target));
    }

    w.probeSelectModule("树状数组 BIT");
    app.processEvents();
    w.grab().save("group_a_ui.png");

    printf("=== done, failures: %d ===\n", g_fail);
    return g_fail == 0 ? 0 : 1;
}
