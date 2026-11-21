#include "IntervalSchedModule.h"
#include "render/SegmentBar.h"
#include <QFont>
#include <QRegularExpression>
#include <algorithm>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include "ui/UiFonts.h"

static const QColor kIvIdle = QColor(214, 224, 238);
static const QColor kIvScan = QColor(255, 205, 130);
static const QColor kIvPick = QColor(140, 205, 165);
static const QColor kIvReject = QColor(228, 231, 236);
static const QColor kIvConflict = QColor(226, 96, 62);
static const QColor kIvNaive = QColor(214, 150, 150);

IntervalSchedModule::IntervalSchedModule() {}

QString IntervalSchedModule::getModuleName() const { return "区间调度"; }
QString IntervalSchedModule::getDefaultInput() const {
    return "4\n"
           "0 4\n"
           "1 2\n"
           "3 6\n"
           "2 5";
}
QString IntervalSchedModule::getInputHint() const {
    return "第一行输入一个正整数 n，表示活动个数（n <= 8）\n"
           "接下来 n 行，每行两个整数 l, r，表示活动的开始与结束时间（l < r）\n"
           "贪心策略：按结束时间从小到大排序，每次选第一个不冲突的活动";
}
QString IntervalSchedModule::moduleSettingsTitle() const { return "区间调度选项"; }

QVector<ModuleField> IntervalSchedModule::moduleSettingsFields() const {
    QVector<ModuleField> fields;
    fields.append(ModuleField::makeToggle("counter", "显示反例对照", m_showCounterExample));
    return fields;
}

void IntervalSchedModule::applyModuleSetting(const QString& key, const ModuleField& field) {
    if (key == "counter") m_showCounterExample = field.boolValue;
}

void IntervalSchedModule::clearBands() {
    for (QGraphicsRectItem* band : m_bands) {
        if (!band) continue;
        band->setVisible(false);
        band->setZValue(0);
    }
}

void IntervalSchedModule::addBand(double x1, double x2, double y, double height, QColor color) {
    if (!m_scene) return;
    QGraphicsRectItem* band = new QGraphicsRectItem(x1, y, x2 - x1, height);
    QColor fill = color;
    fill.setAlpha(70);
    band->setBrush(fill);
    band->setPen(QPen(color, 1.2, Qt::DashLine));
    band->setZValue(2);
    band->setAcceptHoverEvents(false);
    m_scene->addItem(band);
    m_bands.push_back(band);
}

void IntervalSchedModule::buildResultSlot() {
    m_greedyTitle = m_scene->addText("贪心解（按结束时间选）");
    m_greedyTitle->setFont(UiFonts::cjk(11, true));
    m_greedyTitle->setDefaultTextColor(QColor(40, 140, 70));
    m_greedyTitle->setPos(0, 0);

    m_naiveTitle = m_scene->addText("错误解（按开始时间选）");
    m_naiveTitle->setFont(UiFonts::cjk(11, true));
    m_naiveTitle->setDefaultTextColor(QColor(180, 70, 70));
    m_naiveTitle->setPos(0, 0);

    for (int i = 0; i < 8; ++i) {
        QGraphicsTextItem* g = m_scene->addText("");
        g->setFont(UiFonts::mono(12, true));
        g->setDefaultTextColor(QColor(40, 140, 70));
        m_greedyLabels.push_back(g);

        QGraphicsTextItem* n = m_scene->addText("");
        n->setFont(UiFonts::mono(12, true));
        n->setDefaultTextColor(QColor(180, 70, 70));
        m_naiveLabels.push_back(n);
    }
}
void IntervalSchedModule::clearResultSlot() {
    for (QGraphicsTextItem* t : m_greedyLabels) t->setPlainText("");
    for (QGraphicsTextItem* t : m_naiveLabels) t->setPlainText("");
}

