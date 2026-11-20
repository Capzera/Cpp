#include <QApplication>
#include <QTimer>
#include <QEventLoop>
#include <cstdio>
#include "ui/MainWindow.h"
#include "ui/widgets/MyComboBox.h"

static int g_fail = 0;

static void check(bool ok, const char* name) {
    printf("[%s] %s\n", ok ? "PASS" : "FAIL", name);
    if (!ok) g_fail++;
}

static void pump(int ms) {
    QEventLoop loop;
    QTimer::singleShot(ms, &loop, &QEventLoop::quit);
    loop.exec();
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    MainWindow w;
    w.resize(1400, 900);
    w.show();
    pump(600);

    MyComboBox* combo = w.findChild<MyComboBox*>();
    check(combo != nullptr, "combo exists");
    if (!combo) return 1;

    int mstGroupCount = 0;
    for (int i = 1; i < combo->count(); ++i) {
        if (combo->groupOf(i) == "图论 · 最小生成树") ++mstGroupCount;
    }
    printf("mst group size = %d\n", mstGroupCount);
    check(mstGroupCount == 2, "both MST modules sit in one group");

    bool hasKruskal = false;
    bool hasPrim = false;
    for (int i = 1; i < combo->count(); ++i) {
        const QString t = combo->itemText(i);
        if (t == "Kruskal 最小生成树") { hasKruskal = true; check(combo->groupOf(i) == "图论 · 最小生成树", "Kruskal in MST group"); }
        if (t == "Prim 最小生成树") { hasPrim = true; check(combo->groupOf(i) == "图论 · 最小生成树", "Prim in MST group"); }
    }
    check(hasKruskal, "Kruskal listed");
    check(hasPrim, "Prim listed");

    QStringList groups = combo->groupNames();
    int mstGroupIndex = groups.indexOf("图论 · 最小生成树");
    check(mstGroupIndex >= 0, "MST group present in group list");

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}
