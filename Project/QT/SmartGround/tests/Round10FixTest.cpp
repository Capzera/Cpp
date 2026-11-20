#include <QApplication>
#include <QGraphicsScene>
#include <QEventLoop>
#include <QTimer>
#include <QImage>
#include <QPainter>
#include <QGraphicsItem>
#include <cstdio>
#include <vector>
#include "modules/GcdModule.h"
#include "modules/DfsModule.h"
#include "modules/PrimModule.h"
#include "modules/CountingSortModule.h"
#include "modules/BipartiteModule.h"
#include "modules/EulerModule.h"
#include "modules/TrieModule.h"
#include "modules/HashTableModule.h"
#include "modules/IModule.h"
#include "ui/UiFonts.h"

static int g_fail = 0;

static void check(bool ok, const char* name) {
    printf("[%s] %s\n", ok ? "PASS" : "FAIL", name);
    if (!ok) g_fail++;
}

static std::vector<ArrayBlock*> collectBlocks(QGraphicsScene* scene) {
    std::vector<ArrayBlock*> result;
    for (QGraphicsItem* item : scene->items()) {
        if (ArrayBlock* block = qgraphicsitem_cast<ArrayBlock*>(item)) result.push_back(block);
    }
    return result;
}

static bool hasGapBetweenCells(QGraphicsScene* scene) {
    std::vector<ArrayBlock*> blocks = collectBlocks(scene);
    for (size_t i = 0; i < blocks.size(); ++i) {
        for (size_t j = i + 1; j < blocks.size(); ++j) {
            ArrayBlock* a = blocks[i];
            ArrayBlock* b = blocks[j];
            if (a->cellWidth() != b->cellWidth()) continue;
            const double dx = b->pos().x() - a->pos().x();
            const double dy = b->pos().y() - a->pos().y();
            if (qAbs(dy) > 1.0) continue;
            if (qAbs(dx) < 0.5) continue;
            if (qAbs(dx - a->cellWidth()) < 0.5) continue;
            return true;
        }
    }
    return false;
}

