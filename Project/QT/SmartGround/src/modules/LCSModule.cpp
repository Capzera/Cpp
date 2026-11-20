#include "LCSModule.h"
#include <QFont>
#include <QRegularExpression>
#include <algorithm>
#include "core/PointerGeometry.h"
#include "ui/UiFonts.h"

static const QColor kLcIdle = QColor(252, 252, 254);
static const QColor kLcSame = QColor(198, 232, 205);
static const QColor kLcUp = QColor(200, 222, 250);
static const QColor kLcLeft = QColor(250, 216, 232);
static const QColor kLcDiag = QColor(200, 226, 250);
static const QColor kLcTrace = QColor(255, 214, 150);

static const double kLcStrY = 92.0;
static const double kLcGridY = 236.0;

LCSModule::LCSModule() {}

QString LCSModule::getModuleName() const { return "最长公共子序列"; }
QString LCSModule::getDefaultInput() const { return "abcbdab\nbdcaba"; }
QString LCSModule::getInputHint() const {
    return "第一行输入字符串 s（长度 2~9，只含小写字母）\n"
           "第二行输入字符串 t（长度 2~9，只含小写字母）\n"
           "dp[i][j] 表示 s 前 i 个字符与 t 前 j 个字符的最长公共子序列长度\n"
           "字符相同取左上角加一，不同取左 / 上两者的最大值";
}

ArrayBlock* LCSModule::dpCell(int i, int j) const {
    if (i < 0 || i >= static_cast<int>(m_dpGrid.size())) return nullptr;
    if (j < 0 || j >= static_cast<int>(m_dpGrid[i].size())) return nullptr;
    return m_dpGrid[i][j];
}

void LCSModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;

    const QStringList lines = inputData.split(QRegularExpression("[\\r\\n]"), Qt::SkipEmptyParts);
    if (lines.size() >= 2) {
        m_a = lines[0].trimmed();
        m_b = lines[1].trimmed();
        const bool aOk = m_a.size() >= 2 && m_a.size() <= 9 && !m_a.contains(QRegularExpression("[^a-z]"));
        const bool bOk = m_b.size() >= 2 && m_b.size() <= 9 && !m_b.contains(QRegularExpression("[^a-z]"));
        m_valid = aOk && bOk;
    }

    m_mathText = createScriptText(scene, m_valid ? "最长公共子序列：dp[i][j]，相同取左上角 +1，不同取 max(左, 上)"
                                        : QString("输入格式：两个字符串（各 2~9 个小写字母）"));
    m_mathText->setFont(UiFonts::mixed(15, true));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(110, 18);
    if (!m_valid) return;

    m_n = static_cast<int>(m_a.size());
    m_m = static_cast<int>(m_b.size());

    const double totalW = (m_n + m_m) * m_strCellW + 40.0;
    const double aStartX = std::max(40.0, 500.0 - totalW / 2.0);
    const double bStartX = aStartX + m_n * m_strCellW + 40.0;

    for (int i = 0; i < m_n; ++i) {
        ArrayBlock* block = new ArrayBlock(i, 0, QString::number(i + 1));
        block->setCellSize(m_strCellW, m_strCellH);
        block->setShowLabel(false);
        block->setCustomValue(QString(m_a.at(i)));
        block->color = kLcIdle;
        block->setPos(aStartX + i * m_strCellW, kLcStrY);
        scene->addItem(block);
        m_aCells.push_back(block);
    }

    for (int j = 0; j < m_m; ++j) {
        ArrayBlock* block = new ArrayBlock(500 + j, 0, QString::number(j + 1));
        block->setCellSize(m_strCellW, m_strCellH);
        block->setShowLabel(false);
        block->setCustomValue(QString(m_b.at(j)));
        block->color = kLcIdle;
        block->setPos(bStartX + j * m_strCellW, kLcStrY);
        scene->addItem(block);
        m_bCells.push_back(block);
    }

    QGraphicsTextItem* aLabel = scene->addText("串 a[]");
    aLabel->setFont(UiFonts::cjk(11, true));
    aLabel->setDefaultTextColor(QColor(40, 70, 110));
    aLabel->setPos(aStartX, kLcStrY - 26);

    QGraphicsTextItem* bLabel = scene->addText("串 b[]");
    bLabel->setFont(UiFonts::cjk(11, true));
    bLabel->setDefaultTextColor(QColor(40, 70, 110));
    bLabel->setPos(bStartX, kLcStrY - 26);

    const double gridW = (m_m + 1) * m_cellW;
    m_gridX = std::max(190.0, 500.0 - gridW / 2.0);
    m_gridY = kLcGridY;

    for (int j = 0; j <= m_m; ++j) {
        QGraphicsTextItem* tag = scene->addText(QString::number(j));
        tag->setFont(UiFonts::mono(11));
        tag->setDefaultTextColor(QColor(96, 110, 130));
        tag->setPos(m_gridX + j * m_cellW + m_cellW / 2.0 - 6.0, m_gridY - 34.0);
        m_colTags.push_back(tag);
    }

    m_dpGrid.assign(m_n + 1, std::vector<ArrayBlock*>(m_m + 1, nullptr));
    for (int i = 0; i <= m_n; ++i) {
        for (int j = 0; j <= m_m; ++j) {
            ArrayBlock* block = new ArrayBlock(i * (m_m + 1) + j, 0, QString::number(j));
            block->setCellSize(m_cellW, m_cellH);
            block->setShowLabel(false);
            block->setCustomValue("0");
            block->color = kLcIdle;
            block->setPos(m_gridX + j * m_cellW, m_gridY + i * m_cellH);
            scene->addItem(block);
            m_dpGrid[i][j] = block;
        }
    }

    for (int i = 0; i <= m_n; ++i) {
        QGraphicsTextItem* tag = scene->addText(QString::number(i));
        tag->setFont(UiFonts::mono(11));
        tag->setDefaultTextColor(QColor(96, 110, 130));
        tag->setPos(m_gridX - 34.0, m_gridY + i * m_cellH + m_cellH / 2.0 - 9.0);
        m_rowTags.push_back(tag);
    }

    m_traceLabel = scene->addText("回溯路径（从 dp[n][m] 沿对角线走回 (0,0)）");
    m_traceLabel->setFont(UiFonts::cjk(11, true));
    m_traceLabel->setDefaultTextColor(QColor(60, 75, 95));
    m_traceLabel->setPos(40, 592);
    for (int i = 0; i < m_n; ++i) {
        ArrayBlock* block = new ArrayBlock(3000 + i, 0, QString::number(i + 1));
        block->setCellSize(40, 34);
        block->showValue = false;
        block->color = kLcIdle;
        block->setPos(40 + i * 40, 650);
        scene->addItem(block);
        m_traceCells.push_back(block);
    }

    m_tracePath = new PathTrail(kLcTrace);
    m_tracePath->setTrailWidth(5.0);
    m_tracePath->setHeadRadius(7.5);
    scene->addItem(m_tracePath);
    m_tracePoints.clear();

    m_noteText = createScriptText(scene, "");
    m_noteText->setFont(UiFonts::mixed(13, true));
    m_noteText->setDefaultTextColor(QColor(200, 110, 40));
    m_noteText->setPos(40, 712);

    m_resultText = createScriptText(scene, "");
    m_resultText->setFont(UiFonts::cjk(13, true));
    m_resultText->setDefaultTextColor(QColor(40, 140, 70));
    m_resultText->setPos(40, 746);

    m_ptrI = new PointerBlock("i", QColor(210, 80, 55));
    m_ptrI->setOpacity(0);
    m_ptrI->setDirection(PointerBlock::Down);
    m_ptrI->setShaftLength(26);
    m_ptrI->setAnchorOffset(PointerGeometry::aboveCell(m_strCellW, m_strCellH));
    m_ptrI->setLabelAtTail(8.0);
    scene->addItem(m_ptrI);

    m_ptrJ = new PointerBlock("j", QColor(45, 130, 210));
    m_ptrJ->setOpacity(0);
    m_ptrJ->setDirection(PointerBlock::Up);
    m_ptrJ->setShaftLength(26);
    m_ptrJ->setAnchorOffset(PointerGeometry::belowCell(m_strCellW, m_strCellH));
    m_ptrJ->setLabelAtTail(8.0);
    scene->addItem(m_ptrJ);
}

