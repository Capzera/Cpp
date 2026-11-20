#include <QApplication>
#include <QImage>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <vector>
#include "modules/BucketSortModule.h"
#include "modules/RadixSortModule.h"
#include "modules/CircularQueueModule.h"
#include "modules/MonotonicQueueModule.h"
#include "modules/StringHashModule.h"
#include "modules/LCSModule.h"
#include "modules/HashTableModule.h"
#include "modules/TrieModule.h"
#include "modules/HuffmanTreeModule.h"
#include "modules/ExgcdModule.h"
#include "render/GraphElements.h"
#include "render/MatrixBlock.h"

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

static bool tipOutsideCell(PointerBlock* ptr, ArrayBlock* cell) {
    const double tipX = ptr->pos().x();
    const double tipY = ptr->pos().y();
    const double cellLeft = cell->pos().x();
    const double cellRight = cellLeft + cell->cellWidth();
    const double cellTop = cell->pos().y();
    const double cellBottom = cellTop + cell->cellHeight();
    if (ptr->direction() == PointerBlock::Down) return tipY <= cellTop - 1.0;
    if (ptr->direction() == PointerBlock::Up) return tipY >= cellBottom + 1.0;
    if (ptr->direction() == PointerBlock::Right) return tipX <= cellLeft - 1.0;
    return tipX >= cellRight + 1.0;
}

static void auditPointer(IModule* mod, QGraphicsScene& scene, const std::vector<AnimFrame>& frames,
                         const char* label, const char* name) {
    bool everAnchored = false;
    for (const auto& f : frames) {
        mod->renderFrame(f);
        for (QGraphicsItem* item : scene.items()) {
            PointerBlock* ptr = qgraphicsitem_cast<PointerBlock*>(item);
            if (!ptr || ptr->label != QLatin1String(label)) continue;
            ArrayBlock* cell = qgraphicsitem_cast<ArrayBlock*>(ptr->anchorItem());
            if (!cell) continue;
            everAnchored = true;
            if (!tipOutsideCell(ptr, cell)) {
                printf("[FAIL] %s  tip=(%.1f,%.1f) cell=[%.1f..%.1f, %.1f..%.1f] dir=%d\n",
                       name, ptr->pos().x(), ptr->pos().y(), cell->pos().x(),
                       cell->pos().x() + cell->cellWidth(), cell->pos().y(),
                       cell->pos().y() + cell->cellHeight(), static_cast<int>(ptr->direction()));
                g_fail++;
                return;
            }
        }
    }
    if (!everAnchored) {
        printf("[FAIL] %s (pointer never anchored to a cell)\n", name);
        g_fail++;
        return;
    }
    printf("[PASS] %s\n", name);
}

