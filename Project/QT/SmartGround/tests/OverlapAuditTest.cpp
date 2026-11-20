#include <QApplication>
#include <QGraphicsScene>
#include <QFontMetrics>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <cstdio>
#include <algorithm>
#include <vector>
#include "modules/FenwickTreeModule.h"
#include "modules/ForwardStarModule.h"
#include "modules/HeapAdjustModule.h"
#include "modules/HeapSortModule.h"
#include "modules/BubbleSortModule.h"
#include "modules/PrefixSumModule.h"
#include "modules/Knapsack01Module.h"
#include "modules/SegmentTreeBuildModule.h"
#include "modules/DSUBasicModule.h"
#include "modules/TopologicalSortModule.h"
#include "modules/GcdModule.h"
#include "modules/DfsModule.h"
#include "modules/PrimModule.h"
#include "modules/CountingSortModule.h"
#include "modules/BipartiteModule.h"
#include "modules/EulerModule.h"
#include "modules/IntervalSchedModule.h"
#include "modules/TrieModule.h"
#include "modules/HashTableModule.h"
#include "render/GraphElements.h"
#include "ui/UiFonts.h"

static int g_fail = 0;

static void check(const char* name, bool ok) {
    printf("[%s] %s\n", ok ? "PASS" : "FAIL", name);
    if (!ok) g_fail++;
}

enum Kind { KindCell, KindCellIndex, KindPointer, KindLabel, KindNode };

struct Box {
    QRectF rect;
    QString tag;
    Kind kind;
    QGraphicsItem* owner = nullptr;
    QGraphicsItem* anchor = nullptr;
};

static void collect(QGraphicsItem* item, std::vector<Box>& out) {
    if (auto* t = qgraphicsitem_cast<QGraphicsTextItem*>(item)) {
        const QString s = t->toPlainText().trimmed();
        if (s.isEmpty()) return;
        out.push_back({t->sceneBoundingRect(), s.left(18), KindLabel, t, nullptr});
        return;
    }
    if (auto* b = qgraphicsitem_cast<ArrayBlock*>(item)) {
        const QRectF cell = b->mapRectToScene(QRectF(0, 0, b->cellWidth(), b->cellHeight()));
        out.push_back({cell, QString("cell#%1").arg(b->id), KindCell, b, nullptr});
        if (b->showLabel) {
            const QRectF lab = b->mapRectToScene(b->indexRect());
            out.push_back({lab, QString("idx#%1").arg(b->id), KindCellIndex, b, nullptr});
        }
        return;
    }
    if (auto* p = qgraphicsitem_cast<PointerBlock*>(item)) {
        out.push_back({p->mapRectToScene(p->boundingRect()), QString("ptr:%1").arg(p->label), KindPointer, p, p->anchorItem()});
        return;
    }
    if (auto* n = qgraphicsitem_cast<GraphNode*>(item)) {
        out.push_back({n->mapRectToScene(QRectF(-20, -20, 40, 40)), QString("node:%1").arg(n->id), KindNode, n, nullptr});
        if (!n->subLabel.isEmpty()) {
            QFontMetrics fm(UiFonts::mono(10, true));
            const double w = fm.horizontalAdvance(n->subLabel) + 16.0;
            out.push_back({n->mapRectToScene(QRectF(-w / 2.0, -44, w, 18)), n->subLabel.left(18), KindLabel, n, nullptr});
        }
        if (!n->valueText.isEmpty()) {
            QFontMetrics fm(UiFonts::mono(10, true));
            const double w = fm.horizontalAdvance(n->valueText) + 12.0;
            out.push_back({n->mapRectToScene(QRectF(-w / 2.0, 22, w, 18)), n->valueText.left(18), KindLabel, n, nullptr});
        }
    }
}

static double overlapArea(const QRectF& a, const QRectF& b) {
    const QRectF r = a.intersected(b);
    if (r.isEmpty()) return 0.0;
    return r.width() * r.height();
}

static bool sameRow(const Box& a, const Box& b) {
    return std::abs(a.rect.center().y() - b.rect.center().y()) < 3.0
        && a.rect.left() < b.rect.right() && b.rect.left() < a.rect.right();
}

static bool indexOfCell(const Box& label, const Box& cell) {
    if (label.kind != KindLabel) return false;
    if (cell.kind != KindCell) return false;
    const double gap = cell.rect.top() - label.rect.bottom();
    if (gap < -4.0 || gap > 34.0) return false;
    const double overlap = std::min(label.rect.right(), cell.rect.right())
                         - std::max(label.rect.left(), cell.rect.left());
    if (overlap < std::min(label.rect.width(), cell.rect.width()) * 0.35) return false;
    return true;
}

