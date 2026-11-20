#include <QApplication>
#include <QGraphicsScene>
#include <QFontMetrics>
#include <cstdio>
#include <algorithm>
#include "modules/DijkstraHeapModule.h"
#include "modules/TreeDiameterModule.h"
#include "modules/ForwardStarModule.h"
#include "render/GraphElements.h"

static int g_fail = 0;
static void check(const char* name, bool ok) {
    printf("[%s] %s\n", ok ? "PASS" : "FAIL", name);
    if (!ok) g_fail++;
}

int main(int argc, char** argv) {
    setvbuf(stdout, nullptr, _IONBF, 0);
    QApplication app(argc, argv);

    {
        printf("--- dijkstra pq text fit ---\n");
        QGraphicsScene scene;
        DijkstraHeapModule m;
        m.buildLayout(&scene, "6 9 1\n1 2 7\n1 3 9\n1 6 14\n2 3 10\n2 4 15\n3 4 11\n3 6 2\n4 5 6\n5 6 9");
        auto frames = m.runAlgorithm();
        for (auto& f : frames) m.renderFrame(f);
        bool allFit = true;
        double worst = 0;
        for (auto& item : scene.items()) {
            ArrayBlock* b = qgraphicsitem_cast<ArrayBlock*>(item);
            if (!b) continue;
            if (!b->showValue) continue;
            QString t = b->displayText();
            if (t.isEmpty()) continue;
            QFontMetrics fm(QFont("Consolas", b->valueFontSize(), QFont::Bold));
            double need = fm.horizontalAdvance(t);
            if (need > b->cellWidth() - 4.0) {
                allFit = false;
                printf("  OVERFLOW '%s' need=%.0f cell=%.0f\n", qPrintable(t), need, b->cellWidth());
            }
            worst = std::max(worst, need / b->cellWidth());
        }
        printf("  worst fill ratio = %.2f\n", worst);
        check("dijheap: all cell texts fit inside cells", allFit);
    }

    {
        printf("--- tree diameter tag overlap ---\n");
        QGraphicsScene scene;
        TreeDiameterModule m;
        m.buildLayout(&scene, "1 2 3\n2 4 5\n3 6\n4 7");
        auto frames = m.runAlgorithm();
        for (auto& f : frames) m.renderFrame(f);
        std::vector<QPair<QPointF, QString>> tags;
        for (auto& item : scene.items()) {
            GraphNode* n = qgraphicsitem_cast<GraphNode*>(item);
            if (!n) continue;
            QString tag = n->tagText;
            if (tag.isEmpty()) continue;
            tags.push_back({n->pos(), tag});
        }
        QFontMetrics fm(QFont("Consolas", 11, QFont::Bold));
        bool noOverlap = true;
        for (size_t i = 0; i < tags.size(); ++i) {
            for (size_t j = i + 1; j < tags.size(); ++j) {
                double dx = std::abs(tags[i].first.x() - tags[j].first.x());
                double dy = std::abs(tags[i].first.y() - tags[j].first.y());
                if (dy > 30.0) continue;
                double w1 = fm.horizontalAdvance(tags[i].second);
                double w2 = fm.horizontalAdvance(tags[j].second);
                if (dx < (w1 + w2) / 2.0) {
                    noOverlap = false;
                    printf("  OVERLAP '%s' vs '%s' dx=%.0f\n", qPrintable(tags[i].second),
                           qPrintable(tags[j].second), dx);
                }
            }
        }
        check("diameter: node tags do not overlap", noOverlap);
    }

    {
        printf("--- forward star pointer clearance ---\n");
        QGraphicsScene scene;
        ForwardStarModule m;
        m.buildLayout(&scene, "5 6 1\n1 2 3\n1 3 5\n2 4 2\n3 4 1\n4 5 4\n2 5 6");
        auto frames = m.runAlgorithm();
        m.m_animated = false;
        for (auto& f : frames) m.renderFrame(f);
        PointerBlock* ptrK = nullptr;
        std::vector<double> rowY;
        for (auto& item : scene.items()) {
            PointerBlock* p = qgraphicsitem_cast<PointerBlock*>(item);
            if (p && p->label == "k") ptrK = p;
            ArrayBlock* b = qgraphicsitem_cast<ArrayBlock*>(item);
            if (!b) continue;
            double y = b->pos().y();
            bool known = false;
            for (double v : rowY) {
                if (std::abs(v - y) < 1.0) { known = true; break; }
            }
            if (!known) rowY.push_back(y);
        }
        std::sort(rowY.begin(), rowY.end());
        double toRowBottom = -1;
        if (rowY.size() >= 2) {
            double toRowTop = rowY[1];
            for (auto* item : scene.items()) {
                ArrayBlock* b = qgraphicsitem_cast<ArrayBlock*>(item);
                if (!b) continue;
                if (std::abs(b->pos().y() - toRowTop) < 1.0) {
                    toRowBottom = b->pos().y() + b->cellHeight();
                    break;
                }
            }
        }
        check("forwardstar: k pointer found", ptrK != nullptr);
        if (ptrK) {
            printf("  ptrK.y=%.0f toRowBottom=%.0f\n", ptrK->pos().y(), toRowBottom);
            check("forwardstar: k pointer sits at the to row bottom edge",
                  toRowBottom > 0 && std::abs(ptrK->pos().y() - toRowBottom) < 2.0);
        }
    }

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}