int main(int argc, char** argv) {
    setvbuf(stdout, nullptr, _IONBF, 0);
    QApplication app(argc, argv);
    printf("=== start ===\n");

    {
        printf("--- hash table: k pointer stays outside the slot ---\n");
        QGraphicsScene scene;
        HashTableModule m;
        m.buildLayout(&scene, "4\n5\n3\n7\n11");
        auto frames = m.runAlgorithm();
        auditPointer(&m, scene, frames, "k", "hash: k arrow tip never enters the slot cell");
        for (const auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "v_hash_k.png");
        check("hash: rendered", nonWhite(img) > 300);
    }

    {
        printf("--- trie: i pointer stays outside the char cell ---\n");
        QGraphicsScene scene;
        TrieModule m;
        m.buildLayout(&scene, "3\ncat\ncar\ndog");
        auto frames = m.runAlgorithm();
        auditPointer(&m, scene, frames, "i", "trie: i arrow tip never enters the char cell");
        for (const auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "v_trie_i.png");
        check("trie: rendered", nonWhite(img) > 300);
    }

    {
        printf("--- bucket sort: pointers stay outside ---\n");
        QGraphicsScene scene;
        BucketSortModule m;
        m.buildLayout(&scene, "8\n37 22 11 45 13 30 26 8");
        auto frames = m.runAlgorithm();
        auditPointer(&m, scene, frames, "i", "bucket: i arrow tip never enters the source cell");
        auditPointer(&m, scene, frames, "b", "bucket: b arrow tip never enters the bucket head");
    }

    {
        printf("--- radix sort: pointers stay outside ---\n");
        QGraphicsScene scene;
        RadixSortModule m;
        m.buildLayout(&scene, "6\n329 457 657 839 436 720");
        auto frames = m.runAlgorithm();
        auditPointer(&m, scene, frames, "i", "radix: i arrow tip never enters the source cell");
        auditPointer(&m, scene, frames, "d", "radix: d arrow tip never enters the bucket head");
    }

    {
        printf("--- circular queue: pointers stay outside ---\n");
        QGraphicsScene scene;
        CircularQueueModule m;
        m.buildLayout(&scene, "5\nPUSH 1\nPUSH 2\nPUSH 3\nPUSH 4\nPUSH 5\nPOP\nPUSH 6");
        auto frames = m.runAlgorithm();
        auditPointer(&m, scene, frames, "head", "cq: head arrow tip never enters the cell");
        auditPointer(&m, scene, frames, "tail", "cq: tail arrow tip never enters the cell");
    }

    {
        printf("--- monotonic queue: pointers stay outside ---\n");
        QGraphicsScene scene;
        MonotonicQueueModule m;
        m.buildLayout(&scene, "6\n1 2 3 4 5 6\n3");
        auto frames = m.runAlgorithm();
        auditPointer(&m, scene, frames, "i", "mq: i arrow tip never enters the cell");
        auditPointer(&m, scene, frames, "head", "mq: head arrow tip never enters the cell");
        auditPointer(&m, scene, frames, "tail", "mq: tail arrow tip never enters the cell");
    }

    {
        printf("--- string hash: pointer stays outside ---\n");
        QGraphicsScene scene;
        StringHashModule m;
        m.buildLayout(&scene, "abcdefg\ncdefg");
        auto frames = m.runAlgorithm();
        auditPointer(&m, scene, frames, "i", "stringhash: i arrow tip never enters the char cell");
    }

    {
        printf("--- lcs: pointers stay outside ---\n");
        QGraphicsScene scene;
        LCSModule m;
        m.buildLayout(&scene, "abcbdab\nbdcaba");
        auto frames = m.runAlgorithm();
        auditPointer(&m, scene, frames, "i", "lcs: i arrow tip never enters the row cell");
        auditPointer(&m, scene, frames, "j", "lcs: j arrow tip never enters the column cell");
        for (const auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "v_lcs.png");
        check("lcs: rendered", nonWhite(img) > 400);
    }

    {
        printf("--- exgcd: row pointer tip aims at the active row ---\n");
        QGraphicsScene scene;
        ExgcdModule m;
        m.buildLayout(&scene, "11914 146415");
        auto frames = m.runAlgorithm();

        bool everAnchored = false;
        int mismatches = 0;
        for (const auto& f : frames) {
            m.renderFrame(f);
            for (QGraphicsItem* item : scene.items()) {
                PointerBlock* ptr = qgraphicsitem_cast<PointerBlock*>(item);
                if (!ptr || ptr->label != QStringLiteral("行")) continue;
                QGraphicsTextItem* anchor = dynamic_cast<QGraphicsTextItem*>(ptr->anchorItem());
                if (!anchor) continue;
                everAnchored = true;
                const double wantY = anchor->pos().y() + anchor->boundingRect().center().y();
                const double targetY = anchor->pos().y() + ptr->anchorOffset().y();
                if (qAbs(targetY - wantY) > 2.0) {
                    printf("[FAIL] exgcd: 行 pointer aims at y=%.1f but row center is y=%.1f\n",
                           targetY, wantY);
                    mismatches++;
                }
                const double tipX = anchor->pos().x() + ptr->anchorOffset().x();
                if (tipX >= anchor->pos().x() - 1.0) {
                    printf("[FAIL] exgcd: 行 pointer tip x=%.1f must stay left of the row text x=%.1f\n",
                           tipX, anchor->pos().x());
                    mismatches++;
                }
                if (ptr->direction() != PointerBlock::Right) {
                    printf("[FAIL] exgcd: 行 pointer must point right, dir=%d\n",
                           static_cast<int>(ptr->direction()));
                    mismatches++;
                }
            }
        }
        check("exgcd: 行 pointer is anchored to a row", everAnchored);
        check("exgcd: 行 pointer tip always aims at the active row center", mismatches == 0);

        for (const auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "v_exgcd.png");
        check("exgcd: rendered", nonWhite(img) > 300);
    }

    {
        printf("--- huffman: six phases, progressive tree ---\n");
        QGraphicsScene scene;
        HuffmanTreeModule m;
        m.buildLayout(&scene, "6\n45 9 16 5 13 12");
        auto frames = m.runAlgorithm();

        int lift = 0, drop = 0, sortDone = 0, queueDrop = 0;
        int pick = 0, jelly = 0, promote = 0, merge = 0, final = 0;
        int code = 0, wpl = 0, wplDone = 0;
        for (auto& f : frames) {
            if (f.type == ActionType::HF_SORT_LIFT) ++lift;
            if (f.type == ActionType::HF_SORT_DROP) ++drop;
            if (f.type == ActionType::HF_SORT_DONE) ++sortDone;
            if (f.type == ActionType::HF_QUEUE_DROP) ++queueDrop;
            if (f.type == ActionType::HF_PICK) ++pick;
            if (f.type == ActionType::HF_JELLY) ++jelly;
            if (f.type == ActionType::HF_PROMOTE) ++promote;
            if (f.type == ActionType::HF_MERGE) ++merge;
            if (f.type == ActionType::HF_FINAL) ++final;
            if (f.type == ActionType::HF_CODE) ++code;
            if (f.type == ActionType::HF_WPL) ++wpl;
            if (f.type == ActionType::HF_WPL_DONE) ++wplDone;
        }
        check("huffman: sort phase lifts out-of-place elements", lift >= 1);
        check("huffman: sort phase drops elements into place", drop >= 1);
        check("huffman: sort completion frame recorded", sortDone == 1);
        check("huffman: candidate queue drop frame recorded", queueDrop == 1);
        check("huffman: one pick frame per merge", pick == merge);
        check("huffman: one jelly frame per merge", jelly == merge);
        check("huffman: merge count for 6 leaves is 5", merge == 5);
        check("huffman: final frame recorded", final == 1);
        check("huffman: one code frame per leaf", code == 6);
        check("huffman: one wpl term per leaf", wpl == 6);
        check("huffman: wpl summary frame recorded", wplDone == 1);

        int sortIndex = -1, mergeIndex = -1, codeIndex = -1;
        for (size_t i = 0; i < frames.size(); ++i) {
            if (frames[i].type == ActionType::HF_SORT_DONE && sortIndex < 0) sortIndex = static_cast<int>(i);
            if (frames[i].type == ActionType::HF_MERGE && mergeIndex < 0) mergeIndex = static_cast<int>(i);
            if (frames[i].type == ActionType::HF_CODE && codeIndex < 0) codeIndex = static_cast<int>(i);
        }
        check("huffman: sorting happens before building", sortIndex >= 0 && mergeIndex > sortIndex);
        check("huffman: building happens before coding", mergeIndex >= 0 && codeIndex > mergeIndex);

        for (auto& f : frames) m.renderFrame(f);

        int treeNodes = 0, queueNodes = 0, edges = 0;
        for (QGraphicsItem* item : scene.items()) {
            GraphNode* gn = qgraphicsitem_cast<GraphNode*>(item);
            if (gn) {
                if (gn->id >= 5000) ++queueNodes; else ++treeNodes;
            }
            if (qgraphicsitem_cast<GraphEdge*>(item)) ++edges;
        }
        check("huffman: full tree has 11 nodes", treeNodes == 11);
        check("huffman: candidate queue has 11 copies", queueNodes == 11);
        check("huffman: full tree has 10 edges", edges == 10);

        int promoteCount = 0;
        for (auto& f : frames) {
            if (f.type == ActionType::HF_PROMOTE) ++promoteCount;
        }
        check("huffman: one promote frame per merge", promoteCount == merge);

        QImage img = shot(&scene, "v_huffman.png");
        check("huffman: rendered", nonWhite(img) > 300);
    }

    {
        printf("--- matrix: plain brackets wrap the whole grid ---\n");
        MatrixBlock block(3, 4);
        block.setShape(MatrixBlock::PlainMatrix);
        const QRectF rect = block.boundingRect();
        const double gridW = 4 * block.cellWidth();
        const double gridH = 3 * block.cellHeightValue();
        printf("  bounding=(%.1f,%.1f %.1fx%.1f) grid=%.1fx%.1f margin=%.1f\n",
               rect.x(), rect.y(), rect.width(), rect.height(), gridW, gridH, block.sideMargin());
        check("matrix: side margin leaves room for the bracket arm", block.sideMargin() >= 20.0);
        check("matrix: bounding box is wider than the grid", rect.width() > gridW);
        check("matrix: bounding box is taller than the grid", rect.height() > gridH);
        check("matrix: bracket arm fits inside the side margin", block.sideMargin() > 17.0);
    }

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}
