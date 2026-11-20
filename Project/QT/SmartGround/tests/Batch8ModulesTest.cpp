#include <QApplication>
#include <QImage>
#include <QPainter>
#include <QGraphicsScene>
#include <cstdio>
#include <set>
#include "modules/ShellSortModule.h"
#include "modules/FastPowModule.h"
#include "modules/SieveOfEratosthenesModule.h"
#include "modules/ForwardStarModule.h"
#include "modules/DSUPathCompressionModule.h"
#include "modules/TreeSubtreeSizeModule.h"
#include "modules/TreeDiameterModule.h"
#include "modules/DijkstraHeapModule.h"

static int g_fail = 0;

static void check(const char* name, bool ok) {
    printf("[%s] %s\n", ok ? "PASS" : "FAIL", name);
    if (!ok) g_fail++;
}

static QImage shot(QGraphicsScene* scene, const QString& name) {
    QImage img(scene->sceneRect().size().toSize(), QImage::Format_ARGB32);
    img.fill(Qt::white);
    QPainter p(&img);
    scene->render(&p);
    img.save(name);
    return img;
}

static int nonWhite(const QImage& img) {
    int n = 0;
    for (int y = 0; y < img.height(); ++y)
        for (int x = 0; x < img.width(); ++x)
            if (img.pixelColor(x, y).lightness() < 250) n++;
    return n;
}

