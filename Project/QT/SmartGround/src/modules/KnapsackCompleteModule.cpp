#include "KnapsackCompleteModule.h"
#include <QFont>
#include <QRegularExpression>
#include <algorithm>

static const QColor kKcSkip = QColor(155, 210, 245);
static const QColor kKcTake = QColor(255, 205, 125);
static const QColor kKcBest = QColor(150, 220, 175);
static const QColor kKcPale = QColor(250, 250, 252);

KnapsackCompleteModule::KnapsackCompleteModule() {}

QString KnapsackCompleteModule::getModuleName() const { return "完全背包 DP"; }
QString KnapsackCompleteModule::getDefaultInput() const { return "3 8\n2 3\n3 4\n4 5"; }

QString KnapsackCompleteModule::getInputHint() const {
    return "第一行输入两个正整数：物品个数 n、背包容量 W\n"
           "接下来 n 行，每行两个整数 wi, vi，分别表示第 i 件物品的体积与价值\n"
           "完全背包中每件物品可以重复选择，因此状态从同一行的左侧转移";
}

void KnapsackCompleteModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;

    QStringList tokens = inputData.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    bool ok = false;
    if (tokens.size() >= 2) {
        m_n = tokens[0].toInt(&ok);
        m_valid = ok && m_n >= 1 && m_n <= 8;
        m_cols = tokens[1].toInt(&ok);
        m_valid = m_valid && ok && m_cols >= 0 && m_cols <= 10 && tokens.size() >= 2 + m_n * 2;
    }
    if (m_valid) {
        m_w.assign(m_n + 1, 0);
        m_v.assign(m_n + 1, 0);
        for (int i = 1; i <= m_n; ++i) {
            m_w[i] = tokens[2 + (i - 1) * 2].toInt(&ok);
            if (!ok || m_w[i] < 1) m_valid = false;
            m_v[i] = tokens[3 + (i - 1) * 2].toInt(&ok);
            if (!ok || m_v[i] < 0) m_valid = false;
        }
    }

    m_mathText = scene->addText(m_valid ? "完全背包：每件物品可重复选择，状态从同一行左侧转移"
                                         : "输入格式：n W，随后输入 n 行 wi vi；限制 n≤8，W≤10");
    m_mathText->setFont(QFont("Consolas", 15, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(60, 22);
    if (!m_valid) return;

    QFont labelFont("Microsoft YaHei UI", 10, QFont::Bold);
    labelFont.setStyleStrategy(QFont::PreferQuality);

    int itemStartX = 460 - (m_n - 1) * 100 - 30;
    QGraphicsTextItem* itemTitle = scene->addText("items：体积 / 价值");
    itemTitle->setFont(labelFont);
    itemTitle->setPos(itemStartX - 8, 80);
    for (int i = 1; i <= m_n; ++i) {
        ArrayBlock* item = new ArrayBlock(i, m_w[i], QString::number(i));
        item->setPos(itemStartX + (i - 1) * 100, 135);
        scene->addItem(item);
        m_itemBlocks.push_back(item);
        QGraphicsTextItem* valueLabel = scene->addText(QString("v = %1").arg(m_v[i]));
        valueLabel->setFont(QFont("Consolas", 10, QFont::Bold));
        valueLabel->setDefaultTextColor(QColor(70, 90, 120));
        valueLabel->setPos(item->pos().x() + 8, item->pos().y() + 48);
    }

    QGraphicsTextItem* gridTitle = scene->addText("dp[i][j]：行 = 物品个数 i，列 = 容量 j");
    gridTitle->setFont(labelFont);
    gridTitle->setPos(25, 248);
    m_gridStartX = 460 - (m_cols + 1) * 30;
    m_gridStartY = 340;
    m_dpGrid.assign(m_n + 1, std::vector<ArrayBlock*>(m_cols + 1, nullptr));
    for (int i = 0; i <= m_n; ++i) {
        QGraphicsTextItem* rowLabel = scene->addText(QString::number(i));
        rowLabel->setFont(QFont("Consolas", 13, QFont::Bold));
        rowLabel->setDefaultTextColor(QColor(70, 80, 95));
        rowLabel->setPos(m_gridStartX - 40, m_gridStartY + i * 74 + 12);
        for (int j = 0; j <= m_cols; ++j) {
            ArrayBlock* block = new ArrayBlock(i * (m_cols + 1) + j, 0, QString::number(j));
            block->setShowLabel(i == 0);
            block->setPos(m_gridStartX + j * 60, m_gridStartY + i * 74);
            block->color = kKcPale;
            scene->addItem(block);
            m_dpGrid[i][j] = block;
        }
    }
    QGraphicsTextItem* iAxis = scene->addText("i");
    iAxis->setFont(QFont("Consolas", 13, QFont::Bold));
    iAxis->setDefaultTextColor(QColor(70, 80, 95));
    iAxis->setPos(m_gridStartX - 40, m_gridStartY - 58);
    QGraphicsTextItem* jAxis = scene->addText("j");
    jAxis->setFont(QFont("Consolas", 13, QFont::Bold));
    jAxis->setDefaultTextColor(QColor(70, 80, 95));
    jAxis->setPos(m_gridStartX + (m_cols + 1) * 30 - 8, m_gridStartY - 58);
}

std::vector<AnimFrame> KnapsackCompleteModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;

    m_frames.push_back({ActionType::KC_RESET, 0, 0, 0, 0, {},
                        "初始化：dp[0][j] = 0 表示不选任何物品",
                        "完全背包：dp[i][j] = max(dp[i-1][j], dp[i][j-w[i]] + v[i])"});

    std::vector<std::vector<int>> dp(m_n + 1, std::vector<int>(m_cols + 1, 0));
    for (int i = 1; i <= m_n; ++i) {
        for (int j = 0; j <= m_cols; ++j) {
            int skip = dp[i - 1][j];
            m_frames.push_back({ActionType::KC_CANDIDATE, encode(i - 1, j), encode(i, j), skip, 0, {0},
                                QString("物品 %1：不选它，价值为 dp[%2][%3]").arg(i).arg(i - 1).arg(j),
                                QString("skip = dp[%1][%2] = %3").arg(i - 1).arg(j).arg(skip)});

            int best = skip;
            int take = -1;
            bool canTake = false;
            if (j >= m_w[i]) {
                take = dp[i][j - m_w[i]] + m_v[i];
                canTake = true;
                m_frames.push_back({ActionType::KC_CANDIDATE, encode(i, j - m_w[i]), encode(i, j), take, 1, {1},
                                    QString("物品 %1：再选一次，价值为 dp[%2][%3] + %4")
                                        .arg(i).arg(i).arg(j - m_w[i]).arg(m_v[i]),
                                    QString("take = dp[%1][%2] + %3 = %4　（同一行左侧，物品可重复选）")
                                        .arg(i).arg(j - m_w[i]).arg(m_v[i]).arg(take)});
                best = std::max(skip, take);
            }
            dp[i][j] = best;
            bool chooseTake = canTake && take > skip;
            QString formula;
            if (!canTake) {
                formula = QString("容量不足，dp[%1][%2] = %3").arg(i).arg(j).arg(best);
            } else {
                formula = QString("dp[%1][%2] = max(%3, %4) = %5").arg(i).arg(j).arg(skip).arg(take).arg(best);
            }
            m_frames.push_back({ActionType::KC_DECIDE, i, j, best, chooseTake ? 1 : 0, {},
                                QString("确定 dp[%1][%2]").arg(i).arg(j), formula});
        }
    }
    m_frames.push_back({ActionType::KC_DECIDE, m_n, m_cols, dp[m_n][m_cols], 2, {},
                        QString("最优价值为 %1").arg(dp[m_n][m_cols]),
                        QString("答案 = dp[%1][%2] = %3").arg(m_n).arg(m_cols).arg(dp[m_n][m_cols])});
    return m_frames;
}