static bool rowIsSeamless(QGraphicsScene* scene, double y, int expectedCells) {
    std::vector<ArrayBlock*> row;
    for (QGraphicsItem* item : scene->items()) {
        ArrayBlock* block = qgraphicsitem_cast<ArrayBlock*>(item);
        if (!block) continue;
        if (qAbs(block->pos().y() - y) < 2.0) row.push_back(block);
    }
    if (expectedCells > 0 && static_cast<int>(row.size()) != expectedCells) return false;
    if (row.size() < 2) return true;
    std::sort(row.begin(), row.end(), [](ArrayBlock* a, ArrayBlock* b) {
        return a->pos().x() < b->pos().x();
    });
    for (size_t i = 0; i + 1 < row.size(); ++i) {
        const double dx = row[i + 1]->pos().x() - row[i]->pos().x();
        if (qAbs(dx - row[i]->cellWidth()) > 0.5) return false;
    }
    return true;
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    {
        GcdModule mod;
        QGraphicsScene scene;
        mod.buildLayout(&scene, "48 18");
        const auto frames = mod.runAlgorithm();
        check(frames.size() >= 5, "gcd: enough frames");
        check(frames.front().type == ActionType::GC_RESET, "gcd: starts with reset");
        check(frames.back().type == ActionType::GC_DONE, "gcd: ends with done");
        int divideCount = 0;
        for (const auto& f : frames) if (f.type == ActionType::GC_DIVIDE) ++divideCount;
        check(divideCount == 3, "gcd: 48%18=12, 18%12=6, 12%6=0 -> 3 rounds");
        check(frames.back().u == 6, "gcd: result is 6");
        bool hasMod = false;
        for (const auto& f : frames) if (f.mathText.contains("%")) hasMod = true;
        check(hasMod, "gcd: shows modulo operation");
        check(!hasGapBetweenCells(&scene), "gcd: no cell gaps");
    }

    {
        PrimModule mod;
        QGraphicsScene scene;
        mod.buildLayout(&scene, mod.getDefaultInput());
        const auto frames = mod.runAlgorithm();
        check(frames.size() >= 10, "prim: enough frames (not 3)");
        int takeCount = 0;
        int finishFail = 0;
        for (const auto& f : frames) {
            if (f.type == ActionType::PR_TAKE) ++takeCount;
            if (f.type == ActionType::PR_FINISH && f.val2 == 1) ++finishFail;
        }
        check(takeCount == 5, "prim: takes 5 nodes for 6-node graph");
        check(finishFail == 0, "prim: no isolated-component false alarm");
        check(frames.back().type == ActionType::PR_FINISH && frames.back().val2 == 0, "prim: finishes successfully");
        int relaxCount = 0;
        for (const auto& f : frames) if (f.type == ActionType::PR_RELAX) ++relaxCount;
        check(relaxCount > 0, "prim: relaxes edges");
    }

    {
        BipartiteModule mod;
        QGraphicsScene scene;
        mod.buildLayout(&scene, mod.getDefaultInput());
        const auto frames = mod.runAlgorithm();
        check(!frames.empty(), "bipartite: has frames");
        int blue = 0;
        int orange = 0;
        for (const auto& f : frames) {
            if (f.type == ActionType::BP_FINISH) {
                blue = f.val1;
                orange = f.val2;
            }
        }
        check(blue == 3 && orange == 3, "bipartite: default graph splits 3 blue / 3 orange");
        check(frames.back().type == ActionType::BP_FINISH && frames.back().u == 1, "bipartite: verdict is bipartite");
    }

    {
        BipartiteModule mod;
        QGraphicsScene scene;
        mod.buildLayout(&scene, "6 6\n1 2\n2 3\n3 1\n3 4\n4 5\n5 6");
        const auto frames = mod.runAlgorithm();
        int blue = 0;
        int orange = 0;
        bool hasConflict = false;
        for (const auto& f : frames) {
            if (f.type == ActionType::BP_FINISH) {
                blue = f.val1;
                orange = f.val2;
            }
            if (f.type == ActionType::BP_CONFLICT) hasConflict = true;
        }
        check(hasConflict, "bipartite: odd-cycle graph raises conflict");
        check(frames.back().u == 0, "bipartite: odd-cycle graph rejected as non-bipartite");
        check(blue > 0 && orange > 0, "bipartite: colours assigned before conflict");
    }

    {
        CountingSortModule mod;
        QGraphicsScene scene;
        mod.buildLayout(&scene, mod.getDefaultInput());
        const auto frames = mod.runAlgorithm();
        int prefixCount = 0;
        for (const auto& f : frames) if (f.type == ActionType::CS_PREFIX) ++prefixCount;
        check(prefixCount > 0, "counting: has prefix frames");
        bool hasMerge = false;
        for (const auto& f : frames) {
            if (f.type == ActionType::CS_PREFIX && f.mathText.contains("+")) hasMerge = true;
        }
        check(hasMerge, "counting: prefix frame shows merge expression");
    }

    {
        TrieModule mod;
        QGraphicsScene scene;
        mod.buildLayout(&scene, "3\ncat\ncar\ndog");
        const auto frames = mod.runAlgorithm();
        check(!frames.empty(), "trie: has frames");
        int createCount = 0;
        int hitCount = 0;
        for (const auto& f : frames) {
            if (f.type == ActionType::TR_CREATE) ++createCount;
            if (f.type == ActionType::TR_HIT) ++hitCount;
        }
        check(createCount == 7, "trie: creates 7 nodes (c/ca/cat/car/d/do/dog)");
        check(hitCount == 2, "trie: reuses existing branch twice (car reuses c,a)");
        bool hasWord = false;
        for (const auto& f : frames) if (f.type == ActionType::TR_WORD) hasWord = true;
        check(hasWord, "trie: has word-loading frame");
        check(rowIsSeamless(&scene, 500.0, 8), "trie: string array row is seamless");
        check(rowIsSeamless(&scene, 596.0, 0), "trie: slot array row is seamless");
    }

    {
        HashTableModule mod;
        QGraphicsScene scene;
        mod.buildLayout(&scene, mod.getDefaultInput());
        const auto frames = mod.runAlgorithm();
        int probeCount = 0;
        for (const auto& f : frames) if (f.type == ActionType::HT_PROBE) ++probeCount;
        check(probeCount >= 3, "hash: shows collision probing");
        check(rowIsSeamless(&scene, 118.0, 7), "hash: table row is seamless");
        check(rowIsSeamless(&scene, 222.0, 7), "hash: key row is seamless");
    }

    {
        EulerModule mod;
        QGraphicsScene scene;
        mod.buildLayout(&scene, "4 4\n1 2\n2 3\n3 4\n4 1");
        const auto frames = mod.runAlgorithm();
        int degreeCount = 0;
        for (const auto& f : frames) if (f.type == ActionType::EU_DEGREE) ++degreeCount;
        check(degreeCount == 4, "euler: degrees computed for all 4 nodes");
        check(frames.back().type == ActionType::EU_DONE, "euler: reaches verdict");
    }

    {
        DfsModule mod;
        QGraphicsScene scene;
        mod.buildLayout(&scene, "5 5 1\n1 2\n2 3\n3 1\n3 4\n4 5");
        mod.runAlgorithm();
        int pointerCount = 0;
        for (QGraphicsItem* item : scene.items()) {
            PointerBlock* ptr = qgraphicsitem_cast<PointerBlock*>(item);
            if (!ptr) continue;
            ++pointerCount;
            const QFontMetrics fm(UiFonts::mono(14, true));
            const double advance = fm.horizontalAdvance(ptr->label);
            check(ptr->labelWidth() >= advance,
                  "dfs: pointer label rect wide enough for text");
            const double paintedAdvance = QFontMetrics(QFont("Consolas", 14, QFont::Bold))
                                             .horizontalAdvance(ptr->label);
            check(ptr->labelWidth() >= paintedAdvance,
                  "dfs: pointer label rect wide enough for bold paint font");
        }
        check(pointerCount >= 2, "dfs: stack and cursor pointers exist");
    }

    {
        IModule* mod = new DfsModule();
        QGraphicsScene scene;
        mod->buildLayout(&scene, "4 3 1\n1 2\n2 3\n3 4");
        AnimFrame frame;
        frame.type = ActionType::DFS_DONE;
        frame.explanation = QString("这是一段比较长的说明文字，用来测试自适应延时的计算是否随文本长度变化");
        frame.mathText = QString("math");
        const int delayLong = IModule::frameDelayMs(frame);
        frame.explanation = QString("短");
        frame.mathText.clear();
        const int delayShort = IModule::frameDelayMs(frame);
        check(delayLong > delayShort, "engine: longer text yields longer delay");
        check(delayShort >= 900 && delayLong <= 3200, "engine: delay clamped to sane range");
        delete mod;
    }

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}
