#include <QApplication>
#include <QGraphicsScene>
#include <QFontMetrics>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QPainter>
#include <QImage>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <memory>
#include "modules/IModule.h"
#include "render/GraphElements.h"
#include "ui/UiFonts.h"
#include "StrictAuditModules.h"

struct Box {
    QRectF rect;
    QString tag;
    QString kind;
    QGraphicsItem* owner = nullptr;
};

static bool g_scriptText = false;

static void collect(QGraphicsItem* item, std::vector<Box>& out) {
    if (item->opacity() < 0.05) return;
    if (!item->isVisible()) return;
    if (auto* t = qgraphicsitem_cast<QGraphicsTextItem*>(item)) {
        const QString s = t->toPlainText().trimmed();
        if (s.isEmpty()) return;
        out.push_back({t->sceneBoundingRect(), "TXT:" + s.left(24), "text", t});
        return;
    }
    if (auto* b = qgraphicsitem_cast<ArrayBlock*>(item)) {
        out.push_back({b->mapRectToScene(QRectF(0, 0, b->cellWidth(), b->cellHeight())), QString("CELL#%1").arg(b->id), "cell", b});
        if (b->showLabel) {
            out.push_back({b->mapRectToScene(b->indexRect()), QString("IDX#%1").arg(b->id), "text", b});
        }
        return;
    }
    if (auto* p = qgraphicsitem_cast<PointerBlock*>(item)) {
        out.push_back({p->mapRectToScene(QRectF(p->labelLeft(), p->labelTop(), p->labelWidth(), 20.0)), "PTR:" + p->label, "ptr", p});
        return;
    }
    if (auto* n = qgraphicsitem_cast<GraphNode*>(item)) {
        out.push_back({n->mapRectToScene(QRectF(-20, -20, 40, 40)), QString("NODE#%1").arg(n->id), "node", n});
        if (!n->subLabel.isEmpty()) {
            QFontMetrics fm(UiFonts::mono(10, true));
            const double w = fm.horizontalAdvance(n->subLabel) + 16.0;
            out.push_back({n->mapRectToScene(QRectF(-w / 2.0, -44, w, 18)), n->subLabel.left(18), "text", n});
        }
        if (!n->valueText.isEmpty()) {
            QFontMetrics fm(UiFonts::mono(10, true));
            const double w = fm.horizontalAdvance(n->valueText) + 12.0;
            out.push_back({n->mapRectToScene(QRectF(-w / 2.0, 22, w, 18)), n->valueText.left(18), "text", n});
        }
    }
}

static double overlapArea(const QRectF& a, const QRectF& b) {
    const QRectF r = a.intersected(b);
    if (r.isEmpty()) return 0.0;
    return r.width() * r.height();
}

static bool exempt(const Box& A, const Box& B) {
    if (A.kind == "node" && B.kind == "node") return true;
    if (A.kind == "cell" && B.kind == "cell") return true;
    return false;
}

static int g_conflicts = 0;

static void auditFrames(IModule* m, const char* label, std::vector<std::pair<QString, QRectF>>& report) {
    QGraphicsScene scene;
    m->m_animated = false;
    m->buildLayout(&scene, m->getDefaultInput());
    const auto frames = m->runAlgorithm();
    for (size_t fi = 0; fi < frames.size(); ++fi) {
        m->renderFrame(frames[fi]);
        m->finalizeFrame();
        std::vector<Box> boxes;
        for (auto* item : scene.items()) {
            if (qgraphicsitem_cast<QGraphicsLineItem*>(item)) continue;
            if (dynamic_cast<GraphEdge*>(item)) continue;
            collect(item, boxes);
        }
        for (size_t i = 0; i < boxes.size(); ++i) {
            for (size_t j = i + 1; j < boxes.size(); ++j) {
                if (exempt(boxes[i], boxes[j])) continue;
                const double area = overlapArea(boxes[i].rect, boxes[j].rect);
                if (area < 12.0) continue;
                ++g_conflicts;
                if (report.size() < 200) {
                    report.push_back({QString("%1 f%2 %3 <-> %4 area=%5  A(%6,%7 %8x%9) B(%10,%11 %12x%13)")
                                          .arg(QString::fromUtf8(label)).arg(fi)
                                          .arg(boxes[i].tag).arg(boxes[j].tag).arg(int(area))
                                          .arg(int(boxes[i].rect.x())).arg(int(boxes[i].rect.y()))
                                          .arg(int(boxes[i].rect.width())).arg(int(boxes[i].rect.height()))
                                          .arg(int(boxes[j].rect.x())).arg(int(boxes[j].rect.y()))
                                          .arg(int(boxes[j].rect.width())).arg(int(boxes[j].rect.height())),
                                      boxes[i].rect});
                }
            }
        }
    }
    m->reset();
}

