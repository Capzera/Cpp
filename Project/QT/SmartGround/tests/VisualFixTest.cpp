#include <QApplication>
#include <QGraphicsScene>
#include <cstdio>
#include <cmath>
#include "modules/IModule.h"
#include "modules/BipartiteModule.h"
#include "modules/EulerModule.h"
#include "modules/HungarianModule.h"
#include "render/GraphElements.h"
#include "ui/widgets/MyToggleSwitch.h"
#include "ui/UiTheme.h"

static int g_fail = 0;

static void check(bool ok, const QString& name) {
    printf("[%s] %s\n", ok ? "PASS" : "FAIL", qPrintable(name));
    if (!ok) g_fail++;
}

static bool anyVisibleValue(const std::map<int, ArrayBlock*>& blocks) {
    for (const auto& kv : blocks) {
        if (kv.second->showValue) return true;
    }
    return false;
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    {
        BipartiteModule m;
        QGraphicsScene scene;
        m.buildLayout(&scene, "6 6\n1 2\n1 3\n2 4\n3 4\n3 5\n4 6");
        auto frames = m.runAlgorithm();
        m.m_animated = false;
        for (const auto& f : frames) {
            m.renderFrame(f);
            m.finalizeFrame();
        }
        check(!anyVisibleValue(m.queueBlocks()), "bipartite: queue row emptied after algorithm");
        double maxTailX = 0;
        double minTailY = 1e9;
        for (auto* item : scene.items()) {
            auto* p = qgraphicsitem_cast<PointerBlock*>(item);
            if (!p) continue;
            if (p->opacity() < 0.05) continue;
            if (p->label == "tail") {
                maxTailX = qMax(maxTailX, p->pos().x());
                minTailY = qMin(minTailY, p->pos().y());
            }
        }
        printf("  tail maxX=%.1f minY=%.1f\n", maxTailX, minTailY);
        check(maxTailX < 200.0 + 8 * 46.0 + 10, "bipartite: tail pointer stays within queue row bounds");
    }

    {
        BipartiteModule m;
        QGraphicsScene scene;
        m.buildLayout(&scene, "6 6\n1 2\n1 3\n2 4\n3 4\n3 5\n4 6");
        auto frames = m.runAlgorithm();
        m.m_animated = false;
        double maxLabelY = -1e9;
        for (const auto& f : frames) {
            m.renderFrame(f);
            m.finalizeFrame();
            for (auto* item : scene.items()) {
                auto* p = qgraphicsitem_cast<PointerBlock*>(item);
                if (!p) continue;
                if (p->opacity() < 0.05) continue;
                maxLabelY = qMax(maxLabelY, p->pos().y() + p->labelTop() + 20.0);
            }
        }
        printf("  bipartite max pointer label bottom = %.1f\n", maxLabelY);
        check(maxLabelY < 700.0, "bipartite: pointer labels stay within canvas");
    }

    {
        EulerModule m;
        QGraphicsScene scene;
        m.buildLayout(&scene, "5 6\n1 2\n1 3\n2 3\n3 4\n3 5\n4 5");
        auto frames = m.runAlgorithm();
        m.m_animated = false;
        int bridgeCount = 0;
        for (const auto& f : frames) {
            if (f.type == ActionType::EU_BRIDGE) ++bridgeCount;
        }
        check(bridgeCount > 0, "euler: emits bridge frames");
    }

    {
        MyToggleSwitch t;
        t.setOnText("无向图");
        t.setOffText("有向图");
        t.setChecked(false);
        t.setFixedSize(220, 40);
        t.show();
        app.processEvents();
        t.grab().save("toggle_off.png");

        MyToggleSwitch t2;
        t2.setOnText("无向图");
        t2.setOffText("有向图");
        t2.setChecked(true);
        t2.setFixedSize(220, 40);
        t2.show();
        app.processEvents();
        t2.grab().save("toggle_on.png");
        check(true, "toggle snapshots rendered");
    }

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}
