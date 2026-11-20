#include <QApplication>
#include <QImage>
#include <QPainter>
#include <QGraphicsScene>
#include <cstdio>
#include "modules/BucketSortModule.h"
#include "modules/RadixSortModule.h"
#include "modules/CircularQueueModule.h"
#include "modules/MonotonicQueueModule.h"
#include "modules/ExgcdModule.h"
#include "modules/HuffmanTreeModule.h"
#include "modules/StringHashModule.h"
#include "modules/LCSModule.h"

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
        printf("--- bucket sort ---\n");
        QGraphicsScene scene;
        BucketSortModule m;
        m.buildLayout(&scene, "8\n37 22 11 45 13 30 26 8");
        auto frames = m.runAlgorithm();
        check("bucket: frames generated", frames.size() > 10);
        int bucketFrames = 0, sortFrames = 0, collectFrames = 0;
        for (auto& f : frames) {
            if (f.type == ActionType::BK_BUCKET) ++bucketFrames;
            if (f.type == ActionType::BK_SORT) ++sortFrames;
            if (f.type == ActionType::BK_COLLECT) ++collectFrames;
        }
        check("bucket: one bucket frame per element", bucketFrames == 8);
        check("bucket: one collect frame per element", collectFrames == 8);
        check("bucket: has sort frames", sortFrames >= 8);
        std::vector<int> collected;
        for (auto& f : frames) if (f.type == ActionType::BK_COLLECT) collected.push_back(f.val1);
        std::vector<int> expect = {8, 11, 13, 22, 26, 30, 37, 45};
        check("bucket: collected values are sorted", collected == expect);
        for (auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "bucket_sort.png");
        check("bucket: scene rendered", nonWhite(img) > 400);
    }

    {
        printf("--- radix sort ---\n");
        QGraphicsScene scene;
        RadixSortModule m;
        m.buildLayout(&scene, "6\n329 457 657 839 436 720");
        auto frames = m.runAlgorithm();
        check("radix: frames generated", frames.size() > 20);
        int digitFrames = 0, bucketFrames = 0, collectFrames = 0;
        for (auto& f : frames) {
            if (f.type == ActionType::RD_DIGIT) ++digitFrames;
            if (f.type == ActionType::RD_BUCKET) ++bucketFrames;
            if (f.type == ActionType::RD_COLLECT) ++collectFrames;
        }
        check("radix: digit extraction per element per pass", digitFrames == 18);
        check("radix: one bucket frame per element per pass", bucketFrames == 18);
        check("radix: one collect frame per element per pass", collectFrames == 18);
        std::vector<int> lastPass;
        for (auto& f : frames) {
            if (f.type == ActionType::RD_COLLECT && f.u == 2) lastPass.push_back(f.val1);
        }
        std::vector<int> expect = {329, 436, 457, 657, 720, 839};
        check("radix: final pass yields ascending order", lastPass == expect);
        for (auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "radix_sort.png");
        check("radix: scene rendered", nonWhite(img) > 400);
    }

    {
        printf("--- circular queue ---\n");
        QGraphicsScene scene;
        CircularQueueModule m;
        m.buildLayout(&scene, "5\nPUSH 1\nPUSH 2\nPUSH 3\nPUSH 4\nPUSH 5\nPOP\nPUSH 6");
        auto frames = m.runAlgorithm();
        check("cq: frames generated", frames.size() > 10);
        int enq = 0, deq = 0, wrap = 0;
        for (auto& f : frames) {
            if (f.type == ActionType::CQ_ENQUEUE) ++enq;
            if (f.type == ActionType::CQ_DEQUEUE) ++deq;
            if (f.type == ActionType::CQ_WRAP) ++wrap;
        }
        check("cq: enqueue frames recorded", enq == 6);
        check("cq: dequeue frames recorded", deq == 1);
        check("cq: wrap-around detected", wrap >= 1);
        for (auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "circular_queue.png");
        check("cq: scene rendered", nonWhite(img) > 300);
    }

    {
        printf("--- monotonic queue ---\n");
        QGraphicsScene scene;
        MonotonicQueueModule m;
        m.buildLayout(&scene, "8\n5 1 9 8 7 6 1 2\n3");
        auto frames = m.runAlgorithm();
        check("mq: frames generated", frames.size() > 15);
        int maxFrames = 0, pushFrames = 0, popFrames = 0, expireFrames = 0;
        for (auto& f : frames) {
            if (f.type == ActionType::MQ_MAX) ++maxFrames;
            if (f.type == ActionType::MQ_PUSH) ++pushFrames;
            if (f.type == ActionType::MQ_POP_BACK) ++popFrames;
            if (f.type == ActionType::MQ_EXPIRE) ++expireFrames;
        }
        check("mq: window max frames recorded", maxFrames >= 4);
        check("mq: push frames recorded", pushFrames >= 8);
        check("mq: back-pop frames recorded", popFrames >= 1);
        check("mq: expire frames recorded", expireFrames >= 1);
        std::vector<int> maxima;
        for (auto& f : frames) if (f.type == ActionType::MQ_MAX) maxima.push_back(f.val1);
        check("mq: first window max is 9", !maxima.empty() && maxima.front() == 9);
        for (auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "monotonic_queue.png");
        check("mq: scene rendered", nonWhite(img) > 400);
    }

    {
        printf("--- exgcd ---\n");
        QGraphicsScene scene;
        ExgcdModule m;
        m.buildLayout(&scene, "11914 146415");
        auto frames = m.runAlgorithm();
        check("exgcd: frames generated", frames.size() > 8);
        int recurse = 0, base = 0, backtrace = 0, equation = 0;
        for (auto& f : frames) {
            if (f.type == ActionType::EX_RECURSE) ++recurse;
            if (f.type == ActionType::EX_BASE) ++base;
            if (f.type == ActionType::EX_BACKTRACE) ++backtrace;
            if (f.type == ActionType::EX_EQUATION) ++equation;
        }
        check("exgcd: recurse frames recorded", recurse >= 3);
        check("exgcd: base case recorded", base == 1);
        check("exgcd: backtrace frames recorded", backtrace >= 3);
        check("exgcd: equation transform frames recorded", equation == 3);

        long long x = 0, y = 0;
        for (auto& f : frames) {
            if (f.type == ActionType::EX_DONE) { x = f.u; y = f.v; }
        }
        check("exgcd: inverse of 11914 mod 146415 is 15079", x == 15079);
        check("exgcd: 11914*15079 - 146415*1227 == 1", 11914LL * x - 146415LL * y == 1);

        bool hasFinalEq = false;
        for (auto& f : frames) {
            if (f.type == ActionType::EX_BACKTRACE &&
                f.explanation.contains("15079") && f.explanation.contains("1227")) {
                hasFinalEq = true;
            }
        }
        check("exgcd: final back-substitution reaches 15079 and 1227", hasFinalEq);

        {
            QGraphicsScene s2;
            ExgcdModule m2;
            m2.m_animated = false;
            m2.buildLayout(&s2, "11914 146415");
            auto f2 = m2.runAlgorithm();
            for (auto& fr : f2) m2.renderFrame(fr);
            std::vector<std::pair<QRectF, QString>> boxes;
            for (auto* item : s2.items()) {
                QGraphicsTextItem* t = qgraphicsitem_cast<QGraphicsTextItem*>(item);
                if (!t) continue;
                const QString txt = t->toPlainText().trimmed();
                if (txt.isEmpty()) continue;
                boxes.push_back({t->sceneBoundingRect(), txt});
            }
            int overlaps = 0;
            for (size_t i = 0; i < boxes.size(); ++i) {
                for (size_t j = i + 1; j < boxes.size(); ++j) {
                    if (!boxes[i].first.intersected(boxes[j].first).isEmpty()) ++overlaps;
                }
            }
            check("exgcd: no overlapping text blocks", overlaps == 0);
        }

        for (auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "exgcd.png");
        check("exgcd: scene rendered", nonWhite(img) > 300);
    }

    {
        printf("--- huffman ---\n");
        QGraphicsScene scene;
        HuffmanTreeModule m;
        m.buildLayout(&scene, "6\n5 9 12 13 16 45");
        auto frames = m.runAlgorithm();
        check("huffman: frames generated", frames.size() > 10);
        int pick = 0, merge = 0;
        for (auto& f : frames) {
            if (f.type == ActionType::HF_PICK) ++pick;
            if (f.type == ActionType::HF_MERGE) ++merge;
        }
        check("huffman: pick frames recorded", pick >= 5);
        check("huffman: merge frames recorded", merge == 5);
        for (auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "huffman_tree.png");
        check("huffman: scene rendered", nonWhite(img) > 300);

        {
            QGraphicsScene s2;
            HuffmanTreeModule m2;
            m2.m_animated = false;
            m2.buildLayout(&s2, "7\n45 13 20 16 9 12 5");
            auto f2 = m2.runAlgorithm();
            for (auto& fr : f2) m2.renderFrame(fr);

            std::map<int, QPointF> treePos;
            int arrayCells = 0;
            bool arrayHasValue = true;
            for (auto* item : s2.items()) {
                GraphNode* gn = qgraphicsitem_cast<GraphNode*>(item);
                if (gn && gn->id < 5000) treePos[gn->id] = gn->pos();
                ArrayBlock* ab = qgraphicsitem_cast<ArrayBlock*>(item);
                if (ab && ab->id < 6000) {
                    ++arrayCells;
                    if (!ab->showValue || ab->displayText().isEmpty()) arrayHasValue = false;
                }
            }
            check("huffman: frequency array has exactly n cells", arrayCells == 7);
            check("huffman: frequency array cells all carry values", arrayHasValue);

            bool rootOnTop = true;
            double bottomY = 0.0;
            double topY = 1e9;
            for (const auto& kv : treePos) {
                topY = qMin(topY, kv.second.y());
                bottomY = qMax(bottomY, kv.second.y());
            }
            if (topY > bottomY) rootOnTop = false;
            check("huffman: root row is the topmost row", rootOnTop);

            std::map<int, int> parentOf;
            for (auto* item : s2.items()) {
                GraphEdge* e = qgraphicsitem_cast<GraphEdge*>(item);
                if (!e || !e->source || !e->dest) continue;
                parentOf[e->source->id] = e->dest->id;
            }
            bool parentAboveChild = true;
            for (const auto& kv : parentOf) {
                auto childIt = treePos.find(kv.first);
                auto parentIt = treePos.find(kv.second);
                if (childIt == treePos.end() || parentIt == treePos.end()) continue;
                if (parentIt->second.y() >= childIt->second.y() - 1.0) parentAboveChild = false;
            }
            check("huffman: every parent sits strictly above its children", parentAboveChild);

            int directed = 0;
            for (auto* item : s2.items()) {
                GraphEdge* e = qgraphicsitem_cast<GraphEdge*>(item);
                if (e && e->is_directed) ++directed;
            }
            check("huffman: tree edges carry no arrowheads", directed == 0);
        }
    }


    {
        printf("--- string hash ---\n");
        QGraphicsScene scene;
        StringHashModule m;
        m.buildLayout(&scene, "abcdefg\ncdefg");
        auto frames = m.runAlgorithm();
        check("stringhash: frames generated", frames.size() > 10);
        int prep = 0, pow = 0, query = 0, match = 0;
        for (auto& f : frames) {
            if (f.type == ActionType::STH_PREP) ++prep;
            if (f.type == ActionType::STH_POW) ++pow;
            if (f.type == ActionType::STH_QUERY) ++query;
            if (f.type == ActionType::STH_MATCH) ++match;
        }
        check("stringhash: prefix hash frames recorded", prep >= 7);
        check("stringhash: power frames recorded", pow >= 7);
        check("stringhash: query frames recorded", query >= 1);
        check("stringhash: match frame recorded", match >= 1);
        int answer = -1;
        for (auto& f : frames) if (f.type == ActionType::STH_DONE) answer = f.u;
        check("stringhash: overlap length of abcdefg/cdefg is 5", answer == 5);
        for (auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "string_hash.png");
        check("stringhash: scene rendered", nonWhite(img) > 300);
    }

    {
        printf("--- lcs ---\n");
        QGraphicsScene scene;
        LCSModule m;
        m.buildLayout(&scene, "abcbdab\nbdcaba");
        auto frames = m.runAlgorithm();
        check("lcs: frames generated", frames.size() > 20);
        int candidate = 0, decide = 0, trace = 0;
        for (auto& f : frames) {
            if (f.type == ActionType::LC_CANDIDATE) ++candidate;
            if (f.type == ActionType::LC_DECIDE) ++decide;
            if (f.type == ActionType::LC_TRACE) ++trace;
        }
        check("lcs: candidate frames recorded", candidate == 72);
        printf("  lcs counts: candidate=%d decide=%d trace=%d\n", candidate, decide, trace);
        check("lcs: decide frames recorded", decide == 42);
        check("lcs: trace frames recorded", trace == 8);
        int length = -1;
        for (auto& f : frames) if (f.type == ActionType::LC_DONE) length = f.u;
        check("lcs: length of abcbdab/bdcaba is 4", length == 4);
        for (auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "lcs.png");
        check("lcs: scene rendered", nonWhite(img) > 400);
    }

    {
        printf("--- invalid inputs ---\n");
        QGraphicsScene scene;
        BucketSortModule a;
        a.buildLayout(&scene, "");
        check("bucket: invalid input handled", a.runAlgorithm().empty());
        RadixSortModule b;
        b.buildLayout(&scene, "abc");
        check("radix: invalid input handled", b.runAlgorithm().empty());
        CircularQueueModule c;
        c.buildLayout(&scene, "");
        check("cq: invalid input handled", c.runAlgorithm().empty());
        MonotonicQueueModule d;
        d.buildLayout(&scene, "");
        check("mq: invalid input handled", d.runAlgorithm().empty());
        ExgcdModule e;
        e.buildLayout(&scene, "0 0");
        check("exgcd: invalid input handled", e.runAlgorithm().empty());
        HuffmanTreeModule f;
        f.buildLayout(&scene, "");
        check("huffman: invalid input handled", f.runAlgorithm().empty());
        StringHashModule g;
        g.buildLayout(&scene, "");
        check("stringhash: invalid input handled", g.runAlgorithm().empty());
        LCSModule h;
        h.buildLayout(&scene, "");
        check("lcs: invalid input handled", h.runAlgorithm().empty());
    }

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}