static bool tipAnchored(PointerBlock* ptr) {
    QGraphicsItem* anchor = ptr->anchorItem();
    if (!anchor) return true;
    const QPointF tip = ptr->pos();
    if (auto* cell = qgraphicsitem_cast<ArrayBlock*>(anchor)) {
        const QPointF c = cell->centerPos();
        if (ptr->direction() == PointerBlock::Down) {
            return std::abs(tip.x() - c.x()) < 1.5 && tip.y() <= cell->pos().y() - 1.0;
        }
        if (ptr->direction() == PointerBlock::Up) {
            return std::abs(tip.x() - c.x()) < 1.5 && tip.y() >= cell->pos().y() + cell->cellHeight() + 1.0;
        }
        if (ptr->direction() == PointerBlock::Right) {
            return std::abs(tip.y() - c.y()) < 1.5 && tip.x() <= cell->pos().x() - 1.0;
        }
        return std::abs(tip.y() - c.y()) < 1.5 && tip.x() >= cell->pos().x() + cell->cellWidth() + 1.0;
    }
    if (auto* node = qgraphicsitem_cast<GraphNode*>(anchor)) {
        const QPointF c = node->pos();
        if (ptr->direction() == PointerBlock::Down) return std::abs(tip.x() - c.x()) < 1.5 && tip.y() <= c.y() - 20.0;
        if (ptr->direction() == PointerBlock::Up) return std::abs(tip.x() - c.x()) < 1.5 && tip.y() >= c.y() + 20.0;
        if (ptr->direction() == PointerBlock::Right) return std::abs(tip.y() - c.y()) < 1.5 && tip.x() <= c.x() - 20.0;
        return std::abs(tip.y() - c.y()) < 1.5 && tip.x() >= c.x() + 20.0;
    }
    return true;
}

static int g_tipFail = 0;
static int g_axisFail = 0;

static bool labelOnAxis(PointerBlock* ptr, QGraphicsItem* anchor) {
    const double tipX = ptr->pos().x();
    const double tipY = ptr->pos().y();
    const double labelCenterX = ptr->pos().x() + ptr->labelLeft() + ptr->labelWidth() / 2.0;
    const double labelCenterY = ptr->pos().y() + ptr->labelTop() + 10.0;
    if (auto* cell = qgraphicsitem_cast<ArrayBlock*>(anchor)) {
        const QPointF c = cell->centerPos();
        if (ptr->direction() == PointerBlock::Down || ptr->direction() == PointerBlock::Up) {
            return std::abs(labelCenterX - tipX) < 1.5 && std::abs(tipX - c.x()) < 1.5;
        }
        return std::abs(labelCenterY - tipY) < 1.5 && std::abs(tipY - c.y()) < 1.5;
    }
    if (auto* node = qgraphicsitem_cast<GraphNode*>(anchor)) {
        const QPointF c = node->pos();
        if (ptr->direction() == PointerBlock::Down || ptr->direction() == PointerBlock::Up) {
            return std::abs(labelCenterX - tipX) < 1.5 && std::abs(tipX - c.x()) < 1.5;
        }
        return std::abs(labelCenterY - tipY) < 1.5 && std::abs(tipY - c.y()) < 1.5;
    }
    return true;
}

