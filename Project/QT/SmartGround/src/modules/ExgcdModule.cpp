#include "ExgcdModule.h"
#include <QFont>
#include <QFontMetrics>
#include <QRegularExpression>
#include <algorithm>
#include "core/PointerGeometry.h"
#include "ui/UiFonts.h"

static const QColor kExText = QColor(40, 70, 110);
static const QColor kExDim = QColor(140, 152, 168);
static const QColor kExHi = QColor(214, 96, 32);
static const QColor kExOk = QColor(40, 140, 70);
static const QColor kExLink = QColor(150, 166, 188);

static constexpr double kExEqX = 60.0;
static constexpr double kExEqTitleY = 48.0;
static constexpr double kExEqY = 80.0;
static constexpr double kExEqStep = 34.0;
static constexpr double kExGcdTitleY = 190.0;
static constexpr double kExHeadY = 222.0;
static constexpr double kExRowY = 252.0;
static constexpr double kExRowStep = 32.0;
static constexpr double kExLeftX = 100.0;
static constexpr double kExRightX = 500.0;
static constexpr double kExBackX = 60.0;
static constexpr double kExBackTitleGap = 12.0;
static constexpr double kExBackStep = 32.0;

ExgcdModule::ExgcdModule() {}

QString ExgcdModule::getModuleName() const { return "扩展欧几里得"; }
QString ExgcdModule::getDefaultInput() const { return "11914 146415"; }
QString ExgcdModule::getInputHint() const {
    return "第一行输入两个正整数 a b（均不超过 1000000）\n"
           "求 a 在模 b 意义下的乘法逆元（等价于求 a·x + b·y = gcd(a, b)）\n"
           "若 gcd(a, b) ≠ 1 则 a 在模 b 下不存在逆元";
}

QString ExgcdModule::moduleSettingsTitle() const { return "求解模式"; }

QVector<ModuleField> ExgcdModule::moduleSettingsFields() const {
    QVector<ModuleField> fields;
    fields.append(ModuleField::makeSegment("mode", "模式",
                                            {"求模逆元", "求整数解"}, m_inverseMode ? 0 : 1));
    return fields;
}

void ExgcdModule::applyModuleSetting(const QString& key, const ModuleField& field) {
    if (key == "mode") m_inverseMode = (field.intValue == 0);
}

void ExgcdModule::computeChain() {
    m_rows.clear();
    m_chain.clear();

    long long r0 = m_b;
    long long r1 = m_a;
    m_chain.push_back(r0);
    m_chain.push_back(r1);
    while (r1 != 0) {
        const long long q = r0 / r1;
        const long long r2 = r0 % r1;
        ExgcdStepRow row;
        row.dividend = r0;
        row.divisor = r1;
        row.quotient = q;
        row.remainder = r2;
        m_rows.push_back(row);
        m_chain.push_back(r2);
        r0 = r1;
        r1 = r2;
    }
    m_gcdIndex = static_cast<int>(m_chain.size()) - 2;
    m_gcd = m_chain[m_gcdIndex];
}

QString ExgcdModule::fragmentText(int index) const {
    if (index < 2 || index >= static_cast<int>(m_chain.size())) return QString();
    const long long q = m_rows[index - 2].quotient;
    return QString("(%1 - %2 × %3)")
        .arg(m_chain[index - 2]).arg(q).arg(m_chain[index - 1]);
}

QString ExgcdModule::renderTerms(const std::vector<ExgcdTerm>& terms, int substIndex,
                                  int* fragStart) const {
    QString out;
    int start = -1;
    for (const ExgcdTerm& t : terms) {
        if (t.coef == 0) continue;
        const bool isFragment = (substIndex >= 0 && t.index == substIndex);
        const long long absCoef = qAbs(t.coef);
        QString body;
        if (isFragment) {
            body = fragmentText(t.index);
        } else if (t.index >= 0 && t.index < static_cast<int>(m_chain.size())) {
            body = QString::number(m_chain[t.index]);
        } else {
            continue;
        }
        const QString coefPart = (absCoef == 1) ? QString() : QString("%1 × ").arg(absCoef);
        const QString sign = out.isEmpty() ? (t.coef < 0 ? QString("-") : QString())
                                          : (t.coef < 0 ? QString(" - ") : QString(" + "));
        if (isFragment && start < 0) {
            start = out.size() + sign.size() + coefPart.size();
        }
        out += sign + coefPart + body;
    }
    if (fragStart) *fragStart = start;
    if (out.isEmpty()) return "0";
    return out;
}

