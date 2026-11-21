#include <QApplication>
#include <QGraphicsScene>
#include <QTimer>
#include <QDebug>
#include "modules/BFSModule.h"
#include "modules/TopologicalSortModule.h"
#include "modules/DijkstraNaiveModule.h"
#include "modules/KruskalModule.h"
#include "modules/TarjanModule.h"
#include "modules/DSUPathCompressionModule.h"
#include "modules/HeapBasicsModule.h"
#include "modules/StackModule.h"
#include "modules/QueueModule.h"
#include "modules/PrefixSumModule.h"
#include "modules/DifferenceModule.h"
#include "modules/BubbleSortModule.h"
#include "modules/MonotonicStackModule.h"
#include "modules/KMPNextModule.h"
#include "modules/LISModule.h"
#include "modules/Knapsack01Module.h"

static void stress(IModule* m, const QString& label, const QStringList& inputs) {
    QGraphicsScene scene;
    for (const QString& in : inputs) {
        for (int directed = 0; directed < 2; ++directed) {
            GraphModuleUtils::GraphOptions opt;
            opt.directed = directed == 1;
            opt.bidirectional = directed == 0;
            m->setGraphOptions(opt);
            scene.clear();
            m->m_animated = false;
            m->buildLayout(&scene, in);
            auto frames = m->runAlgorithm();
            for (const auto& f : frames) m->renderFrame(f);
        }
    }
    printf("OK %s\n", qPrintable(label));
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QGraphicsScene scene;

    stress(new BFSModule(), "BFS", {
        "7 8 1\n1 2\n1 3\n2 4\n2 5\n3 5\n3 6\n5 7\n6 7",
        "7 8 1\n1 2 3\n1 3 5\n2 4 2\n2 5 4\n3 5 1\n3 6 6\n5 7 3\n6 7 2",
        "1 0 1\n",
        "0 0\n",
        "3 2 1\n1 2\n1 3\n",
        "20 40 5\n1 2\n2 3\n3 4\n4 5\n5 6\n6 7\n7 8\n8 9\n9 10\n10 11\n11 12\n12 13\n13 14\n14 15\n15 16\n16 17\n17 18\n18 19\n19 20\n20 1\n2 3\n4 5\n6 7\n8 9\n10 11\n12 13\n14 15\n16 17\n18 19\n2 19\n"
    });

    stress(new TopologicalSortModule(), "Topo", {
        "8 9\n1 3\n1 4\n2 4\n2 5\n3 6\n4 6\n4 7\n5 7\n6 8",
        "5 6\n1 2\n2 3\n3 1\n4 5\n5 4\n3 4",
        "1 0\n"
    });

    stress(new DijkstraNaiveModule(), "Dijkstra", {
        "6 9 1\n1 2 7\n1 3 9\n1 6 14\n2 3 10\n2 4 15\n3 4 11\n3 6 2\n4 5 6\n5 6 9",
        "5 4 1\n1 2 3\n2 3 4\n3 4 5\n4 5 6",
        "5 3 1\n1 2 -1\n2 3 4\n3 4 5",
        "4 2 1\n1 2 5\n1 3 5"
    });

    stress(new KruskalModule(), "Kruskal", {
        "6 9\n1 2 4\n1 3 3\n2 3 2\n2 4 5\n3 4 3\n3 5 6\n4 5 1\n4 6 7\n5 6 8",
        "4 2\n1 2 1\n3 4 2",
        "3 1\n1 2 5"
    });

    stress(new TarjanModule(), "Tarjan", {
        "6 8\n1 2\n2 3\n3 1\n3 4\n4 5\n5 6\n6 4\n2 6",
        "3 3\n1 2\n2 3\n3 1"
    });

    stress(new DSUPathCompressionModule(), "DSU", {"8\n1 2\n2 3\n3 4\n6 7", "2\n1 2", "3\n1 2\n2 3"});
    stress(new HeapBasicsModule(), "Heap", {"5\n10 5 8 2 9", "1\n42", "8\n1 2 3 4 5 6 7 8"});
    stress(new StackModule(), "Stack", {"8\nPUSH 10\nPUSH 20\nPOP\nPUSH 40", "2\nPUSH 1\nPUSH 2\nPUSH 3", "1\nPOP"});
    stress(new QueueModule(), "Queue", {"8\nPUSH 10\nPUSH 20\nPOP\nPUSH 40", "2\nPUSH 1\nPUSH 2\nPUSH 3", "1\nPOP"});
    stress(new PrefixSumModule(), "PrefixSum", {"5 2\n3 1 2 5 4\n2 4\n1 5", "5 2\n3 1 2 5 4\n0 99\n-3 4"});
    stress(new DifferenceModule(), "Difference", {"5 2\n1 1 1 1 1\n2 4 10\n1 3 5", "5 1\n1 1 1 1 1\n0 99 3"});
    stress(new BubbleSortModule(), "Bubble", {"8\n3 5 1 9 2 7 4 6", "1\n42", "0\n"});
    stress(new MonotonicStackModule(), "MonoStack", {"7\n3 5 1 9 2 8 4", "1\n5", "0\n"});
    stress(new KMPNextModule(), "KMP", {"ABABCABAA", "A", "AAAAAAAAAAAAAAAAAAAA"});
    stress(new LISModule(), "LIS", {"8\n10 9 2 5 3 7 101 18", "1\n5"});
    stress(new Knapsack01Module(), "Knapsack", {"4 8\n2 3\n3 4\n4 5\n5 8", "1 0\n0 0"});

    printf("ALL STRESS PASSED\n");
    return 0;
}