static void auditTips(IModule* m, const char* label) {
    QGraphicsScene scene;
    m->m_animated = false;
    m->buildLayout(&scene, m->getDefaultInput());
    const auto frames = m->runAlgorithm();
    for (const auto& f : frames) {
        m->renderFrame(f);
        m->finalizeFrame();
        std::vector<QGraphicsItem*> alive;
        for (auto* item : scene.items()) alive.push_back(item);
        for (auto* item : alive) {
            PointerBlock* ptr = qgraphicsitem_cast<PointerBlock*>(item);
            if (!ptr) continue;
            if (ptr->opacity() < 0.05) continue;
            if (!ptr->isVisible()) continue;
            QGraphicsItem* anchor = ptr->anchorItem();
            if (!anchor) continue;
            if (std::find(alive.begin(), alive.end(), anchor) == alive.end()) continue;
            if (!tipAnchored(ptr)) {
                QGraphicsItem* an = ptr->anchorItem();
                QString kind = "none";
                double ax = 0, ay = 0;
                if (an) { ax = an->pos().x(); ay = an->pos().y(); }
                if (qgraphicsitem_cast<ArrayBlock*>(an)) kind = "cell";
                else if (qgraphicsitem_cast<GraphNode*>(an)) kind = "node";
                printf("[TIP-FAIL] %s ptr=%s tip=(%.1f,%.1f) dir=%d anchor=%s@(%.1f,%.1f)\n", label,
                       qPrintable(ptr->label), ptr->pos().x(), ptr->pos().y(),
                       static_cast<int>(ptr->direction()), qPrintable(kind), ax, ay);
                ++g_tipFail;
                break;
            }
            if (!labelOnAxis(ptr, anchor)) {
                printf("[AXIS-FAIL] %s ptr=%s tip=(%.1f,%.1f) labelCenter=(%.1f,%.1f) dir=%d\n", label,
                       qPrintable(ptr->label), ptr->pos().x(), ptr->pos().y(),
                       ptr->pos().x() + ptr->labelLeft() + ptr->labelWidth() / 2.0,
                       ptr->pos().y() + ptr->labelTop() + 10.0,
                       static_cast<int>(ptr->direction()));
                ++g_axisFail;
                break;
            }
        }
    }
    m->reset();
}

