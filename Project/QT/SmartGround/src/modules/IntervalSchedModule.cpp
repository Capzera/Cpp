#include "IntervalSchedModule.h"
#include "render/SegmentBar.h"
#include <QFont>
#include <QRegularExpression>
#include <algorithm>
#include <QGraphicsLineItem>
#include "ui/UiFonts.h"

static const QColor kIvIdle = QColor(214, 224, 238);
static const QColor kIvScan = QColor(255, 205, 130);
static const QColor kIvPick = QColor(140, 205, 165);
static const QColor kIvReject = QColor(228, 231, 236);

IntervalSchedModule::IntervalSchedModule() {}

QString IntervalSchedModule::getModuleName() const { return "区间调度"; }
QString IntervalSchedModule::getDefaultInput() const {
    return "5\n"
           "1 3\n"
           "2 5\n"
           "3 7\n"
           "5 9\n"
           "6 10";
}
QString IntervalSchedModule::getInputHint() const {
    return "第一行输入一个正整数 n，表示活动个数（n <= 8）\n"
           "接下来 n 行，每行两个整数 l, r，表示活动的开始与结束时间（l < r）\n"
           "贪心策略：按结束时间从小到大排序，每次选第一个不冲突的活动";
}

void IntervalSchedModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;

    const QStringList lines = inputData.split(QRegularExpression("[\\r\\n]"), Qt::SkipEmptyParts);
    bool ok = false;
    int n = 0;
    if (!lines.isEmpty()) {
        n = lines[0].trimmed().toInt(&ok);
        m_valid = ok && n >= 1 && n <= 8 && lines.size() >= n + 1;
    }

    m_mathText = createScriptText(scene, m_valid ? "区间调度：按结束时间贪心，每次选最早结束且不冲突的活动"
                                        : "输入格式：n，随后输入 n 行 l r；n≤8");
    m_mathText->setFont(UiFonts::mono(15, true));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(140, 18);
    if (!m_valid) return;

    std::vector<std::pair<int, int>> ranges;
    int maxEnd = 0;
    for (int i = 1; i <= n; ++i) {
        const QStringList tokens = lines[i].split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (tokens.size() < 2) {
            m_valid = false;
            break;
        }
        bool tok = false;
        const int l = tokens[0].toInt(&tok);
        const int r = tokens[1].toInt(&tok);
        if (!tok || l < 0 || r <= l) {
            m_valid = false;
            break;
        }
        ranges.push_back({l, r});
        maxEnd = qMax(maxEnd, r);
    }
    if (!m_valid) return;

    const double left = 220.0;
    const double width = 520.0;
    const double scale = width / qMax(1, maxEnd);

    for (int i = 0; i < n; ++i) {
        const double y = 110.0 + i * 52.0;
        const double x1 = ranges[i].first * scale;
        const double x2 = ranges[i].second * scale;
        SegmentBar* bar = new SegmentBar(x1, x2, 30);
        bar->setColor(kIvIdle);
        bar->setBar(QString("活动 %1").arg(i + 1));
        bar->setPos(left + x1, y);
        scene->addItem(bar);

        QGraphicsTextItem* name = scene->addText(QString("活动 %1").arg(i + 1));
        name->setFont(UiFonts::cjk(11, true));
        name->setDefaultTextColor(QColor(60, 75, 95));
        name->setPos(left - 96, y + 4);
        m_segments.push_back({ranges[i].first, ranges[i].second, bar, name});
    }

    QGraphicsLineItem* axis = scene->addLine(left, 96, left + width, 96, QPen(QColor(160, 172, 190), 1.6));
    axis->setZValue(0);
    for (int t = 0; t <= maxEnd; ++t) {
        QGraphicsTextItem* tick = scene->addText(QString::number(t));
        tick->setFont(UiFonts::mono(10, true));
        tick->setDefaultTextColor(QColor(120, 132, 150));
        tick->setPos(left + t * scale - 8, 74);
        tick->setZValue(0);
    }

    m_scanText = createScriptText(scene, "");
    m_scanText->setFont(UiFonts::mono(13, true));
    m_scanText->setDefaultTextColor(QColor(200, 100, 40));
    m_scanText->setPos(left, 96 + n * 52.0 + 10);

    m_resultText = createScriptText(scene, "");
    m_resultText->setFont(UiFonts::cjk(13, true));
    m_resultText->setDefaultTextColor(QColor(40, 140, 70));
    m_resultText->setPos(left, 96 + n * 52.0 + 44);
}