void IntervalSchedModule::setResultSlot(int index, const QString& label, QColor color) {
    if (index < 0) return;
    if (index < static_cast<int>(m_greedyLabels.size())) {
        m_greedyLabels[index]->setPlainText(label);
        m_greedyLabels[index]->setDefaultTextColor(color);
        if (m_animated) playFadeItem(m_greedyLabels[index], 0.0, 1.0, 320);
    }
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

    const double left = m_timelineLeft;
    const double width = m_timelineWidth;
    const double scale = width / qMax(1, maxEnd);
    m_timelineScale = scale;

    for (int i = 0; i < n; ++i) {
        const double y = 110.0 + i * 46.0;
        const double x1 = ranges[i].first * scale;
        const double x2 = ranges[i].second * scale;
        SegmentBar* bar = new SegmentBar(x1, x2, 26);
        bar->setColor(kIvIdle);
        bar->setBar(QString("活动 %1").arg(i + 1));
        bar->setPos(left + x1, y);
        scene->addItem(bar);

        QGraphicsTextItem* name = scene->addText(QString("活动 %1").arg(i + 1));
        name->setFont(UiFonts::cjk(11, true));
        name->setDefaultTextColor(QColor(60, 75, 95));
        name->setPos(left - 96, y + 2);
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

    const double timelineBottom = 96 + n * 46.0;

    buildResultSlot();

    m_scanText = createScriptText(scene, "");
    m_scanText->setFont(UiFonts::mono(13, true));
    m_scanText->setDefaultTextColor(QColor(200, 100, 40));
    m_scanText->setPos(m_timelineLeft, timelineBottom + 8);

    const double greedyY = timelineBottom + 58.0;
    m_greedyTitle->setPos(0, greedyY - 24);
    m_naiveTitle->setPos(0, greedyY + 20);
    for (size_t i = 0; i < m_greedyLabels.size(); ++i) {
        m_greedyLabels[i]->setPos(m_timelineLeft + i * 96.0, greedyY);
        m_naiveLabels[i]->setPos(m_timelineLeft + i * 96.0, greedyY + 44);
    }

    m_resultText = createScriptText(scene, "");
    m_resultText->setFont(UiFonts::cjk(13, true));
    m_resultText->setDefaultTextColor(QColor(40, 140, 70));
    m_resultText->setPos(m_timelineLeft, greedyY + 86);

    m_counterText = createScriptText(scene, "");
    m_counterText->setFont(UiFonts::cjk(12, true));
    m_counterText->setDefaultTextColor(QColor(180, 70, 70));
    m_counterText->setPos(m_timelineLeft, greedyY + 118);
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
    std::vector<int> greedyList;
    for (int i = 0; i < n; ++i) {
        const int idx = order[i];
        const int s = m_segments[idx].start;
        const int e = m_segments[idx].end;
        const bool conflict = !first && s < lastEnd;
        m_frames.push_back({ActionType::IV_COMPARE, idx, i, e, conflict ? 1 : 0, {},
                            conflict ? QString("活动 %1 与已选活动冲突（%2 < %3），舍弃").arg(idx + 1).arg(s).arg(lastEnd)
                                     : QString("活动 %1 不冲突，选中").arg(idx + 1),
                            conflict ? QString("%1 < %2 ⇒ 冲突，舍弃").arg(s).arg(lastEnd)
                                     : QString("%1 >= %2 ⇒ 选中").arg(s).arg(lastEnd)});
        if (!conflict) {
            greedyList.push_back(idx);
            m_frames.push_back({ActionType::IV_PICK, idx, i, e, static_cast<int>(greedyList.size()) - 1, {},
                                QString("活动 %1 入选贪心解（第 %2 个）").arg(idx + 1).arg(greedyList.size()),
                                QString("贪心解 = { %1 }").arg(greedyList.size())});
            lastEnd = e;
            first = false;
            ++picked;
        } else {
            m_frames.push_back({ActionType::IV_REJECT_BAND, idx, i, s, lastEnd, {},
                                QString("活动 %1 被舍弃：与上一个已选活动在时间轴上重叠").arg(idx + 1),
                                QString("重叠区间 [%1, %2] 标红").arg(s).arg(lastEnd)});
            m_frames.push_back({ActionType::IV_REJECT, idx, i, 0, 0, {},
                                QString("活动 %1 标记为出局").arg(idx + 1),
                                QString("舍弃")});
        }
    }

    m_frames.push_back({ActionType::IV_DONE, picked, n, 0, 0, {},
                        QString("贪心结束，共选中 %1 个活动").arg(picked),
                        QString("贪心策略：总选最早结束的活动")});

    if (m_showCounterExample) {
        std::vector<int> naiveOrder(n);
        for (int i = 0; i < n; ++i) naiveOrder[i] = i;
        std::sort(naiveOrder.begin(), naiveOrder.end(), [this](int a, int b) {
            if (m_segments[a].start != m_segments[b].start) return m_segments[a].start < m_segments[b].start;
            return m_segments[a].end < m_segments[b].end;
        });

        m_frames.push_back({ActionType::IV_PHASE_RESET, 0, 0, 0, 0, {},
                            QString("反证：改用错误准则「按开始时间最早选」，看能得到几个活动"),
                            QString("错误准则：按 l 升序")});

        int naiveLastEnd = 0;
        bool naiveFirst = true;
        int naivePicked = 0;
        std::vector<int> naiveList;
        for (int i = 0; i < n; ++i) {
            const int idx = naiveOrder[i];
            const int s = m_segments[idx].start;
            const int e = m_segments[idx].end;
            const bool conflict = !naiveFirst && s < naiveLastEnd;
            m_frames.push_back({ActionType::IV_COUNTER_COMPARE, idx, i, e, conflict ? 1 : 0, {},
                                conflict ? QString("错误准则下活动 %1 冲突，舍弃").arg(idx + 1)
                                         : QString("错误准则下活动 %1 入选").arg(idx + 1),
                                conflict ? QString("舍弃") : QString("入选")});
            if (!conflict) {
                naiveList.push_back(idx);
                m_frames.push_back({ActionType::IV_COUNTER_PICK, idx, i, e, static_cast<int>(naiveList.size()) - 1, {},
                                    QString("错误准则下活动 %1 入选（共 %2 个）").arg(idx + 1).arg(naiveList.size()),
                                    QString("错误解")});
                naiveLastEnd = e;
                naiveFirst = false;
                ++naivePicked;
            }
        }

        const bool worse = naivePicked < picked;
        m_frames.push_back({ActionType::IV_COUNTER_DONE, naivePicked, n, worse ? 1 : 0, 1, {},
                            worse ? QString("反证成立：错误准则只选出 %1 个活动，少于贪心的 %2 个 ⇒ 贪心准则更优")
                                  : QString("本例两种准则结果相同，换一个反例才能体现差异"),
                            worse ? QString("贪心 %1 > 错误 %2").arg(picked).arg(naivePicked)
                                  : QString("结果相同")});
    }
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
        clearBands();
        clearResultSlot();
        if (m_scanText) setScriptText(m_scanText, "");
        if (m_resultText) {
            setScriptText(m_resultText, "");
            m_resultText->setDefaultTextColor(QColor(40, 140, 70));
        }
        if (m_counterText) {
            setScriptText(m_counterText, "");
            m_counterText->setDefaultTextColor(QColor(180, 70, 70));
        }
    } else if (frame.type == ActionType::IV_SORT) {
        const int idx = frame.u;
        if (idx >= 0 && idx < static_cast<int>(m_segments.size())) {
            if (m_segments[idx].bar) {
                m_segments[idx].bar->setColor(kIvScan);
                m_segments[idx].bar->setZValue(3);
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
    } else if (frame.type == ActionType::IV_COMPARE) {
        const int idx = frame.u;
        if (idx < 0 || idx >= static_cast<int>(m_segments.size())) return;
        const bool conflict = frame.val2 == 1;
        if (m_segments[idx].bar) {
            m_segments[idx].bar->setColor(conflict ? kIvReject : kIvScan);
            m_segments[idx].bar->setMuted(conflict);
            m_segments[idx].bar->setZValue(conflict ? 1 : 3);
            if (m_animated) {
                playScalePulse(m_segments[idx].bar, conflict ? 0.96 : 1.06, 220);
            }
        }
        if (m_scanText) {
            setScriptText(m_scanText, conflict
                ? QString("活动 %1 冲突，舍弃").arg(idx + 1)
                : QString("活动 %1 不冲突").arg(idx + 1));
            m_scanText->setDefaultTextColor(conflict ? QColor(170, 100, 100) : QColor(200, 120, 40));
        }
    } else if (frame.type == ActionType::IV_REJECT_BAND) {
        const int idx = frame.u;
        if (idx < 0 || idx >= static_cast<int>(m_segments.size())) return;
        const int s = frame.val1;
        const int e = frame.val2;
        const double scale = m_timelineScale;
        const double y = 110.0 + idx * 46.0 - 3.0;
        addBand(m_timelineLeft + s * scale, m_timelineLeft + e * scale, y, 32.0, kIvConflict);
        if (m_segments[idx].bar) {
            m_segments[idx].bar->setColor(kIvConflict);
            m_segments[idx].bar->setMuted(true);
            m_segments[idx].bar->setZValue(2);
            if (m_animated) {
                playScalePulse(m_segments[idx].bar, 0.94, 220);
                playRipple(m_segments[idx].bar->pos(), kIvConflict, 42.0, 520);
            }
        }
        if (m_segments[idx].nameText) {
            m_segments[idx].nameText->setDefaultTextColor(QColor(170, 100, 100));
        }
        if (m_scanText) {
            setScriptText(m_scanText, QString("活动 %1：与已选活动在时间轴上重叠").arg(idx + 1));
            m_scanText->setDefaultTextColor(QColor(190, 80, 70));
        }
    } else if (frame.type == ActionType::IV_REJECT) {
        const int idx = frame.u;
        if (idx < 0 || idx >= static_cast<int>(m_segments.size())) return;
        if (m_segments[idx].bar) {
            m_segments[idx].bar->setColor(kIvReject);
            m_segments[idx].bar->setMuted(true);
            m_segments[idx].bar->setZValue(1);
        }
        if (m_segments[idx].nameText) {
            m_segments[idx].nameText->setDefaultTextColor(QColor(150, 155, 165));
        }
    } else if (frame.type == ActionType::IV_PICK) {
        const int idx = frame.u;
        if (idx < 0 || idx >= static_cast<int>(m_segments.size())) return;
        if (m_segments[idx].bar) {
            m_segments[idx].bar->setColor(kIvPick);
            m_segments[idx].bar->setMuted(false);
            m_segments[idx].bar->setZValue(5);
            if (m_animated) {
                playScalePulse(m_segments[idx].bar, 1.10, 240);
                playRipple(m_segments[idx].bar->pos(), kIvPick, 46.0, 620);
                playRipple(m_segments[idx].bar->pos(), kIvPick, 30.0, 420);
            }
        }
        if (m_segments[idx].nameText) {
            m_segments[idx].nameText->setDefaultTextColor(QColor(40, 140, 70));
        }
        setResultSlot(frame.val1, QString("活动 %1").arg(idx + 1), QColor(40, 140, 70));
        if (m_scanText) {
            setScriptText(m_scanText, QString("活动 %1 选中").arg(idx + 1));
            m_scanText->setDefaultTextColor(QColor(40, 140, 70));
        }
    } else if (frame.type == ActionType::IV_PHASE_RESET) {
        clearBands();
        for (auto& seg : m_segments) {
            if (seg.bar) {
                seg.bar->setColor(kIvIdle);
                seg.bar->setMuted(false);
                seg.bar->setZValue(1);
            }
            if (seg.nameText) seg.nameText->setDefaultTextColor(QColor(60, 75, 95));
        }
        for (size_t i = 0; i < m_greedyLabels.size(); ++i) {
            m_greedyLabels[i]->setDefaultTextColor(QColor(40, 140, 70));
        }
        if (m_scanText) {
            setScriptText(m_scanText, QString("反证模式：改按开始时间排序"));
            m_scanText->setDefaultTextColor(QColor(180, 70, 70));
        }
    } else if (frame.type == ActionType::IV_COUNTER_COMPARE) {
        const int idx = frame.u;
        if (idx < 0 || idx >= static_cast<int>(m_segments.size())) return;
        const bool conflict = frame.val2 == 1;
        if (m_segments[idx].bar) {
            m_segments[idx].bar->setColor(conflict ? kIvReject : kIvNaive);
            m_segments[idx].bar->setMuted(conflict);
            m_segments[idx].bar->setZValue(conflict ? 1 : 3);
            if (m_animated) playScalePulse(m_segments[idx].bar, conflict ? 0.96 : 1.06, 220);
        }
        if (m_segments[idx].nameText) {
            m_segments[idx].nameText->setDefaultTextColor(conflict ? QColor(150, 155, 165) : QColor(180, 70, 70));
        }
        if (m_scanText) {
            setScriptText(m_scanText, conflict
                ? QString("错误准则：活动 %1 冲突，舍弃").arg(idx + 1)
                : QString("错误准则：活动 %1 不冲突").arg(idx + 1));
            m_scanText->setDefaultTextColor(QColor(180, 70, 70));
        }
    } else if (frame.type == ActionType::IV_COUNTER_PICK) {
        const int idx = frame.u;
        if (idx < 0 || idx >= static_cast<int>(m_segments.size())) return;
        if (m_segments[idx].bar) {
            m_segments[idx].bar->setColor(kIvNaive);
            m_segments[idx].bar->setMuted(false);
            m_segments[idx].bar->setZValue(5);
            if (m_animated) {
                playScalePulse(m_segments[idx].bar, 1.08, 240);
                playRipple(m_segments[idx].bar->pos(), kIvNaive, 40.0, 560);
            }
        }
        if (m_segments[idx].nameText) {
            m_segments[idx].nameText->setDefaultTextColor(QColor(180, 70, 70));
        }
        if (frame.val1 >= 0 && frame.val1 < static_cast<int>(m_naiveLabels.size())) {
            m_naiveLabels[frame.val1]->setPlainText(QString("活动 %1").arg(idx + 1));
            m_naiveLabels[frame.val1]->setDefaultTextColor(QColor(180, 70, 70));
            if (m_animated) playFadeItem(m_naiveLabels[frame.val1], 0.0, 1.0, 320);
        }
        if (m_scanText) {
            setScriptText(m_scanText, QString("错误准则：活动 %1 入选").arg(idx + 1));
            m_scanText->setDefaultTextColor(QColor(180, 70, 70));
        }
    } else if (frame.type == ActionType::IV_COUNTER_DONE) {
        const bool worse = frame.val2 == 1;
        if (m_resultText) {
            if (worse) {
                setScriptText(m_resultText, QString("反证成立：错误准则只选出 %1 / %2 个活动").arg(frame.u).arg(frame.v));
                m_resultText->setDefaultTextColor(QColor(180, 70, 70));
            } else {
                setScriptText(m_resultText, QString("错误准则选出 %1 / %2 个活动").arg(frame.u).arg(frame.v));
                m_resultText->setDefaultTextColor(QColor(180, 70, 70));
            }
        }
        if (m_counterText) {
            setScriptText(m_counterText, worse
                ? QString("按开始时间选只能拿到 %1 个，按结束时间选能拿到 %2 个 ⇒ 必须按结束时间贪心")
                      .arg(frame.u).arg(frame.v)
                : QString("本例两种准则结果相同，换一个反例才能体现差异"));
            m_counterText->setDefaultTextColor(QColor(180, 70, 70));
        }
    } else if (frame.type == ActionType::IV_DONE) {
        if (m_resultText) {
            setScriptText(m_resultText, QString("贪心选出 %1 / %2 个活动").arg(frame.u).arg(frame.v));
            m_resultText->setDefaultTextColor(QColor(40, 140, 70));
        }
        if (m_counterText) {
            setScriptText(m_counterText, "");
        }
    }
}

void IntervalSchedModule::reset() {
    m_frames.clear();
    m_segments.clear();
    m_bands.clear();
    m_greedyLabels.clear();
    m_naiveLabels.clear();
    m_mathText = nullptr;
    m_scanText = nullptr;
    m_resultText = nullptr;
    m_counterText = nullptr;
    m_greedyTitle = nullptr;
    m_naiveTitle = nullptr;
    m_valid = false;
}

void IntervalSchedModule::releaseSceneItems() {
    m_segments.clear();
    m_bands.clear();
    m_greedyLabels.clear();
    m_naiveLabels.clear();
    m_mathText = nullptr;
    m_scanText = nullptr;
    m_resultText = nullptr;
    m_counterText = nullptr;
    m_greedyTitle = nullptr;
    m_naiveTitle = nullptr;
}