static bool isIndexLike(const Box& b) {
    QString t = b.tag;
    bool numeric = true;
    for (QChar c : t) {
        if (!c.isDigit()) { numeric = false; break; }
    }
    return numeric || b.kind == KindCellIndex;
}

static bool pointerTargets(const Box& pointer, const Box& other) {
    if (pointer.kind != KindPointer) return false;
    if (!pointer.anchor) return false;
    if (pointer.anchor != other.owner) return false;
    return other.kind == KindCell || other.kind == KindNode || other.kind == KindCellIndex;
}

static void auditScene(const char* label, QGraphicsScene& scene, double minArea = 8.0) {
    std::vector<Box> boxes;
    for (auto* item : scene.items()) {
        if (qgraphicsitem_cast<QGraphicsLineItem*>(item)) continue;
        if (qgraphicsitem_cast<LinkLine*>(item)) continue;
        if (qgraphicsitem_cast<QGraphicsPolygonItem*>(item)) continue;
        if (qgraphicsitem_cast<GraphEdge*>(item)) continue;
        collect(item, boxes);
    }
    int conflicts = 0;
    for (size_t i = 0; i < boxes.size(); ++i) {
        for (size_t j = i + 1; j < boxes.size(); ++j) {
            const Box& A = boxes[i];
            const Box& B = boxes[j];
            if (A.kind == KindCell && B.kind == KindCell) {
                if (sameRow(A, B)) continue;
            }
            if (A.kind == KindNode && B.kind == KindNode) continue;
            if ((A.kind == KindNode && B.kind == KindCell) || (B.kind == KindNode && A.kind == KindCell)) continue;
            if ((A.kind == KindNode && B.kind == KindCellIndex) || (B.kind == KindNode && A.kind == KindCellIndex)) continue;
            if (A.kind == KindCellIndex && B.kind == KindCellIndex) continue;
            if (A.kind == KindCellIndex && B.kind == KindCell) {
                if (sameRow(A, B)) continue;
            }
            if (A.kind == KindCell && B.kind == KindCellIndex) {
                if (sameRow(A, B)) continue;
            }
            if (indexOfCell(A, B) || indexOfCell(B, A)) continue;
            if (pointerTargets(A, B) || pointerTargets(B, A)) continue;
            const double area = overlapArea(A.rect, B.rect);
            if (area < minArea) continue;
            ++conflicts;
            printf("  [%s] OVERLAP '%s'(%d) %s <-> '%s'(%d) %s area=%.0f\n", label,
                   qPrintable(A.tag), int(A.kind),
                   qPrintable(QString("(%1,%2 %3x%4)").arg(A.rect.x()).arg(A.rect.y()).arg(A.rect.width()).arg(A.rect.height())),
                   qPrintable(B.tag), int(B.kind),
                   qPrintable(QString("(%1,%2 %3x%4)").arg(B.rect.x()).arg(B.rect.y()).arg(B.rect.width()).arg(B.rect.height())),
                   area);
        }
    }
    printf("  [%s] items=%zu conflicts=%d\n", label, boxes.size(), conflicts);
    check(label, conflicts == 0);
}

template <typename M>
static void auditModule(const char* label) {
    QGraphicsScene scene;
    M m;
    m.m_animated = false;
    m.buildLayout(&scene, m.getDefaultInput());
    for (auto& f : m.runAlgorithm()) m.renderFrame(f);
    auditScene(label, scene);
}

int main(int argc, char** argv) {
    setvbuf(stdout, nullptr, _IONBF, 0);
    QApplication app(argc, argv);

    auditModule<FenwickTreeModule>("fenwick: no text/pointer overlap");
    auditModule<ForwardStarModule>("forwardstar: no text/pointer overlap");
    auditModule<HeapAdjustModule>("heapadjust: no text/pointer overlap");
    auditModule<HeapSortModule>("heapsort: no text/pointer overlap");
    auditModule<BubbleSortModule>("bubble: baseline no overlap");
    auditModule<PrefixSumModule>("prefix: baseline no overlap");
    auditModule<Knapsack01Module>("knapsack: no overlap");
    auditModule<SegmentTreeBuildModule>("segmenttree: no overlap");
    auditModule<DSUBasicModule>("dsu: no overlap");
    auditModule<TopologicalSortModule>("topo: no overlap");
    auditModule<GcdModule>("gcd: no overlap");
    auditModule<DfsModule>("dfs: no overlap");
    auditModule<PrimModule>("prim: no overlap");
    auditModule<CountingSortModule>("counting: no overlap");
    auditModule<BipartiteModule>("bipartite: no overlap");
    auditModule<EulerModule>("euler: no overlap");
    auditModule<IntervalSchedModule>("interval: no overlap");
    auditModule<TrieModule>("trie: no overlap");
    auditModule<HashTableModule>("hashtable: no overlap");

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}