void KnapsackCompleteModule::renderFrame(const AnimFrame& frame) {
    if (!m_mathText) return;
    m_mathText->setPlainText(frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    if (frame.type == ActionType::KC_RESET) {
        for (int i = 0; i <= m_n; ++i) {
            for (int j = 0; j <= m_cols; ++j) {
                ArrayBlock* block = m_dpGrid[i][j];
                block->value = 0;
                block->showValue = true;
                block->color = kKcPale;
                block->update();
            }
        }
        for (ArrayBlock* item : m_itemBlocks) { item->color = Qt::white; item->update(); }
    } else if (frame.type == ActionType::KC_CANDIDATE) {
        for (auto& row : m_dpGrid) for (ArrayBlock* block : row) { block->color = kKcPale; block->update(); }
        int sourceRow = frame.u / (m_cols + 1), sourceCol = frame.u % (m_cols + 1);
        int targetRow = frame.v / (m_cols + 1), targetCol = frame.v % (m_cols + 1);
        ArrayBlock* source = m_dpGrid[sourceRow][sourceCol];
        ArrayBlock* target = m_dpGrid[targetRow][targetCol];
        QColor color = frame.val2 == 1 ? kKcTake : kKcSkip;
        if (targetRow >= 1 && targetRow - 1 < static_cast<int>(m_itemBlocks.size())) {
            m_itemBlocks[targetRow - 1]->color = color;
            m_itemBlocks[targetRow - 1]->update();
        }
        source->color = color;
        source->update();
        playValueFlight(source->pos(), target, frame.val1, color, true);
    } else if (frame.type == ActionType::KC_DECIDE) {
        if (frame.val2 == 2) {
            ArrayBlock* target = m_dpGrid[frame.u][frame.v];
            target->value = frame.val1;
            target->color = kKcBest;
            target->showValue = true;
            target->update();
            return;
        }
        ArrayBlock* target = m_dpGrid[frame.u][frame.v];
        target->value = frame.val1;
        target->showValue = true;
        target->color = frame.val2 ? kKcBest : QColor(205, 225, 245);
        target->update();
        if (frame.u > 0 && frame.u - 1 < static_cast<int>(m_itemBlocks.size())) {
            m_itemBlocks[frame.u - 1]->color = Qt::white;
            m_itemBlocks[frame.u - 1]->update();
        }
    }
}

void KnapsackCompleteModule::onLiveDataChanged(const QString& row, int index, int value) {
    Q_UNUSED(row);
    Q_UNUSED(index);
    Q_UNUSED(value);
}

void KnapsackCompleteModule::reset() {
    m_frames.clear();
    m_dpGrid.clear();
    m_itemBlocks.clear();
    m_mathText = nullptr;
    m_gridStartX = 0;
    m_gridStartY = 0;
    m_cols = 0;
    m_n = 0;
    m_w.clear();
    m_v.clear();
    m_valid = false;
}

void KnapsackCompleteModule::releaseSceneItems() {
    m_dpGrid.clear();
    m_itemBlocks.clear();
    m_mathText = nullptr;
}