int main(int argc, char** argv) {
    setvbuf(stdout, nullptr, _IONBF, 0);
    QApplication app(argc, argv);

    std::vector<std::unique_ptr<IModule>> mods;
    mods.push_back(std::make_unique<TarjanCutVertexBridgeModule>());
    mods.push_back(std::make_unique<BFSModule>());
    mods.push_back(std::make_unique<DfsModule>());
    mods.push_back(std::make_unique<PrimModule>());
    mods.push_back(std::make_unique<KruskalModule>());
    mods.push_back(std::make_unique<DijkstraNaiveModule>());
    mods.push_back(std::make_unique<DijkstraHeapModule>());
    mods.push_back(std::make_unique<TopologicalSortModule>());
    mods.push_back(std::make_unique<EulerModule>());
    mods.push_back(std::make_unique<BipartiteModule>());
    mods.push_back(std::make_unique<DSUPathCompressionModule>());
    mods.push_back(std::make_unique<ForwardStarModule>());
    mods.push_back(std::make_unique<TarjanModule>());
    mods.push_back(std::make_unique<HungarianModule>());
    mods.push_back(std::make_unique<FenwickTreeModule>());
    mods.push_back(std::make_unique<SegmentTreeBuildModule>());
    mods.push_back(std::make_unique<SegmentTreeQueryModule>());
    mods.push_back(std::make_unique<SegmentTreeTagModule>());
    mods.push_back(std::make_unique<HashTableModule>());
    mods.push_back(std::make_unique<TrieModule>());
    mods.push_back(std::make_unique<StringHashModule>());
    mods.push_back(std::make_unique<KMPNextModule>());
    mods.push_back(std::make_unique<KMPMatchModule>());
    mods.push_back(std::make_unique<Knapsack01Module>());
    mods.push_back(std::make_unique<KnapsackCompleteModule>());
    mods.push_back(std::make_unique<LISModule>());
    mods.push_back(std::make_unique<LCSModule>());
    mods.push_back(std::make_unique<BubbleSortModule>());
    mods.push_back(std::make_unique<SelectionSortModule>());
    mods.push_back(std::make_unique<InsertionSortModule>());
    mods.push_back(std::make_unique<ShellSortModule>());
    mods.push_back(std::make_unique<QuickSortModule>());
    mods.push_back(std::make_unique<MergeSortModule>());
    mods.push_back(std::make_unique<CountingSortModule>());
    mods.push_back(std::make_unique<BucketSortModule>());
    mods.push_back(std::make_unique<RadixSortModule>());
    mods.push_back(std::make_unique<HeapBasicsModule>());
    mods.push_back(std::make_unique<HeapAdjustModule>());
    mods.push_back(std::make_unique<HeapSortModule>());
    mods.push_back(std::make_unique<StackModule>());
    mods.push_back(std::make_unique<QueueModule>());
    mods.push_back(std::make_unique<MonotonicStackModule>());
    mods.push_back(std::make_unique<CircularQueueModule>());
    mods.push_back(std::make_unique<MonotonicQueueModule>());
    mods.push_back(std::make_unique<IntervalSchedModule>());
    mods.push_back(std::make_unique<HuffmanTreeModule>());
    mods.push_back(std::make_unique<ExgcdModule>());
    mods.push_back(std::make_unique<FastPowModule>());
    mods.push_back(std::make_unique<GcdModule>());
    mods.push_back(std::make_unique<SieveOfEratosthenesModule>());
    mods.push_back(std::make_unique<EulerSieveModule>());
    mods.push_back(std::make_unique<TreeSubtreeSizeModule>());
    mods.push_back(std::make_unique<TreeDiameterModule>());
    mods.push_back(std::make_unique<PrefixSumModule>());
    mods.push_back(std::make_unique<DifferenceModule>());
    mods.push_back(std::make_unique<BinarySearchClosedModule>());
    mods.push_back(std::make_unique<BinarySearchHalfOpenModule>());
    mods.push_back(std::make_unique<SandboxArrayModule>());
    mods.push_back(std::make_unique<SandboxMatrixModule>());
    mods.push_back(std::make_unique<SandboxTreeModule>());
    mods.push_back(std::make_unique<SandboxGraphModule>());
    mods.push_back(std::make_unique<SandboxBlankModule>());

    for (int pass = 0; pass < 2; ++pass) {
        g_scriptText = (pass == 1);
        IModule::setScriptTextVisible(g_scriptText);
        printf("=== pass scriptText=%d ===\n", g_scriptText ? 1 : 0);
        for (auto& m : mods) {
            const QString name = m->getModuleName();
            printf("  module %s [frames]\n", name.toUtf8().constData());
            fflush(stdout);
            std::vector<std::pair<QString, QRectF>> report;
            auditFrames(m.get(), name.toUtf8().constData(), report);
            printf("  module %s [tips]\n", name.toUtf8().constData());
            fflush(stdout);
            auditTips(m.get(), name.toUtf8().constData());
            printf("  module %s [done]\n", name.toUtf8().constData());
            fflush(stdout);
            for (auto& r : report) printf("  %s\n", qPrintable(r.first));
        }
    }

    printf("\nTOTAL OVERLAP CONFLICTS = %d\n", g_conflicts);
    printf("TOTAL TIP FAILURES = %d\n", g_tipFail);
    printf("TOTAL AXIS FAILURES = %d\n", g_axisFail);
    return (g_conflicts == 0 && g_tipFail == 0 && g_axisFail == 0) ? 0 : 1;
}