int main(int argc, char** argv) {
    setvbuf(stdout, nullptr, _IONBF, 0);
    QApplication app(argc, argv);
    printf("=== start ===\n");

    {
        printf("--- shell sort ---\n");
        QGraphicsScene scene;
        ShellSortModule m;
        m.buildLayout(&scene, "8\n9 8 7 6 5 4 3 2");
        auto frames = m.runAlgorithm();
        check("shell: frames generated", frames.size() > 20);
        int gapFrames = 0, groupFrames = 0, insertFrames = 0;
        for (auto& f : frames) {
            if (f.type == ActionType::SH_GAP) ++gapFrames;
            if (f.type == ActionType::SH_GROUP) ++groupFrames;
            if (f.type == ActionType::SH_INSERT) ++insertFrames;
        }
        check("shell: has SH_GAP frames", gapFrames >= 3);
        check("shell: has SH_GROUP frames", groupFrames >= 4);
        check("shell: has SH_INSERT frames", insertFrames >= 7);
        int lastGap = 0;
        for (auto& f : frames) if (f.type == ActionType::SH_GAP) lastGap = f.val1;
        check("shell: final gap is 1", lastGap == 1);
        for (auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "shell_sort.png");
        check("shell: scene rendered", nonWhite(img) > 300);
    }

    {
        printf("--- fast pow ---\n");
        QGraphicsScene scene;
        FastPowModule m;
        m.buildLayout(&scene, "3 13 1000000007");
        auto frames = m.runAlgorithm();
        check("fastpow: frames generated", frames.size() >= 6);
        long long modResult = -1;
        for (auto& f : frames) if (f.type == ActionType::FP_RESULT && f.v == 1) modResult = f.big1;
        check("fastpow: 3^13 mod 1e9+7 = 1594323", modResult == 1594323);
        int bitFrames = 0;
        for (auto& f : frames) {
            if (f.type == ActionType::FP_BIT) ++bitFrames;
        }
        check("fastpow: one bit frame per binary digit", bitFrames == 4);
        check("fastpow: frame count is compact", frames.size() <= 7);
        for (auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "fast_pow.png");
        check("fastpow: scene rendered", nonWhite(img) > 300);

        QGraphicsScene scene2;
        FastPowModule m2;
        m2.buildLayout(&scene2, "2 20");
        auto frames2 = m2.runAlgorithm();
        long long exact = -1;
        for (auto& f : frames2) if (f.type == ActionType::FP_RESULT && f.v == 1) exact = f.big1;
        check("fastpow: exact 2^20 = 1048576", exact == 1048576);
        long long maxBase = 0;
        for (auto& f : frames2) if (f.type == ActionType::FP_BIT) maxBase = std::max(maxBase, f.big2);
        check("fastpow: exact base does not overflow int", maxBase == 4294967296LL);
        for (auto& f : frames2) m2.renderFrame(f);
        QImage img2 = shot(&scene2, "fast_pow_exact.png");
        check("fastpow: exact scene rendered", nonWhite(img2) > 300);

        QGraphicsScene scene3;
        FastPowModule m3;
        m3.buildLayout(&scene3, "1000000 40");
        auto frames3 = m3.runAlgorithm();
        check("fastpow: overflowing exact power is rejected", frames3.empty());
    }

    {
        printf("--- sieve ---\n");
        QGraphicsScene scene;
        SieveOfEratosthenesModule m;
        m.buildLayout(&scene, "120");
        auto frames = m.runAlgorithm();
        check("sieve: frames generated", frames.size() > 20);
        int primeFrames = 0, crossFrames = 0, batchFrames = 0;
        std::set<int> primes;
        int batchTotal = 0;
        for (auto& f : frames) {
            if (f.type == ActionType::SV_PRIME) { ++primeFrames; primes.insert(f.u); }
            if (f.type == ActionType::SV_CROSS) ++crossFrames;
            if (f.type == ActionType::SV_CROSS_BATCH) { ++batchFrames; batchTotal += static_cast<int>(f.group.size()); }
        }
        check("sieve: found 30 primes up to 120", primes.size() == 30);
        check("sieve: 2 is prime", primes.count(2) == 1);
        check("sieve: 119 is not prime", primes.count(119) == 0);
        check("sieve: one batch frame per prime with multiples", batchFrames == 4);
        check("sieve: no per-number cross frames", crossFrames == 0);
        check("sieve: batch covers all multiples", batchTotal == 128);
        for (auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "sieve.png");
        check("sieve: scene rendered", nonWhite(img) > 800);
    }

    {
        printf("--- forward star ---\n");
        QGraphicsScene scene;
        ForwardStarModule m;
        m.buildLayout(&scene, "5 6 1\n1 2 3\n1 3 5\n2 4 2\n3 4 1\n4 5 4\n2 5 6");
        auto frames = m.runAlgorithm();
        check("forwardstar: frames generated", frames.size() > 20);
        int addFrames = 0, linkFrames = 0, headFrames = 0, traverseFrames = 0;
        for (auto& f : frames) {
            if (f.type == ActionType::FS_ADD) ++addFrames;
            if (f.type == ActionType::FS_LINK) ++linkFrames;
            if (f.type == ActionType::FS_HEAD) ++headFrames;
            if (f.type == ActionType::FS_TRAVERSE) ++traverseFrames;
        }
        check("forwardstar: has FS_ADD frames", addFrames >= 6);
        check("forwardstar: has FS_LINK frames", linkFrames >= 6);
        check("forwardstar: has FS_HEAD frames", headFrames >= 6);
        check("forwardstar: has FS_TRAVERSE frames", traverseFrames >= 5);
        for (auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "forward_star.png");
        check("forwardstar: scene rendered", nonWhite(img) > 500);
    }

    {
        printf("--- dsu path compression ---\n");
        QGraphicsScene scene;
        DSUPathCompressionModule m;
        m.buildLayout(&scene, "8\n0 1 2 0 4 0 5 6");
        auto frames = m.runAlgorithm();
        check("dsupath: frames generated", frames.size() > 20);
        int findFrames = 0, compressFrames = 0;
        for (auto& f : frames) {
            if (f.type == ActionType::DP_FIND) ++findFrames;
            if (f.type == ActionType::DP_COMPRESS) ++compressFrames;
        }
        check("dsupath: has DP_FIND frames", findFrames >= 8);
        check("dsupath: has DP_COMPRESS frames", compressFrames >= 8);
        bool hasDone = false;
        for (auto& f : frames) if (f.type == ActionType::DP_DONE) hasDone = true;
        check("dsupath: has DP_DONE", hasDone);
        for (auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "dsu_path.png");
        check("dsupath: scene rendered", nonWhite(img) > 500);
    }

    {
        printf("--- subtree size ---\n");
        QGraphicsScene scene;
        TreeSubtreeSizeModule m;
        m.buildLayout(&scene, "1 2 3\n2 4 5\n3 6 7\n6 8");
        auto frames = m.runAlgorithm();
        check("subtree: frames generated", frames.size() > 20);
        std::map<int, int> finalSz;
        for (auto& f : frames) if (f.type == ActionType::TSZ_COMPUTE) finalSz[f.u] = f.val1;
        check("subtree: sz[8] = 1", finalSz[8] == 1);
        check("subtree: sz[6] = 2", finalSz[6] == 2);
        check("subtree: sz[4] = 1", finalSz[4] == 1);
        check("subtree: sz[3] = 4", finalSz[3] == 4);
        check("subtree: sz[2] = 3", finalSz[2] == 3);
        check("subtree: sz[1] = 8", finalSz[1] == 8);
        for (auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "subtree_size.png");
        check("subtree: scene rendered", nonWhite(img) > 500);
    }

    {
        printf("--- tree diameter ---\n");
        QGraphicsScene scene;
        TreeDiameterModule m;
        m.buildLayout(&scene, "1 2 3\n2 4 5\n3 6\n4 7");
        auto frames = m.runAlgorithm();
        check("diameter: frames generated", frames.size() > 15);
        int length = -1;
        int endA = 0, endB = 0;
        for (auto& f : frames) {
            if (f.type == ActionType::TD_DONE) { length = f.val1; endA = f.u; endB = f.v; }
        }
        check("diameter: length = 5", length == 5);
        check("diameter: endpoints 7 and 6", (endA == 7 && endB == 6) || (endA == 6 && endB == 7));
        for (auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "tree_diameter.png");
        check("diameter: scene rendered", nonWhite(img) > 500);
    }

    {
        printf("--- dijkstra heap ---\n");
        QGraphicsScene scene;
        DijkstraHeapModule m;
        m.buildLayout(&scene, "6 9 1\n1 2 7\n1 3 9\n1 6 14\n2 3 10\n2 4 15\n3 4 11\n3 6 2\n4 5 6\n5 6 9");
        auto frames = m.runAlgorithm();
        check("dijheap: frames generated", frames.size() > 20);
        bool hasSkip = false;
        int pushFrames = 0, popFrames = 0;
        for (auto& f : frames) {
            if (f.type == ActionType::DH_SKIP) hasSkip = true;
            if (f.type == ActionType::DH_PUSH) ++pushFrames;
            if (f.type == ActionType::DH_POP) ++popFrames;
        }
        check("dijheap: has DH_SKIP (lazy delete)", hasSkip);
        check("dijheap: pq push frames recorded", pushFrames >= 5);
        check("dijheap: pq pop frames recorded", popFrames >= 5);
        for (auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "dijkstra_heap.png");
        check("dijheap: scene rendered", nonWhite(img) > 800);
    }

    {
        printf("--- invalid inputs ---\n");
        QGraphicsScene scene;
        ShellSortModule s;
        s.buildLayout(&scene, "");
        check("shell: invalid input handled", s.runAlgorithm().empty());
        FastPowModule f;
        f.buildLayout(&scene, "");
        check("fastpow: invalid input handled", f.runAlgorithm().empty());
        SieveOfEratosthenesModule v;
        v.buildLayout(&scene, "abc");
        check("sieve: invalid input handled", v.runAlgorithm().empty());
        ForwardStarModule w;
        w.buildLayout(&scene, "garbage");
        check("forwardstar: invalid input handled", w.runAlgorithm().empty());
        DSUPathCompressionModule d;
        d.buildLayout(&scene, "");
        check("dsupath: invalid input handled", d.runAlgorithm().empty());
        TreeSubtreeSizeModule t;
        t.buildLayout(&scene, "");
        check("subtree: invalid input handled", t.runAlgorithm().empty());
        TreeDiameterModule td;
        td.buildLayout(&scene, "");
        check("diameter: invalid input handled", td.runAlgorithm().empty());
        DijkstraHeapModule dh;
        dh.buildLayout(&scene, "");
        check("dijheap: invalid input handled", dh.runAlgorithm().empty());
    }

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}