std::vector<AnimFrame> LCSModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;

    std::vector<std::vector<int>> dp(m_n + 1, std::vector<int>(m_m + 1, 0));

    m_frames.push_back({ActionType::LC_RESET, 0, 0, 0, 0, {},
                        QString("初始化：dp 全部为 0"),
                        QString("dp[i][j] = LCS(s[1..i], t[1..j])")});

    for (int i = 1; i <= m_n; ++i) {
        for (int j = 1; j <= m_m; ++j) {
            const bool same = m_a.at(i - 1) == m_b.at(j - 1);
            if (same) {
                const int value = dp[i - 1][j - 1] + 1;
                dp[i][j] = value;
                m_frames.push_back({ActionType::LC_CANDIDATE, i, j, value, 3, {},
                                    QString("s[%1] = t[%2] = '%3'，取左上角 dp[%4][%5] + 1 = %6")
                                        .arg(i).arg(j).arg(m_a.at(i - 1)).arg(i - 1).arg(j - 1).arg(value),
                                    QString("dp[%1][%2] = dp[%3][%4] + 1").arg(i).arg(j).arg(i - 1).arg(j - 1)});
                m_frames.push_back({ActionType::LC_DECIDE, i, j, value, 1, {},
                                    QString("确定 dp[%1][%2] = %3").arg(i).arg(value),
                                    QString("字符相同 ⇒ dp[i][j] = dp[i-1][j-1] + 1")});
            } else {
                const int up = dp[i - 1][j];
                const int left = dp[i][j - 1];
                const int best = std::max(up, left);
                dp[i][j] = best;
                m_frames.push_back({ActionType::LC_CANDIDATE, i, j, up, 0, {},
                                    QString("s[%1] = '%2' 与 t[%3] = '%4' 不同，比较上方 dp[%5][%6] = %7")
                                        .arg(i).arg(m_a.at(i - 1)).arg(j).arg(m_b.at(j - 1)).arg(i - 1).arg(j).arg(up),
                                    QString("候选一：上方 dp[%1][%2] = %3").arg(i - 1).arg(j).arg(up)});
                m_frames.push_back({ActionType::LC_CANDIDATE, i, j, left, 2, {},
                                    QString("左方 dp[%1][%2] = %3").arg(i).arg(j - 1).arg(left),
                                    QString("候选二：左方 dp[%1][%2] = %3").arg(i).arg(j - 1).arg(left)});
                const int pick = up > left ? 0 : (left > up ? 2 : 3);
                m_frames.push_back({ActionType::LC_DECIDE, i, j, best, pick, {},
                                    QString("确定 dp[%1][%2] = max(%3, %4) = %5")
                                        .arg(i).arg(j).arg(up).arg(left).arg(best),
                                    QString("字符不同 ⇒ dp[i][j] = max(dp[i-1][j], dp[i][j-1])")});
            }
        }
    }

    std::vector<std::pair<int, int>> trace;
    std::vector<std::pair<int, int>> walk;
    walk.push_back({m_n, m_m});
    int i = m_n;
    int j = m_m;
    while (i > 0 && j > 0) {
        if (m_a.at(i - 1) == m_b.at(j - 1)) {
            trace.push_back({i, j});
            --i;
            --j;
        } else if (dp[i - 1][j] >= dp[i][j - 1]) {
            --i;
        } else {
            --j;
        }
        walk.push_back({i, j});
    }

    int matchIndex = 0;
    for (size_t k = 0; k + 1 < walk.size(); ++k) {
        const bool isMatch = std::find(trace.begin(), trace.end(), walk[k]) != trace.end();
        std::vector<int> packed;
        for (size_t w = 0; w <= k; ++w) {
            packed.push_back(walk[w].first);
            packed.push_back(walk[w].second);
        }
        const QString expl = isMatch
            ? QString("回溯：s[%1] = t[%2] = '%3'，取入最长公共子序列")
                  .arg(walk[k].first).arg(walk[k].second).arg(m_a.at(walk[k].first - 1))
            : QString("回溯：dp[%1][%2] 与 dp[%3][%4] 不相等，沿较优一侧退让")
                  .arg(walk[k].first).arg(walk[k].second).arg(walk[k + 1].first).arg(walk[k + 1].second);
        m_frames.push_back({ActionType::LC_TRACE, walk[k].first, walk[k].second,
                            isMatch ? matchIndex : -1, 0, packed,
                            expl,
                            isMatch ? QString("沿对角线取字符 '%1'").arg(m_a.at(walk[k].first - 1))
                                    : QString("退让到 dp[%1][%2]").arg(walk[k + 1].first).arg(walk[k + 1].second)});
        if (isMatch) ++matchIndex;
    }

    QString result;
    for (size_t k = trace.size(); k > 0; --k) result += m_a.at(trace[k - 1].first - 1);

    m_lcsResult = result;
    m_frames.push_back({ActionType::LC_DONE, dp[m_n][m_m], static_cast<int>(trace.size()), 0, 0, {},
                        QString("最长公共子序列长度为 %1").arg(dp[m_n][m_m]),
                        QString("LCS = \"%1\"").arg(result)});
    return m_frames;
}