void ExgcdModule::buildBackSteps() {
    m_backSteps.clear();

    std::vector<ExgcdTerm> terms;
    terms.push_back({m_gcdIndex, 1});

    while (true) {
        int top = -1;
        for (const ExgcdTerm& t : terms) {
            if (t.coef == 0) continue;
            if (t.index > top) top = t.index;
        }
        if (top < 2) break;

        const long long q = m_rows[top - 2].quotient;

        ExgcdBackStep step;
        step.sourceRow = top - 2;

        int fragStart = -1;
        const QString substituted = renderTerms(terms, top, &fragStart);
        step.fragStart = fragStart;
        step.fragPrefixLen = fragStart;
        step.fragment = fragmentText(top);
        step.prefixText = (fragStart >= 0 && fragStart <= substituted.size())
                              ? substituted.left(fragStart) : QString();

        std::vector<ExgcdTerm> next;
        for (const ExgcdTerm& t : terms) {
            if (t.index == top) {
                next.push_back({top - 2, t.coef});
                next.push_back({top - 1, -t.coef * q});
                continue;
            }
            next.push_back(t);
        }

        std::vector<ExgcdTerm> merged;
        for (const ExgcdTerm& t : next) {
            if (t.coef == 0) continue;
            bool found = false;
            for (ExgcdTerm& m : merged) {
                if (m.index == t.index) {
                    m.coef += t.coef;
                    found = true;
                    break;
                }
            }
            if (!found) merged.push_back(t);
        }
        std::vector<ExgcdTerm> cleaned;
        for (const ExgcdTerm& t : merged) {
            if (t.coef == 0) continue;
            cleaned.push_back(t);
        }
        terms = cleaned;

        step.newTerms = substituted;
        step.simplified = renderTerms(terms, -1, nullptr);
        m_backSteps.push_back(step);
    }

    m_x = 0;
    m_y = 0;
    for (const ExgcdTerm& t : terms) {
        if (t.index == 1) m_x = t.coef;
        if (t.index == 0) m_y = -t.coef;
    }
}

