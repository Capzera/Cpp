#include <QApplication>
#include <QGraphicsScene>
#include <QTimer>
#include <QEventLoop>
#include <QDebug>
#include <cstdio>
#include "modules/BFSModule.h"
#include "modules/TopologicalSortModule.h"
#include "modules/DijkstraNaiveModule.h"
#include "modules/KruskalModule.h"
#include "modules/TarjanModule.h"
#include "modules/DSUBasicModule.h"
#include "modules/HeapBasicsModule.h"
#include "modules/StackModule.h"
#include "modules/QueueModule.h"
#include "modules/PrefixSumModule.h"
#include "modules/DifferenceModule.h"
#include "modules/BubbleSortModule.h"
#include "modules/SelectionSortModule.h"
#include "modules/InsertionSortModule.h"
#include "modules/QuickSortModule.h"
#include "modules/MergeSortModule.h"
#include "modules/MonotonicStackModule.h"
#include "modules/KMPNextModule.h"
#include "modules/LISModule.h"
#include "modules/Knapsack01Module.h"

static void spin(int ms) {
    QEventLoop loop;
    QTimer::singleShot(ms, &loop, &QEventLoop::quit);
    loop.exec();
}

static void animStress(IModule* m, const QString& label, const QString& input) {
    QGraphicsScene scene;
    m->m_animated = true;
    m->buildLayout(&scene, input);
    auto frames = m->runAlgorithm();
    for (size_t i = 0; i < frames.size(); ++i) {
        m->renderFrame(frames[i]);
        if (i % 3 == 0) {
            scene.clear();
            m->m_animated = false;
            m->buildLayout(&scene, input);
            m->m_animated = true;
        }
    }
    scene.clear();
    m->m_animated = false;
    m->buildLayout(&scene, input);
    auto frames2 = m->runAlgorithm();
    for (const auto& f : frames2) m->renderFrame(f);
    spin(700);
    scene.clear();
    spin(700);
    printf("ANIM OK %s\n", qPrintable(label));
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    animStress(new BubbleSortModule(), "Bubble", "8\n3 5 1 9 2 7 4 6");
    animStress(new SelectionSortModule(), "Selection", "8\n3 5 1 9 2 7 4 6");
    animStress(new InsertionSortModule(), "Insertion", "8\n3 5 1 9 2 7 4 6");
    animStress(new QuickSortModule(), "Quick", "8\n3 5 1 9 2 7 4 6");
    animStress(new MergeSortModule(), "Merge", "8\n3 5 1 9 2 7 4 6");
    animStress(new MonotonicStackModule(), "MonoStack", "7\n3 5 1 9 2 8 4");
    animStress(new PrefixSumModule(), "PrefixSum", "5 2\n3 1 2 5 4\n2 4\n1 5");
    animStress(new DifferenceModule(), "Difference", "5 2\n1 1 1 1 1\n2 4 10\n1 3 5");
    animStress(new BFSModule(), "BFS", "7 8 1\n1 2 3\n1 3 5\n2 4 2\n2 5 4\n3 5 1\n3 6 6\n5 7 3\n6 7 2");
    animStress(new TopologicalSortModule(), "Topo", "8 9\n1 3\n1 4\n2 4\n2 5\n3 6\n4 6\n4 7\n5 7\n6 8");
    animStress(new DijkstraNaiveModule(), "Dijkstra", "6 9 1\n1 2 7\n1 3 9\n1 6 14\n2 3 10\n2 4 15\n3 4 11\n3 6 2\n4 5 6\n5 6 9");
    animStress(new KruskalModule(), "Kruskal", "6 9\n1 2 4\n1 3 3\n2 3 2\n2 4 5\n3 4 3\n3 5 6\n4 5 1\n4 6 7\n5 6 8");
    animStress(new TarjanModule(), "Tarjan", "6 8\n1 2\n2 3\n3 1\n3 4\n4 5\n5 6\n6 4\n2 6");
    animStress(new KMPNextModule(), "KMP", "ABABCABAA");
    animStress(new LISModule(), "LIS", "8\n10 9 2 5 3 7 101 18");
    animStress(new Knapsack01Module(), "Knapsack", "4 8\n2 3\n3 4\n4 5\n5 8");
    printf("ALL ANIM PASSED\n");
    return 0;
}
