#include <QApplication>
#include <QGraphicsScene>
#include <QImage>
#include <QPainter>
#include <cstdio>
#include "modules/TrieModule.h"
#include "modules/BipartiteModule.h"
#include "modules/EulerModule.h"
#include "modules/IntervalSchedModule.h"
#include "modules/ModuleFields.h"

static int g_fail = 0;

static void check(bool ok, const char* name) {
    printf("[%s] %s\n", ok ? "PASS" : "FAIL", name);
    if (!ok) g_fail++;
}

static void renderModule(IModule* mod, const QString& input, const QString& tag, int frameIndex) {
    QGraphicsScene scene;
    mod->buildLayout(&scene, input);
    auto frames = mod->runAlgorithm();
    mod->m_animated = false;
    if (frameIndex < 0) frameIndex = static_cast<int>(frames.size()) - 1;
    for (int i = 0; i <= frameIndex && i < static_cast<int>(frames.size()); ++i) {
        mod->renderFrame(frames[i]);
    }
    const QRectF bounds = scene.itemsBoundingRect().adjusted(-30, -30, 30, 30);
    QImage img(bounds.size().toSize(), QImage::Format_ARGB32);
    img.fill(Qt::white);
    QPainter p(&img);
    p.setRenderHint(QPainter::Antialiasing);
    scene.render(&p, QRectF(QPointF(0, 0), img.size()), bounds);
    p.end();
    img.save(QString("vis_%1.png").arg(tag));
    printf("saved vis_%s.png  frames=%d\n", tag.toUtf8().constData(), int(frames.size()));
}

static bool hasFrame(IModule* mod, const QString& input, ActionType type) {
    QGraphicsScene scene;
    mod->buildLayout(&scene, input);
    auto frames = mod->runAlgorithm();
    for (const auto& f : frames) {
        if (f.type == type) return true;
    }
    return false;
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    {
        TrieModule m;
        ModuleField f = ModuleField::makeSegment("mode", "演示模式", {"插入建树", "查询单词", "字典序遍历"}, 1);
        m.applyModuleSetting("mode", f);
        ModuleField q = ModuleField::makeText("query", "查询单词", "car");
        m.applyModuleSetting("query", q);
        renderModule(&m, "3\ncat\ncar\ndog", "trie_query", -1);
        check(hasFrame(&m, "3\ncat\ncar\ndog", ActionType::TR_QUERY_HIT), "trie query emits hit frame");
        check(hasFrame(&m, "3\ncat\ncar\ndog", ActionType::TR_QUERY_FOUND), "trie query emits found frame");
    }

    {
        TrieModule m;
        ModuleField f = ModuleField::makeSegment("mode", "演示模式", {"插入建树", "查询单词", "字典序遍历"}, 2);
        m.applyModuleSetting("mode", f);
        renderModule(&m, "3\ncat\ncar\ndog", "trie_order", -1);
        check(hasFrame(&m, "3\ncat\ncar\ndog", ActionType::TR_ORDER_DONE), "trie order emits done frame");
    }

    {
        TrieModule m;
        ModuleField f = ModuleField::makeSegment("mode", "演示模式", {"插入建树", "查询单词", "字典序遍历"}, 1);
        m.applyModuleSetting("mode", f);
        ModuleField q = ModuleField::makeText("query", "查询单词", "cab");
        m.applyModuleSetting("query", q);
        check(hasFrame(&m, "3\ncat\ncar\ndog", ActionType::TR_QUERY_MISS), "trie query emits miss frame");
    }

    {
        BipartiteModule m;
        renderModule(&m, "5 6\n1 2\n2 3\n3 1\n3 4\n4 5\n5 3", "bip_oddcycle", -1);
        check(hasFrame(&m, "5 6\n1 2\n2 3\n3 1\n3 4\n4 5\n5 3", ActionType::BP_ODD_CYCLE), "bipartite emits odd cycle frame");
    }

    {
        EulerModule m;
        renderModule(&m, "5 6\n1 2\n1 3\n2 3\n3 4\n3 5\n4 5", "euler", -1);
        check(hasFrame(&m, "5 6\n1 2\n1 3\n2 3\n3 4\n3 5\n4 5", ActionType::EU_DEADEND), "euler emits deadend frame");
        check(hasFrame(&m, "5 6\n1 2\n1 3\n2 3\n3 4\n3 5\n4 5", ActionType::EU_SPLICE), "euler emits splice frame");
    }

    {
        IntervalSchedModule m;
        renderModule(&m, "5\n1 3\n2 5\n3 7\n5 9\n6 10", "interval", -1);
        check(hasFrame(&m, "5\n1 3\n2 5\n3 7\n5 9\n6 10", ActionType::IV_REJECT_BAND), "interval emits conflict band frame");
        check(hasFrame(&m, "5\n1 3\n2 5\n3 7\n5 9\n6 10", ActionType::IV_COUNTER_DONE), "interval emits counterexample frame");
    }

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}