void ExgcdModule::buildLayoutBlocks() {
    clearLayoutBlocks();

    m_eqTitle = m_scene->addText("第一步：把同余式变形为不定方程");
    m_eqTitle->setFont(UiFonts::cjk(12, true));
    m_eqTitle->setDefaultTextColor(QColor(60, 75, 95));
    m_eqTitle->setPos(kExEqX, kExEqTitleY);

    m_eqLines.clear();
    m_eqArrows.clear();
    for (int i = 0; i < 3; ++i) {
        QGraphicsTextItem* t = m_scene->addText("");
        t->setFont(UiFonts::mixed(14, true));
        t->setDefaultTextColor(kExText);
        t->setPos(kExEqX, kExEqY + i * kExEqStep);
        m_eqLines.push_back(t);

        QGraphicsTextItem* arrow = m_scene->addText("↓");
        arrow->setFont(UiFonts::mixed(12, true));
        arrow->setDefaultTextColor(kExLink);
        arrow->setPos(kExEqX - 30, kExEqY + i * kExEqStep - 4);
        arrow->setOpacity(0);
        m_eqArrows.push_back(arrow);
    }

    m_gcdTitle = m_scene->addText("第二步：辗转相除求 gcd(a, b)");
    m_gcdTitle->setFont(UiFonts::cjk(12, true));
    m_gcdTitle->setDefaultTextColor(QColor(60, 75, 95));
    m_gcdTitle->setPos(kExEqX, kExGcdTitleY);

    m_gcdHeadLeft = m_scene->addText("取余");
    m_gcdHeadLeft->setFont(UiFonts::cjk(11, true));
    m_gcdHeadLeft->setDefaultTextColor(kExDim);
    m_gcdHeadLeft->setPos(kExLeftX, kExHeadY);

    m_gcdHeadRight = m_scene->addText("整除式");
    m_gcdHeadRight->setFont(UiFonts::cjk(11, true));
    m_gcdHeadRight->setDefaultTextColor(kExDim);
    m_gcdHeadRight->setPos(kExRightX, kExHeadY);

    m_leftLines.clear();
    m_rightLines.clear();
    for (size_t r = 0; r < m_rows.size(); ++r) {
        const double y = kExRowY + static_cast<double>(r) * kExRowStep;
        QGraphicsTextItem* l = m_scene->addText("");
        l->setFont(UiFonts::mono(13));
        l->setDefaultTextColor(kExText);
        l->setPos(kExLeftX, y);
        m_leftLines.push_back(l);

        QGraphicsTextItem* rr = m_scene->addText("");
        rr->setFont(UiFonts::mono(13));
        rr->setDefaultTextColor(kExText);
        rr->setPos(kExRightX, y);
        m_rightLines.push_back(rr);
    }

    const double backTitleY = kExRowY + static_cast<double>(m_rows.size()) * kExRowStep + kExBackTitleGap;
    m_backTitle = m_scene->addText("第三步：递归回代，逐层展开");
    m_backTitle->setFont(UiFonts::cjk(12, true));
    m_backTitle->setDefaultTextColor(QColor(60, 75, 95));
    m_backTitle->setPos(kExEqX, backTitleY);

    m_backLines.clear();
    for (size_t r = 0; r < m_backSteps.size(); ++r) {
        QGraphicsTextItem* t = m_scene->addText("");
        t->setFont(UiFonts::mono(13));
        t->setDefaultTextColor(kExText);
        t->setPos(kExBackX, backTitleY + 32 + static_cast<double>(r) * kExBackStep);
        m_backLines.push_back(t);
    }

    const double backEndY = backTitleY + 32 + static_cast<double>(m_backSteps.size()) * kExBackStep;

    m_noteText = createScriptText(m_scene, "");
    m_noteText->setFont(UiFonts::mixed(13, true));
    m_noteText->setDefaultTextColor(kExHi);
    m_noteText->setPos(kExEqX, backEndY + 16);

    m_resultText = createScriptText(m_scene, "");
    m_resultText->setFont(UiFonts::mixed(14, true));
    m_resultText->setDefaultTextColor(kExOk);
    m_resultText->setPos(kExEqX, backEndY + 50);

    m_ptrRow = new PointerBlock("行", QColor(210, 80, 55));
    m_ptrRow->setOpacity(0);
    m_ptrRow->setDirection(PointerBlock::Right);
    m_ptrRow->setShaftLength(22.0);
    m_ptrRow->setAnchorOffset(QPointF(-24.0, 0.0));
    m_ptrRow->setLabelOffset(QPointF(-44.0, -10.0));
    m_scene->addItem(m_ptrRow);
}

void ExgcdModule::clearLayoutBlocks() {
    m_eqLines.clear();
    m_eqArrows.clear();
    m_leftLines.clear();
    m_rightLines.clear();
    m_backLines.clear();
    m_eqTitle = nullptr;
    m_gcdTitle = nullptr;
    m_gcdHeadLeft = nullptr;
    m_gcdHeadRight = nullptr;
    m_backTitle = nullptr;
    m_noteText = nullptr;
    m_resultText = nullptr;
    m_ptrRow = nullptr;
}

void ExgcdModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;

    const QStringList tokens = inputData.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    bool ok = false;
    long long a = 0;
    long long b = 0;
    if (tokens.size() >= 2) {
        a = tokens[0].toLongLong(&ok);
        const bool okA = ok && a >= 1 && a <= 1000000;
        b = tokens[1].toLongLong(&ok);
        m_valid = okA && ok && b >= 1 && b <= 1000000;
    }
    if (!m_valid) return;

    m_a = a;
    m_b = b;

    m_mathText = createScriptText(scene, m_inverseMode
                                    ? QString("求 %1 在模 %2 意义下的乘法逆元").arg(a).arg(b)
                                    : QString("扩展欧几里得：求整数 x, y 使 %1·x + %2·y = gcd(%1, %2)").arg(a).arg(b));
    m_mathText->setFont(UiFonts::mixed(15, true));
    m_mathText->setDefaultTextColor(kExText);
    m_mathText->setPos(40, 14);

    computeChain();
    buildBackSteps();
    buildLayoutBlocks();
}