void LCSModule::renderFrame(const AnimFrame& frame) {
    if (m_mathText) {
        setScriptText(m_mathText, frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    }
    if (!m_valid) return;

    if (frame.type == ActionType::LC_RESET) {
        for (auto& row : m_dpGrid) {
            for (auto* b : row) { b->setCustomValue("0"); b->color = kLcIdle; b->update(); }
        }
        for (auto* b : m_traceCells) { b->showValue = false; b->color = kLcIdle; b->update(); }
        if (m_tracePath) {
            m_tracePath->clearPoints();
            m_tracePath->setProgress(0.0);
        }
        m_tracePoints.clear();
        if (m_ptrI) m_ptrI->setOpacity(0);
        if (m_ptrJ) m_ptrJ->setOpacity(0);
        if (m_noteText) {
            setScriptText(m_noteText, "");
            m_noteText->setDefaultTextColor(QColor(200, 110, 40));
        }
        if (m_resultText) setScriptText(m_resultText, "");
        return;
    }

    if (frame.type == ActionType::LC_CANDIDATE) {
        for (auto& row : m_dpGrid) {
            for (auto* b : row) {
                if (b->color != kLcIdle) { b->color = kLcIdle; b->update(); }
            }
        }
        const int i = frame.u;
        const int j = frame.v;
        const QColor tint = frame.val2 == 0 ? kLcUp : (frame.val2 == 2 ? kLcLeft : kLcDiag);
        ArrayBlock* target = dpCell(i, j);
        if (target) { target->color = tint; target->update(); }
        ArrayBlock* source = nullptr;
        if (frame.val2 == 0) source = dpCell(i - 1, j);
        else if (frame.val2 == 2) source = dpCell(i, j - 1);
        else source = dpCell(i - 1, j - 1);
        if (source) { source->color = tint; source->update(); }
        if (m_animated && source && target && (frame.val2 == 0 || frame.val2 == 2)) {
            playValueArcFlight(source->centerPos(), target->centerPos(), frame.val1, tint, 460);
        }
        if (m_animated && target) {
            playCellRippleAt(target->centerPos(), QSizeF(target->cellWidth(), m_cellH), tint, m_cellH * 1.6 * 0.5, 520);
        }
        if (m_ptrI && i >= 1 && i <= static_cast<int>(m_aCells.size())) {
            m_ptrI->setOpacity(1);
            retargetPointerTo(m_ptrI, m_aCells[i - 1], PointerGeometry::aboveCell(m_strCellW, m_strCellH));
        }
        if (m_ptrJ && j >= 1 && j <= static_cast<int>(m_bCells.size())) {
            m_ptrJ->setOpacity(1);
            retargetPointerTo(m_ptrJ, m_bCells[j - 1], PointerGeometry::belowCell(m_strCellW, m_strCellH));
        }
        if (m_noteText) setScriptText(m_noteText, frame.explanation);
        return;
    }

    if (frame.type == ActionType::LC_DECIDE) {
        ArrayBlock* target = dpCell(frame.u, frame.v);
        if (target) {
            target->setCustomValue(QString::number(frame.val1));
            target->color = frame.val2 == 1 ? kLcSame : kLcDiag;
            target->update();
            if (m_animated) {
                playScalePulse(target, 1.16, 240);
                playCellLanding(target, frame.val2 == 1 ? kLcSame : kLcDiag, 380);
            }
        }
        if (m_ptrI && frame.u >= 1 && frame.u <= static_cast<int>(m_aCells.size())) {
            m_ptrI->setOpacity(1);
            retargetPointerTo(m_ptrI, m_aCells[frame.u - 1], PointerGeometry::aboveCell(m_strCellW, m_strCellH));
        }
        if (m_ptrJ && frame.v >= 1 && frame.v <= static_cast<int>(m_bCells.size())) {
            m_ptrJ->setOpacity(1);
            retargetPointerTo(m_ptrJ, m_bCells[frame.v - 1], PointerGeometry::belowCell(m_strCellW, m_strCellH));
        }
        if (m_noteText) {
            setScriptText(m_noteText, frame.explanation);
            m_noteText->setDefaultTextColor(QColor(70, 110, 170));
        }
        return;
    }

    if (frame.type == ActionType::LC_TRACE) {
        const int i = frame.u;
        const int j = frame.v;
        const int k = frame.val1;

        QVector<QPointF> pts;
        for (size_t w = 0; w + 1 < frame.group.size(); w += 2) {
            ArrayBlock* c = dpCell(frame.group[w], frame.group[w + 1]);
            if (c) pts.append(c->centerPos());
        }
        if (pts.size() >= 2 && m_tracePath) {
            playPathTrail(m_tracePath, pts, kLcTrace, 520);
            m_tracePoints = pts;
        }

        ArrayBlock* target = dpCell(i, j);
        if (target) {
            target->color = kLcTrace;
            target->update();
            if (m_animated) {
                playScalePulse(target, 1.18, 220);
                playCellRippleAt(target->centerPos(), QSizeF(target->cellWidth(), m_cellH), kLcTrace, m_cellH * 1.7 * 0.5, 520);
            }
        }
        if (k >= 0 && k < static_cast<int>(m_traceCells.size())) {
            m_traceCells[k]->showValue = true;
            m_traceCells[k]->setCustomValue(QString(m_a.at(i - 1)));
            m_traceCells[k]->color = kLcTrace;
            m_traceCells[k]->update();
            if (m_animated) playCellLanding(m_traceCells[k], kLcTrace, 340);
        }
        if (m_noteText) setScriptText(m_noteText, frame.explanation);
        return;
    }

    if (frame.type == ActionType::LC_DONE) {
        if (m_resultText) {
            setScriptText(m_resultText, QString("最长公共子序列 \"%1\"，长度 %2").arg(m_lcsResult).arg(frame.u));
            m_resultText->setDefaultTextColor(QColor(40, 140, 70));
        }
        if (m_noteText) {
            setScriptText(m_noteText, frame.explanation);
            m_noteText->setDefaultTextColor(QColor(40, 140, 70));
        }
        if (m_ptrI) m_ptrI->setOpacity(0);
        if (m_ptrJ) m_ptrJ->setOpacity(0);
    }
}

void LCSModule::reset() {
    m_frames.clear();
    m_a.clear();
    m_b.clear();
    m_dpGrid.clear();
    m_aCells.clear();
    m_bCells.clear();
    m_traceCells.clear();
    m_colTags.clear();
    m_rowTags.clear();
    m_tracePath = nullptr;
    m_tracePoints.clear();
    m_mathText = nullptr;
    m_noteText = nullptr;
    m_resultText = nullptr;
    m_traceLabel = nullptr;
    m_ptrI = nullptr;
    m_ptrJ = nullptr;
    m_lcsResult.clear();
    m_n = 0;
    m_m = 0;
    m_valid = false;
}

void LCSModule::releaseSceneItems() {
    m_dpGrid.clear();
    m_aCells.clear();
    m_bCells.clear();
    m_traceCells.clear();
    m_colTags.clear();
    m_rowTags.clear();
    m_tracePath = nullptr;
    m_tracePoints.clear();
    m_mathText = nullptr;
    m_noteText = nullptr;
    m_resultText = nullptr;
    m_traceLabel = nullptr;
    m_ptrI = nullptr;
    m_ptrJ = nullptr;
}