std::vector<AnimFrame> IntervalSchedModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;

    const int n = static_cast<int>(m_segments.size());
    std::vector<int> order(n);
    for (int i = 0; i < n; ++i) order[i] = i;
    std::sort(order.begin(), order.end(), [this](int a, int b) {
        if (m_segments[a].end != m_segments[b].end) return m_segments[a].end < m_segments[b].end;
        return m_segments[a].start < m_segments[b].start;
    });

    m_frames.push_back({ActionType::IV_RESET, 0, 0, 0, 0, {},
                        QString("按结束时间从小到大排序，共 %1 个活动").arg(n),
                        QString("区间调度 = 活动选择问题的经典贪心")});

    for (int i = 0; i < n; ++i) {
        m_frames.push_back({ActionType::IV_SORT, order[i], i, 0, 0, {},
                            QString("排序后第 %1 个：活动 %2，区间 [%3, %4]").arg(i + 1).arg(order[i] + 1)
                                .arg(m_segments[order[i]].start).arg(m_segments[order[i]].end),
                            QString("按结束时间 r 升序")});
    }

    int lastEnd = 0;
    bool first = true;
    int picked = 0;
    for (int i = 0; i < n; ++i) {
        const int idx = order[i];
        const int s = m_segments[idx].start;
        const int e = m_segments[idx].end;
        const bool conflict = !first && s < lastEnd;
        m_frames.push_back({ActionType::IV_PICK, idx, i, e, conflict ? 1 : 0, {},
                            conflict ? QString("活动 %1 与已选活动冲突（%2 < %3），舍弃").arg(idx + 1).arg(s).arg(lastEnd)
                                     : QString("活动 %1 不冲突，选中").arg(idx + 1),
                            conflict ? QString("%1 < %2 ⇒ 冲突，舍弃").arg(s).arg(lastEnd)
                                     : QString("%1 >= %2 ⇒ 选中").arg(s).arg(lastEnd)});
        if (!conflict) {
            lastEnd = e;
            first = false;
            ++picked;
        }
    }

    m_frames.push_back({ActionType::IV_DONE, picked, n, 0, 0, {},
                        QString("贪心结束，共选中 %1 个活动").arg(picked),
                        QString("贪心策略：总选最早结束的活动")});
    return m_frames;
}

void IntervalSchedModule::renderFrame(const AnimFrame& frame) {
    if (m_mathText) {
        setScriptText(m_mathText, frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    }
    if (!m_valid) return;

    if (frame.type == ActionType::IV_RESET) {
        for (auto& seg : m_segments) {
            if (seg.bar) {
                seg.bar->setColor(kIvIdle);
                seg.bar->setMuted(false);
                seg.bar->setZValue(1);
            }
            if (seg.nameText) seg.nameText->setDefaultTextColor(QColor(60, 75, 95));
        }
        if (m_scanText) setScriptText(m_scanText, "");
        if (m_resultText) setScriptText(m_resultText, "");
    } else if (frame.type == ActionType::IV_SORT) {
        const int idx = frame.u;
        if (idx >= 0 && idx < static_cast<int>(m_segments.size())) {
            if (m_segments[idx].bar) {
                m_segments[idx].bar->setColor(kIvScan);
                m_segments[idx].bar->setZValue(2);
                if (m_animated) {
                    playScalePulse(m_segments[idx].bar, 1.06, 200);
                    playRipple(m_segments[idx].bar->pos(), kIvScan, 40.0, 480);
                }
            }
            if (m_segments[idx].nameText) {
                m_segments[idx].nameText->setDefaultTextColor(QColor(200, 120, 40));
            }
        }
        if (m_scanText) {
            setScriptText(m_scanText, QString("排序后第 %1 个：活动 %2").arg(frame.v + 1).arg(idx + 1));
        }
    } else if (frame.type == ActionType::IV_PICK) {
        const int idx = frame.u;
        if (idx < 0 || idx >= static_cast<int>(m_segments.size())) return;
        const bool conflict = frame.val2 == 1;
        if (m_segments[idx].bar) {
            m_segments[idx].bar->setColor(conflict ? kIvReject : kIvPick);
            m_segments[idx].bar->setMuted(conflict);
            m_segments[idx].bar->setZValue(conflict ? 1 : 4);
            if (m_animated) {
                playScalePulse(m_segments[idx].bar, conflict ? 0.96 : 1.10, 240);
                if (conflict) {
                    playRipple(m_segments[idx].bar->pos(), QColor(214, 118, 108), 44.0, 520);
                } else {
                    playRipple(m_segments[idx].bar->pos(), kIvPick, 46.0, 620);
                    playRipple(m_segments[idx].bar->pos(), kIvPick, 30.0, 420);
                }
            }
        }
        if (m_segments[idx].nameText) {
            m_segments[idx].nameText->setDefaultTextColor(conflict ? QColor(150, 155, 165) : QColor(40, 140, 70));
        }
        if (m_scanText) {
            setScriptText(m_scanText, conflict
                ? QString("活动 %1 冲突，舍弃").arg(idx + 1)
                : QString("活动 %1 选中").arg(idx + 1));
            m_scanText->setDefaultTextColor(conflict ? QColor(170, 100, 100) : QColor(40, 140, 70));
        }
    } else if (frame.type == ActionType::IV_DONE) {
        for (auto& seg : m_segments) {
            if (seg.bar) {
                seg.bar->setColor(kIvIdle);
                seg.bar->setMuted(false);
                seg.bar->setZValue(1);
            }
        }
        if (m_resultText) {
            setScriptText(m_resultText, QString("贪心选出 %1 / %2 个活动").arg(frame.u).arg(frame.v));
            m_resultText->setDefaultTextColor(QColor(40, 140, 70));
        }
        if (m_scanText) setScriptText(m_scanText, "");
    }
}

void IntervalSchedModule::reset() {
    m_frames.clear();
    m_segments.clear();
    m_mathText = nullptr;
    m_scanText = nullptr;
    m_resultText = nullptr;
    m_valid = false;
}

void IntervalSchedModule::releaseSceneItems() {
    m_segments.clear();
    m_mathText = nullptr;
    m_scanText = nullptr;
    m_resultText = nullptr;
}