std::vector<AnimFrame> ExgcdModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;

    m_frames.push_back({ActionType::EX_RESET, 0, 0, 0, 0, {},
                        QString("目标：%1·x ≡ 1 (mod %2)").arg(m_a).arg(m_b),
                        QString("把同余式变形为不定方程")});

    const QStringList eqs = {
        QString("%1·x ≡ 1 (mod %2)").arg(m_a).arg(m_b),
        QString("%1·x = %2·y + 1").arg(m_a).arg(m_b),
        QString("%1·x - %2·y = 1").arg(m_a).arg(m_b)
    };
    const QStringList eqNotes = {
        QString("原式：%1·x ≡ 1 (mod %2)").arg(m_a).arg(m_b),
        QString("去掉同余号：%1·x = %2·y + 1").arg(m_a).arg(m_b),
        QString("移项：%1·x - %2·y = 1").arg(m_a).arg(m_b)
    };
    for (int i = 0; i < 3; ++i) {
        m_frames.push_back({ActionType::EX_EQUATION, i, 0, 0, 0, {},
                            eqNotes[i], QString("同余式 → 不定方程")});
    }

    for (size_t r = 0; r < m_rows.size(); ++r) {
        m_frames.push_back({ActionType::EX_RECURSE, static_cast<int>(r),
                            static_cast<int>(m_rows[r].dividend),
                            static_cast<int>(m_rows[r].divisor),
                            static_cast<int>(m_rows[r].quotient), {},
                            QString("%1 = %2 × %3 + %4")
                                .arg(m_rows[r].dividend).arg(m_rows[r].quotient)
                                .arg(m_rows[r].divisor).arg(m_rows[r].remainder),
                            QString("辗转相除")});
    }

    m_frames.push_back({ActionType::EX_BASE, 0, static_cast<int>(m_gcd), 0, 0, {},
                        QString("gcd(%1, %2) = %3，所以%4")
                            .arg(m_a).arg(m_b).arg(m_gcd)
                            .arg(m_gcd == 1 ? "存在逆元" : "不存在逆元"),
                        m_gcd == 1 ? QString("gcd = 1，所以逆元存在")
                                   : QString("gcd ≠ 1，逆元不存在")});

    for (size_t s = 0; s < m_backSteps.size(); ++s) {
        const ExgcdBackStep& step = m_backSteps[s];
        const QString text = (s == 0) ? QString("1 = %1").arg(step.simplified)
                                       : QString("1 = %1 = %2").arg(step.newTerms).arg(step.simplified);
        const int row = step.sourceRow;
        const QString note = (row >= 0 && row < static_cast<int>(m_rows.size()))
                                 ? QString("回代：把 %1 = %2 × %3 + %4 中的余数 %4 展开")
                                       .arg(m_rows[row].dividend).arg(m_rows[row].quotient)
                                       .arg(m_rows[row].divisor).arg(m_rows[row].remainder)
                                 : QString("回代展开");
        m_frames.push_back({ActionType::EX_BACKTRACE, static_cast<int>(s), 0, 0, 0, {}, text, note});
    }

    m_frames.push_back({ActionType::EX_DONE, static_cast<int>(m_x), static_cast<int>(m_y), 0, 0, {},
                        m_inverseMode
                            ? QString("%1·(%2) - %3·(%4) = 1，即 %1 在模 %3 意义下的乘法逆元是 %2")
                                .arg(m_a).arg(m_x).arg(m_b).arg(m_y)
                            : QString("%1·(%2) + %3·(%4) = gcd(%1, %3) = %5")
                                .arg(m_a).arg(m_x).arg(m_b).arg(-m_y).arg(m_gcd),
                        QString("最终结果")});
    return m_frames;
}

