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
    check("module count == 63", names.size() == 63);
    check("has cartesian", names.contains("自由结构 - 笛卡尔坐标系"));
    check("has gcd", names.contains("GCD 辗转相除"));
    check("has dfs", names.contains("DFS 深度优先搜索"));
    check("has prim", names.contains("Prim 最小生成树"));
    check("has counting sort", names.contains("计数排序"));
    check("has bipartite", names.contains("二分图判定"));
    check("has euler", names.contains("欧拉路径"));
    check("has interval sched", names.contains("区间调度"));
    check("has trie", names.contains("Trie 字典树"));
    check("has hash table", names.contains("哈希表 (开放寻址)"));
    check("has fenwick tree", names.contains("树状数组 BIT"));
    check("has complete knapsack", names.contains("完全背包 DP"));
    check("has tarjan cut vertex", names.contains("Tarjan 割点与桥"));
    check("has hungarian", names.contains("二分图匹配 - 匈牙利算法"));
    check("has heap adjust", names.contains("堆调整 up / down"));
    check("has heap sort", names.contains("堆排序"));
    check("has shell sort", names.contains("希尔排序"));
    check("has fast pow", names.contains("快速幂"));
    check("has sieve", names.contains("埃氏筛"));
    check("has euler sieve", names.contains("欧拉筛"));
    check("has forward star", names.contains("链式前向星"));
    check("has dsu path compression", names.contains("并查集"));
    check("has subtree size", names.contains("子树大小统计"));
    check("has tree diameter", names.contains("树的直径"));
    check("has dijkstra heap", names.contains("Dijkstra 堆优化"));
    check("has bucket sort", names.contains("桶排序"));
    check("has radix sort", names.contains("基数排序"));
    check("has circular queue", names.contains("循环队列"));
    check("has monotonic queue", names.contains("单调队列"));
    check("has exgcd", names.contains("扩展欧几里得"));
    check("has huffman", names.contains("哈夫曼树"));
    check("has string hash", names.contains("字符串哈希"));
    check("has lcs", names.contains("最长公共子序列"));

    const QStringList targets = {"树状数组 BIT", "完全背包 DP", "Tarjan 割点与桥",
                                 "二分图匹配 - 匈牙利算法", "堆调整 up / down", "堆排序",
                                 "希尔排序", "快速幂", "埃氏筛", "链式前向星",
                                 "并查集", "子树大小统计", "树的直径",
                                 "Dijkstra 堆优化"};
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
