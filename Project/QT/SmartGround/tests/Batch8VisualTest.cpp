#include <QApplication>
#include <QImage>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <vector>
#include "modules/FastPowModule.h"
#include "modules/SieveOfEratosthenesModule.h"
#include "modules/ForwardStarModule.h"
#include "modules/TreeDiameterModule.h"
#include "modules/DijkstraHeapModule.h"
#include "render/GraphElements.h"

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

static int darkPixels(const QImage& img, int x0, int y0, int x1, int y1) {
    int n = 0;
    x0 = qMax(0, x0); y0 = qMax(0, y0);
    x1 = qMin(img.width(), x1); y1 = qMin(img.height(), y1);
    for (int y = y0; y < y1; ++y)
        for (int x = x0; x < x1; ++x)
            if (img.pixelColor(x, y).lightness() < 160) n++;
    return n;
}

int main(int argc, char** argv) {
    setvbuf(stdout, nullptr, _IONBF, 0);
    QApplication app(argc, argv);
    printf("=== start ===\n");

    {
        printf("--- sieve: digits survive crossing ---\n");
        QGraphicsScene scene;
        SieveOfEratosthenesModule m;
        m.buildLayout(&scene, "60");
        auto frames = m.runAlgorithm();
        for (auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "v_sieve.png");
        check("sieve: rendered", nonWhite(img) > 800);
        ArrayBlock* cell4 = nullptr;
        for (auto& item : scene.items()) {
            ArrayBlock* block = qgraphicsitem_cast<ArrayBlock*>(item);
            if (!block) continue;
            if (block->id == 4 && !block->showLabel) { cell4 = block; break; }
        }
        check("sieve: cell 4 exists", cell4 != nullptr);
        if (cell4) {
            check("sieve: cell 4 still shows its digit", cell4->showValue && cell4->displayText() == "4");
            check("sieve: cell 4 is marked as struck", cell4->isStruck());
        }
    }

    {
        printf("--- fast pow: qpow only, no qmul panel ---\n");
        QGraphicsScene scene;
        FastPowModule m;
        m.buildLayout(&scene, "2 20");
        auto frames = m.runAlgorithm();
        check("fastpow: frame count is compact", frames.size() <= 7);
        int bitFrames = 0;
        for (auto& f : frames) {
            if (f.type == ActionType::FP_BIT) ++bitFrames;
        }
        check("fastpow: bit frame per digit", bitFrames == 5);
        for (auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "v_fastpow.png");
        check("fastpow: rendered", nonWhite(img) > 300);
        QString text;
        for (auto& item : scene.items()) {
            QGraphicsTextItem* t = qgraphicsitem_cast<QGraphicsTextItem*>(item);
            if (!t) continue;
            text += t->toPlainText() + "|";
        }
        check("fastpow: no qmul panel text", !text.contains("qmul") && !text.contains("竖式"));
    }

    {
        printf("--- tree diameter: wide spacing ---\n");
        QGraphicsScene scene;
        TreeDiameterModule m;
        m.buildLayout(&scene, "1 2 3\n2 4 5\n3 6\n4 7");
        auto frames = m.runAlgorithm();
        for (auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "v_diameter.png");
        check("diameter: rendered", nonWhite(img) > 500);
        std::map<int, QPointF> centers;
        for (auto& item : scene.items()) {
            GraphNode* node = qgraphicsitem_cast<GraphNode*>(item);
            if (!node) continue;
            centers[node->id] = node->pos();
        }
        double minGap = 1e9;
        for (auto& a : centers) {
            for (auto& b : centers) {
                if (a.first >= b.first) continue;
                double dx = std::abs(a.second.x() - b.second.x());
                double dy = std::abs(a.second.y() - b.second.y());
                if (dy < 1.0) minGap = qMin(minGap, dx);
            }
        }
        check("diameter: sibling nodes spaced widely", minGap >= 150.0);
    }

    {
        printf("--- dijkstra heap: array pq ---\n");
        QGraphicsScene scene;
        DijkstraHeapModule m;
        m.buildLayout(&scene, "6 9 1\n1 2 7\n1 3 9\n1 6 14\n2 3 10\n2 4 15\n3 4 11\n3 6 2\n4 5 6\n5 6 9");
        auto frames = m.runAlgorithm();
        int pushIndex = 0;
        for (size_t i = 0; i < frames.size(); ++i) {
            if (frames[i].type == ActionType::DH_PUSH) { pushIndex = static_cast<int>(i); break; }
        }
        for (int i = 0; i <= pushIndex && i < static_cast<int>(frames.size()); ++i) m.renderFrame(frames[i]);
        QImage mid = shot(&scene, "v_dijkstra_pq.png");
        check("dijheap: mid-frame rendered", nonWhite(mid) > 800);
        bool pairSeen = false;
        for (auto& item : scene.items()) {
            ArrayBlock* block = qgraphicsitem_cast<ArrayBlock*>(item);
            if (!block) continue;
            if (block->displayText().startsWith("(") && block->displayText().endsWith(")")) pairSeen = true;
        }
        check("dijheap: pq stores pair values", pairSeen);
        for (auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "v_dijkstra.png");
        check("dijheap: rendered", nonWhite(img) > 800);
        GraphNode* heapTreeNode = nullptr;
        for (auto& item : scene.items()) {
            GraphNode* node = qgraphicsitem_cast<GraphNode*>(item);
            if (!node) continue;
            if (node->subLabelText().isEmpty()) continue;
            heapTreeNode = node;
            break;
        }
        check("dijheap: no heap tree node labels", heapTreeNode == nullptr);
    }

    {
        printf("--- forward star: pointer and spacing ---\n");
        QGraphicsScene scene;
        ForwardStarModule m;
        m.buildLayout(&scene, "5 6 1\n1 2 3\n1 3 5\n2 4 2\n3 4 1\n4 5 4\n2 5 6");
        auto frames = m.runAlgorithm();
        m.m_animated = false;
        int addIndex = 0;
        for (size_t i = 0; i < frames.size(); ++i) {
            if (frames[i].type == ActionType::FS_ADD) { addIndex = static_cast<int>(i); break; }
        }
        for (int i = 0; i <= addIndex && i < static_cast<int>(frames.size()); ++i) m.renderFrame(frames[i]);
        shot(&scene, "v_forwardstar_mid.png");
        for (auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "v_forwardstar.png");
        check("forwardstar: rendered", nonWhite(img) > 500);
        PointerBlock* ptrK = nullptr;
        std::vector<double> rowY;
        for (auto& item : scene.items()) {
            PointerBlock* ptr = qgraphicsitem_cast<PointerBlock*>(item);
            if (ptr && ptr->label == "k") ptrK = ptr;
            ArrayBlock* block = qgraphicsitem_cast<ArrayBlock*>(item);
            if (!block) continue;
            double y = block->pos().y();
            bool known = false;
            for (double v : rowY) {
                if (std::abs(v - y) < 1.0) { known = true; break; }
            }
            if (!known) rowY.push_back(y);
        }
        std::sort(rowY.begin(), rowY.end());
        check("forwardstar: four array rows", rowY.size() >= 4);
        double rowGap = rowY.size() >= 4 ? rowY[1] - rowY[0] : 0;
        check("forwardstar: rows are widely spaced", rowGap >= 90.0);
        check("forwardstar: k pointer exists", ptrK != nullptr);
        check("forwardstar: k pointer points up", ptrK && ptrK->direction() == PointerBlock::Up);
        printf("rows:");
        for (double v : rowY) printf(" %.0f", v);
        printf(" ptrK.y=%.0f\n", ptrK ? ptrK->pos().y() : -1.0);
        check("forwardstar: k pointer sits below the to row",
              ptrK && rowY.size() >= 2 && ptrK->pos().y() > rowY[1]);
    }

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}