void ExgcdModule::renderFrame(const AnimFrame& frame) {
    if (m_mathText) {
        setScriptText(m_mathText, frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    }
    if (!m_valid) return;

    if (frame.type == ActionType::EX_RESET) {
        for (size_t i = 0; i < m_eqLines.size(); ++i) {
            m_eqLines[i]->setPlainText("");
            m_eqLines[i]->setDefaultTextColor(kExDim);
        }
        for (auto* t : m_eqArrows) t->setOpacity(0);
        for (auto* t : m_leftLines) { t->setPlainText(""); t->setDefaultTextColor(kExText); }
        for (auto* t : m_rightLines) { t->setPlainText(""); t->setDefaultTextColor(kExText); }
        for (auto* t : m_backLines) { t->setPlainText(""); t->setDefaultTextColor(kExText); }
        if (m_ptrRow) m_ptrRow->setOpacity(0);
        if (m_noteText) { setScriptText(m_noteText, ""); m_noteText->setDefaultTextColor(kExHi); }
        if (m_resultText) { setScriptText(m_resultText, ""); m_resultText->setDefaultTextColor(kExOk); }
        return;
    }

    if (frame.type == ActionType::EX_EQUATION) {
        const int idx = frame.u;
        const QStringList eqs = {
            QString("%1·x ≡ 1 (mod %2)").arg(m_a).arg(m_b),
            QString("%1·x = %2·y + 1").arg(m_a).arg(m_b),
            QString("%1·x - %2·y = 1").arg(m_a).arg(m_b)
        };
        if (idx >= 0 && idx < static_cast<int>(m_eqLines.size())) {
            QGraphicsTextItem* target = m_eqLines[idx];
            const QPointF dst = target->pos();
            const QString text = eqs[idx];
            if (idx == 0) {
                target->setPlainText(text);
                target->setDefaultTextColor(kExText);
            } else {
                target->setDefaultTextColor(kExHi);
                if (m_animated) {
                    playTextFlightAligned(text, dst + QPointF(90.0, -26.0), dst, 14, kExHi, 460,
                                          [target, text]() {
                                              if (!target) return;
                                              target->setPlainText(text);
                                          });
                } else {
                    target->setPlainText(text);
                }
            }
            if (idx > 0 && idx - 1 < static_cast<int>(m_eqArrows.size())) {
                m_eqArrows[idx - 1]->setOpacity(1);
            }
        }
        if (m_noteText) {
            setScriptText(m_noteText, frame.explanation);
            m_noteText->setDefaultTextColor(kExHi);
        }
        return;
    }

    if (frame.type == ActionType::EX_RECURSE) {
        const int r = frame.u;
        if (r >= 0 && r < static_cast<int>(m_leftLines.size())) {
            const QString leftText = QString("%1 % %2 = %3")
                                         .arg(m_rows[r].dividend).arg(m_rows[r].divisor)
                                         .arg(m_rows[r].remainder);
            const QString rightText = QString("%1 = %2 × %3 + %4")
                                          .arg(m_rows[r].dividend).arg(m_rows[r].quotient)
                                          .arg(m_rows[r].divisor).arg(m_rows[r].remainder);
            m_leftLines[r]->setPlainText(leftText);
            m_rightLines[r]->setPlainText(rightText);
            m_leftLines[r]->setDefaultTextColor(kExHi);
            m_rightLines[r]->setDefaultTextColor(kExHi);
            if (m_animated) {
                const QPointF dst = m_rightLines[r]->pos();
                playTextFlightAligned(rightText, dst + QPointF(120.0, -22.0), dst, 13, kExHi, 420);
                playRipple(m_leftLines[r]->pos() + QPointF(6.0, 8.0), kExHi, 46.0, 620);
            }
        }
        if (m_ptrRow && r >= 0 && r < static_cast<int>(m_leftLines.size())) {
            QGraphicsTextItem* anchor = m_leftLines[r];
            const double midY = anchor->boundingRect().center().y();
            m_ptrRow->setOpacity(1);
            retargetPointerTo(m_ptrRow, anchor, QPointF(-24.0, midY));
        }        if (m_noteText) {
            setScriptText(m_noteText, frame.explanation);
            m_noteText->setDefaultTextColor(kExHi);
        }
        return;
    }

    if (frame.type == ActionType::EX_BASE) {
        if (m_ptrRow) m_ptrRow->setOpacity(0);
        if (m_noteText) {
            setScriptText(m_noteText, frame.explanation);
            m_noteText->setDefaultTextColor(m_gcd == 1 ? kExOk : kExHi);
        }
        return;
    }

    if (frame.type == ActionType::EX_BACKTRACE) {
        const int s = frame.u;
        if (s >= 0 && s < static_cast<int>(m_backLines.size())) {
            QGraphicsTextItem* target = m_backLines[s];
            const ExgcdBackStep& step = m_backSteps[s];
            const QPointF dst = target->pos();

            if (s == 0) {
                target->setPlainText(QString("1 = %1").arg(step.simplified));
                target->setDefaultTextColor(kExHi);
            } else {
                const ExgcdBackStep& prev = m_backSteps[s - 1];
                const QString prevText = QString("1 = %1").arg(prev.simplified);
                const QPointF prevPos = m_backLines[s - 1]->pos();
                target->setPlainText(prevText);
                target->setDefaultTextColor(kExText);
                if (m_animated) {
                    playTextFlightAligned(prevText, prevPos + QPointF(0.0, 2.0), dst, 13, kExText, 400,
                                          [target, prevText]() {
                                              if (!target) return;
                                              target->setPlainText(prevText);
                                          });
                }
            }

            const int row = step.sourceRow;
            if (row >= 0 && row < static_cast<int>(m_rightLines.size())) {
                const QPointF src = m_rightLines[row]->pos();
                const QString frag = step.fragment;
                const QString fullPrefix = QString("1 = %1").arg(step.prefixText);
                const QFontMetrics fm(target->font());
                const QPointF fragDst = dst + QPointF(fm.horizontalAdvance(fullPrefix), 0.0);
                if (m_animated) {
                    playTextFlightAligned(frag, src, fragDst, 13, kExHi, 520,
                                          [target, step]() {
                                              if (!target) return;
                                              target->setPlainText(QString("1 = %1").arg(step.newTerms));
                                              target->setDefaultTextColor(kExText);
                                          });
                } else {
                    target->setPlainText(QString("1 = %1").arg(step.newTerms));
                    target->setDefaultTextColor(kExText);
                }
            }
        }
        if (m_noteText) {
            setScriptText(m_noteText, frame.mathText);
            m_noteText->setDefaultTextColor(kExHi);
        }
        return;
    }

    if (frame.type == ActionType::EX_DONE) {
        if (m_ptrRow) m_ptrRow->setOpacity(0);
        if (m_noteText) {
            setScriptText(m_noteText, frame.explanation);
            m_noteText->setDefaultTextColor(kExOk);
        }
        if (m_resultText) {
            if (m_inverseMode) {
                setScriptText(m_resultText, 
                    QString("结论：%1 在模 %2 意义下的乘法逆元是 %3")
                        .arg(m_a).arg(m_b).arg(frame.u));
            } else {
                setScriptText(m_resultText, 
                    QString("结论：%1·(%2) + %3·(%4) = %5")
                        .arg(m_a).arg(frame.u).arg(m_b).arg(frame.v).arg(m_gcd));
            }
            m_resultText->setDefaultTextColor(kExOk);
        }
    }
}

void ExgcdModule::reset() {
    m_frames.clear();
    m_rows.clear();
    m_chain.clear();
    m_backSteps.clear();
    clearLayoutBlocks();
    m_mathText = nullptr;
    m_a = 0;
    m_b = 0;
    m_gcd = 0;
    m_gcdIndex = -1;
    m_x = 0;
    m_y = 0;
    m_valid = false;
}

void ExgcdModule::releaseSceneItems() {
    m_rows.clear();
    m_chain.clear();
    m_backSteps.clear();
    clearLayoutBlocks();
    m_mathText = nullptr;
}
