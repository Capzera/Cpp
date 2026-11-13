#include <QApplication>
#include <QImage>
#include <QPainter>
#include <QGraphicsScene>
#include <cstdio>
#include "modules/FenwickTreeModule.h"
#include "modules/KnapsackCompleteModule.h"
#include "modules/TarjanCutVertexBridgeModule.h"
#include "modules/HungarianModule.h"
#include "modules/HeapAdjustModule.h"
#include "modules/HeapSortModule.h"

static int g_fail = 0;

static void check(const char* name, bool ok) {
    printf("[%s] %s\n", ok ? "PASS" : "FAIL", name);
    if (!ok) g_fail++;
}

static int nonWhite(const QImage& img) {
    int n = 0;
    for (int y = 0; y < img.height(); ++y)
        for (int x = 0; x < img.width(); ++x)
            if (img.pixelColor(x, y).lightness() < 250) n++;
    return n;
}

static QImage shot(QGraphicsScene* scene, const QString& name) {
    QImage img(scene->sceneRect().size().toSize(), QImage::Format_ARGB32);
    img.fill(Qt::white);
    QPainter p(&img);
    scene->render(&p);
    img.save(name);
    return img;
}

static bool hasFrame(const std::vector<AnimFrame>& frames, ActionType type) {
    for (const auto& f : frames) if (f.type == type) return true;
    return false;
}

int main(int argc, char** argv) {
    setvbuf(stdout, nullptr, _IONBF, 0);
    QApplication app(argc, argv);
    printf("=== start ===\n");

    {
        printf("--- fenwick tree ---\n");
        QGraphicsScene scene;
        FenwickTreeModule m;
        m.buildLayout(&scene, "5\n3 1 4 2 6\n2\n1 2 5\n2 5");
        auto frames = m.runAlgorithm();
        check("fenwick: frames generated", frames.size() > 10);
        check("fenwick: has FT_INSERT", hasFrame(frames, ActionType::FT_INSERT));
        check("fenwick: has FT_JUMP_UP", hasFrame(frames, ActionType::FT_JUMP_UP));
        check("fenwick: has FT_ACC", hasFrame(frames, ActionType::FT_ACC));
        check("fenwick: has FT_RESULT", hasFrame(frames, ActionType::FT_RESULT));

        int result = 0;
        for (const auto& f : frames) if (f.type == ActionType::FT_RESULT) result = f.val1;
        check("fenwick: sum(5) after add(2,5) == 21", result == 21);

        for (const auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "fenwick.png");
        check("fenwick: scene rendered", nonWhite(img) > 500);
    }

    {
        printf("--- fenwick invalid input ---\n");
        QGraphicsScene scene;
        FenwickTreeModule m;
        m.buildLayout(&scene, "garbage");
        auto frames = m.runAlgorithm();
        check("fenwick: invalid input yields no frames", frames.empty());
    }

    {
        printf("--- complete knapsack ---\n");
        QGraphicsScene scene;
        KnapsackCompleteModule m;
        m.buildLayout(&scene, "3 8\n2 3\n3 4\n4 5");
        auto frames = m.runAlgorithm();
        check("knapsack complete: frames generated", frames.size() > 20);
        check("knapsack complete: has KC_CANDIDATE", hasFrame(frames, ActionType::KC_CANDIDATE));
        check("knapsack complete: has KC_DECIDE", hasFrame(frames, ActionType::KC_DECIDE));

        int finalAnswer = 0;
        for (const auto& f : frames) if (f.type == ActionType::KC_DECIDE && f.val2 == 2) finalAnswer = f.val1;
        check("knapsack complete: dp[3][8] == 12", finalAnswer == 12);

        for (const auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "knapsack_complete.png");
        check("knapsack complete: scene rendered", nonWhite(img) > 500);
    }

    {
        printf("--- tarjan cut vertex and bridge ---\n");
        QGraphicsScene scene;
        TarjanCutVertexBridgeModule m;
        m.buildLayout(&scene, "6 6\n1 2\n2 3\n3 1\n3 4\n4 5\n5 6");
        auto frames = m.runAlgorithm();
        check("tarjan cut: frames generated", frames.size() > 15);
        check("tarjan cut: has TC_VISIT", hasFrame(frames, ActionType::TC_VISIT));
        check("tarjan cut: has TC_CUT", hasFrame(frames, ActionType::TC_CUT));
        check("tarjan cut: has TC_BRIDGE", hasFrame(frames, ActionType::TC_BRIDGE));

        int cutNodes = 0;
        for (const auto& f : frames) {
            if (f.type == ActionType::TC_CUT && f.val1 == 1) {
                bool dup = false;
                for (const auto& g : frames) {
                    if (&g == &f) break;
                    if (g.type == ActionType::TC_CUT && g.val1 == 1 && g.u == f.u) { dup = true; break; }
                }
                if (!dup) ++cutNodes;
            }
        }
        check("tarjan cut: found cut vertices", cutNodes >= 2);

        for (const auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "tarjan_cut.png");
        check("tarjan cut: scene rendered", nonWhite(img) > 500);
    }

    {
        printf("--- hungarian matching ---\n");
        QGraphicsScene scene;
        HungarianModule m;
        m.buildLayout(&scene, "3 3\n1 1\n1 2\n2 2\n3 3");
        auto frames = m.runAlgorithm();
        check("hungarian: frames generated", frames.size() > 5);
        check("hungarian: has HN_ASSIGN", hasFrame(frames, ActionType::HN_ASSIGN));

        int maxMatch = 0;
        for (const auto& f : frames) if (f.type == ActionType::HN_DONE) maxMatch = f.val1;
        check("hungarian: max matching size == 3", maxMatch == 3);

        for (const auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "hungarian.png");
        check("hungarian: scene rendered", nonWhite(img) > 500);
    }

    {
        printf("--- hungarian augmenting path ---\n");
        QGraphicsScene scene;
        HungarianModule m;
        m.buildLayout(&scene, "2 2\n1 1\n1 2\n2 1");
        auto frames = m.runAlgorithm();
        check("hungarian: has HN_OCCUPIED for augmenting path", hasFrame(frames, ActionType::HN_OCCUPIED));
        int maxMatch = 0;
        for (const auto& f : frames) if (f.type == ActionType::HN_DONE) maxMatch = f.val1;
        check("hungarian: max matching size == 2", maxMatch == 2);
    }

    {
        printf("--- heap adjust ---\n");
        QGraphicsScene scene;
        HeapAdjustModule m;
        m.buildLayout(&scene, "5\n10 5 8 2 9");
        auto frames = m.runAlgorithm();
        check("heap adjust: frames generated", frames.size() > 8);
        check("heap adjust: has HG_COMPARE", hasFrame(frames, ActionType::HG_COMPARE));
        check("heap adjust: has HG_SWAP", hasFrame(frames, ActionType::HG_SWAP));

        for (const auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "heap_adjust.png");
        check("heap adjust: scene rendered", nonWhite(img) > 500);
    }

    {
        printf("--- heap sort ---\n");
        QGraphicsScene scene;
        HeapSortModule m;
        m.buildLayout(&scene, "6\n10 5 8 2 9 3");
        auto frames = m.runAlgorithm();
        check("heap sort: frames generated", frames.size() > 15);
        check("heap sort: has HS_SWAP", hasFrame(frames, ActionType::HS_SWAP));

        for (const auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "heap_sort.png");
        check("heap sort: scene rendered", nonWhite(img) > 500);
    }

    printf("=== done, failures: %d ===\n", g_fail);
    return g_fail == 0 ? 0 : 1;
}
