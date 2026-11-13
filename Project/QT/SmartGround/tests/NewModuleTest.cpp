#include <QApplication>
#include <QImage>
#include <QPainter>
#include <QGraphicsScene>
#include <cstdio>
#include "modules/SegmentTreeBuildModule.h"
#include "modules/SegmentTreeTagModule.h"
#include "modules/SegmentTreeQueryModule.h"
#include "modules/KMPMatchModule.h"
#include "modules/KMPNextModule.h"

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
        printf("--- build module ---\n");
        QGraphicsScene scene;
        SegmentTreeBuildModule m;
        m.buildLayout(&scene, "4 100\n1 2 3 4");
        printf("  layout done\n");
        auto frames = m.runAlgorithm();
        printf("  frames=%d\n", (int)frames.size());
        check("build: frames generated", frames.size() > 10);
        check("build: has SEG_PUSHUP", [&] {
            for (auto& f : frames) if (f.type == ActionType::SEG_PUSHUP) return true;
            return false;
        }());
        check("build: has SEG_LEAF", [&] {
            for (auto& f : frames) if (f.type == ActionType::SEG_LEAF) return true;
            return false;
        }());
        for (auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "seg_build.png");
        check("build: scene rendered", nonWhite(img) > 500);
    }

    {
        QGraphicsScene scene;
        SegmentTreeTagModule m;
        m.buildLayout(&scene, "4 3 100\n1 2 3 4\n1 1 2 3\n2 1 2 5\n2 2 3 7");
        auto frames = m.runAlgorithm();
        check("tag: frames generated", frames.size() > 15);
        check("tag: has SEG_COVER_MUL", [&] {
            for (auto& f : frames) if (f.type == ActionType::SEG_COVER_MUL) return true;
            return false;
        }());
        check("tag: has SEG_COVER_ADD", [&] {
            for (auto& f : frames) if (f.type == ActionType::SEG_COVER_ADD) return true;
            return false;
        }());
        check("tag: has SEG_PUSH_DOWN", [&] {
            for (auto& f : frames) if (f.type == ActionType::SEG_PUSH_DOWN) return true;
            return false;
        }());
        for (auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "seg_tag.png");
        check("tag: scene rendered", nonWhite(img) > 500);
    }

    {
        QGraphicsScene scene;
        SegmentTreeQueryModule m;
        m.buildLayout(&scene, "4 1 100\n1 2 3 4\n3 2 4");
        auto frames = m.runAlgorithm();
        check("query: frames generated", frames.size() > 10);
        check("query: has SEG_QUERY_COVER", [&] {
            for (auto& f : frames) if (f.type == ActionType::SEG_QUERY_COVER) return true;
            return false;
        }());
        bool hasAnswer = false;
        for (auto& f : frames) if (f.type == ActionType::SEG_QUERY_DONE && f.val1 == 9) hasAnswer = true;
        check("query: answer 2+3+4=9", hasAnswer);
        for (auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "seg_query.png");
        check("query: scene rendered", nonWhite(img) > 500);
    }

    {
        QGraphicsScene scene;
        KMPMatchModule m;
        m.buildLayout(&scene, "ABABCABAA\nABAB");
        auto frames = m.runAlgorithm();
        check("kmp match: frames generated", frames.size() > 10);
        check("kmp match: has KMPM_FALLBACK", [&] {
            for (auto& f : frames) if (f.type == ActionType::KMPM_FALLBACK) return true;
            return false;
        }());
        check("kmp match: has KMPM_HIT", [&] {
            for (auto& f : frames) if (f.type == ActionType::KMPM_HIT) return true;
            return false;
        }());
        int hits = 0;
        for (auto& f : frames) if (f.type == ActionType::KMPM_HIT) hits++;
        check("kmp match: 1 hit in ABABCABAA", hits == 1);
        bool hitAt1 = false;
        for (auto& f : frames) if (f.type == ActionType::KMPM_HIT && f.val1 == 1) hitAt1 = true;
        check("kmp match: hit starts at position 1", hitAt1);
        for (auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "kmp_match.png");
        check("kmp match: scene rendered", nonWhite(img) > 300);
    }

    {
        QGraphicsScene scene;
        KMPNextModule m;
        m.buildLayout(&scene, "ABABCABAA");
        auto frames = m.runAlgorithm();
        check("kmp next: frames generated", frames.size() > 10);
        bool piOk = false;
        for (auto& f : frames) if (f.type == ActionType::KMP_SET && f.u == 7 && f.val1 == 2) piOk = true;
        check("kmp next: pi[7]=2 for ABABCABAA", piOk);
        for (auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "kmp_next.png");
        check("kmp next: scene rendered", nonWhite(img) > 300);
    }

    {
        QGraphicsScene scene;
        SegmentTreeBuildModule m;
        m.buildLayout(&scene, "");
        check("build: invalid input handled", m.runAlgorithm().empty());
    }

    {
        QGraphicsScene scene;
        KMPMatchModule m;
        m.buildLayout(&scene, "");
        check("kmp match: invalid input handled", m.runAlgorithm().empty());
    }

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}
