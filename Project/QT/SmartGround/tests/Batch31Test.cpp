#include <QApplication>
#include <QGraphicsScene>
#include <cstdio>
#include <vector>
#include <algorithm>
#include "modules/IModule.h"
#include "modules/MergeSortModule.h"
#include "modules/Knapsack01Module.h"
#include "modules/DSUPathCompressionModule.h"
#include "modules/EulerSieveModule.h"
#include "modules/SieveOfEratosthenesModule.h"
#include "render/GraphElements.h"

static int g_fail = 0;

static void check(bool ok, const QString& name) {
    printf("[%s] %s\n", ok ? "PASS" : "FAIL", qPrintable(name));
    if (!ok) g_fail++;
}

static int countType(const std::vector<AnimFrame>& frames, ActionType type) {
    int count = 0;
    for (const auto& f : frames) if (f.type == type) ++count;
    return count;
}

static bool hasType(const std::vector<AnimFrame>& frames, ActionType type) {
    return countType(frames, type) > 0;
}

int main(int argc, char** argv) {
    setvbuf(stdout, nullptr, _IONBF, 0);
    QApplication app(argc, argv);

    {
        printf("--- merge sort: dedicated frame family ---\n");
        MergeSortModule m;
        QGraphicsScene scene;
        m.buildLayout(&scene, m.getDefaultInput());
        auto frames = m.runAlgorithm();
        check(hasType(frames, ActionType::MS_SPLIT), "merge: has MS_SPLIT frames");
        check(hasType(frames, ActionType::MS_CMP), "merge: has MS_CMP frames");
        check(hasType(frames, ActionType::MS_PICK), "merge: has MS_PICK frames");
        check(hasType(frames, ActionType::MS_COPY), "merge: has MS_COPY frames");
        check(hasType(frames, ActionType::MS_DONE), "merge: has MS_DONE frame");
        check(countType(frames, ActionType::AR_SWAP) == 0, "merge: does not reuse quick-sort AR frames");
        check(countType(frames, ActionType::AR_SCAN_MIN) == 0, "merge: does not reuse AR_SCAN_MIN");

        m.m_animated = false;
        for (const auto& f : frames) {
            m.renderFrame(f);
            m.finalizeFrame();
        }
        std::vector<int> finalValues;
        for (const auto& kv : m.blocks()) {
            finalValues.push_back(kv.second->value);
        }
        std::sort(finalValues.begin(), finalValues.end());
        std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 9};
        check(finalValues == expected, "merge: final array is sorted");

        int mergeOpen = countType(frames, ActionType::MS_MERGE_OPEN);
        check(mergeOpen == 7, "merge: one merge step per internal node (8 elements -> 7 merges)");
    }

    {
        printf("--- knapsack: compare before landing ---\n");
        Knapsack01Module m;
        QGraphicsScene scene;
        m.buildLayout(&scene, m.getDefaultInput());
        auto frames = m.runAlgorithm();
        check(hasType(frames, ActionType::KN_COMPARE), "knapsack: has KN_COMPARE frames");
        int compares = countType(frames, ActionType::KN_COMPARE);
        int candidates = countType(frames, ActionType::KN_CANDIDATE);
        check(compares > 0 && compares < candidates, "knapsack: compare frames emitted per state");

        bool compareBeforeDecide = true;
        bool seenCompare = false;
        for (const auto& f : frames) {
            if (f.type == ActionType::KN_COMPARE) seenCompare = true;
            if (f.type == ActionType::KN_DECIDE && !seenCompare && f.val2 != 2) compareBeforeDecide = false;
        }
        check(compareBeforeDecide, "knapsack: every decision is preceded by a comparison");

        m.m_animated = false;
        for (const auto& f : frames) {
            m.renderFrame(f);
            m.finalizeFrame();
        }
    }

    {
        printf("--- dsu: unified module ---\n");
        DSUPathCompressionModule m;
        check(m.getModuleName() == QString("并查集"), "dsu: module is named 并查集");

        QGraphicsScene scene;
        m.buildLayout(&scene, m.getDefaultInput());
        auto frames = m.runAlgorithm();
        check(hasType(frames, ActionType::DSU_INIT), "dsu: has DSU_INIT frames");
        check(hasType(frames, ActionType::DSU_FIND_STEP), "dsu: has DSU_FIND_STEP frames");
        check(hasType(frames, ActionType::DSU_FIND_ROOT), "dsu: has DSU_FIND_ROOT frames");
        check(hasType(frames, ActionType::DSU_COMPRESS_STEP), "dsu: has DSU_COMPRESS_STEP frames");
        check(hasType(frames, ActionType::DSU_MERGE_LINK), "dsu: has DSU_MERGE_LINK frames");
        check(hasType(frames, ActionType::DSU_MERGE_SIZE), "dsu: has DSU_MERGE_SIZE frames");
        check(hasType(frames, ActionType::DSU_DONE), "dsu: has DSU_DONE frame");

        bool sawSizeUpdate = false;
        bool sawParentUpdate = false;
        for (const auto& f : frames) {
            if (f.type == ActionType::DSU_MERGE_SIZE && f.val1 > 1) sawSizeUpdate = true;
            if (f.type == ActionType::DSU_MERGE_LINK) sawParentUpdate = true;
        }
        check(sawSizeUpdate, "dsu: sz[] accumulates past the initial value of 1");
        check(sawParentUpdate, "dsu: pa[] is rewritten on merge");

        m.m_animated = false;
        for (const auto& f : frames) {
            m.renderFrame(f);
            m.finalizeFrame();
        }
    }

    {
        printf("--- euler sieve ---\n");
        EulerSieveModule m;
        QGraphicsScene scene;
        m.buildLayout(&scene, "60");
        auto frames = m.runAlgorithm();
        check(hasType(frames, ActionType::ES_OUTER), "euler: has ES_OUTER frames");
        check(hasType(frames, ActionType::ES_INNER), "euler: has ES_INNER frames");
        check(hasType(frames, ActionType::ES_CROSS), "euler: has ES_CROSS frames");
        check(hasType(frames, ActionType::ES_BREAK), "euler: has ES_BREAK frames");
        check(hasType(frames, ActionType::ES_PRIME_ADD), "euler: has ES_PRIME_ADD frames");
        check(hasType(frames, ActionType::ES_DONE), "euler: has ES_DONE frame");

        std::vector<int> crossed;
        for (const auto& f : frames) {
            if (f.type == ActionType::ES_CROSS) crossed.push_back(f.u);
        }
        std::vector<int> sorted = crossed;
        std::sort(sorted.begin(), sorted.end());
        bool allUnique = std::adjacent_find(sorted.begin(), sorted.end()) == sorted.end();
        check(allUnique, "euler: every composite is crossed out exactly once");
        check(static_cast<int>(crossed.size()) == 60 - 1 - 17, "euler: crosses 60 - 1 - 17 primes = 42 composites");

        bool everyBreakIsValid = true;
        for (const auto& f : frames) {
            if (f.type != ActionType::ES_BREAK) continue;
            if (f.val2 == 1) continue;
            if (f.v <= 0) { everyBreakIsValid = false; break; }
            if (f.u % f.v != 0) { everyBreakIsValid = false; break; }
        }
        check(everyBreakIsValid, "euler: every break happens when i % p == 0");

        m.m_animated = false;
        for (const auto& f : frames) {
            m.renderFrame(f);
            m.finalizeFrame();
        }
        int struck = 0;
        int prime = 0;
        for (const auto& kv : m.cells()) {
            if (kv.second->isStruck()) ++struck;
            else if (kv.second->value == 1) continue;
            else ++prime;
        }
        printf("  struck=%d prime=%d\n", struck, prime);
        check(struck == 42, "euler: 42 cells are struck out in the grid");
        check(prime == 17, "euler: 17 cells remain prime");
    }

    {
        printf("--- eratosthenes unchanged ---\n");
        SieveOfEratosthenesModule m;
        QGraphicsScene scene;
        m.buildLayout(&scene, "60");
        auto frames = m.runAlgorithm();
        check(hasType(frames, ActionType::SV_CROSS_BATCH), "eratosthenes: still uses batch crossing");
        check(!hasType(frames, ActionType::ES_CROSS), "eratosthenes: does not emit euler frames");
    }

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}
