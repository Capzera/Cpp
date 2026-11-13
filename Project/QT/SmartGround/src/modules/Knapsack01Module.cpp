#include "Knapsack01Module.h"
#include <QFont>
#include <QRegularExpression>
#include <algorithm>

Knapsack01Module::Knapsack01Module() : m_mathText(nullptr), m_gridStartX(0), m_gridStartY(0), W(0), n(0), m_valid(false) {}

QString Knapsack01Module::getModuleName() const { return "01 背包 DP"; }
QString Knapsack01Module::getDefaultInput() const { return "4 8\n2 3\n3 4\n4 5\n5 8"; }
QString Knapsack01Module::getInputHint() const {
    return "第一行输入两个正整数：物品个数 n、背包容量 W\n"
           "接下来 n 行，每行两个整数 wi, vi，分别表示第 i 件物品的体积与价值\n"
           "每件物品最多只能选一次";
}

void Knapsack01Module::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;
    QStringList tokens = inputData.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    bool ok = false;
    if (tokens.size() >= 2) {
        n = tokens[0].toInt(&ok);
        m_valid = ok && n >= 1 && n <= 8;
        W = tokens[1].toInt(&ok);
        m_valid = m_valid && ok && W >= 0 && W <= 10 && tokens.size() >= 2 + n * 2;
    }
    if (m_valid) {
        w.assign(n + 1, 0);
        v.assign(n + 1, 0);
        for (int i = 1; i <= n; ++i) {
            w[i] = tokens[2 + (i - 1) * 2].toInt(&ok);
            if (!ok || w[i] < 1) m_valid = false;
            v[i] = tokens[3 + (i - 1) * 2].toInt(&ok);
            if (!ok || v[i] < 0) m_valid = false;
        }
    }
    m_mathText = scene->addText(m_valid ? "01 背包：每件物品只可选一次" : "输入格式：n W，随后输入 n 行 wi vi；限制 n≤8，W≤10");
    m_mathText->setFont(QFont("Consolas", 15, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(60, 22);
    if (!m_valid) return;
    QFont labelFont("Microsoft YaHei UI", 10, QFont::Bold);
    labelFont.setStyleStrategy(QFont::PreferQuality);
    int itemStartX = 460 - (n - 1) * 50 - 30;
    QGraphicsTextItem* itemTitle = scene->addText("items：重量 / 价值");
    itemTitle->setFont(labelFont);
    itemTitle->setPos(itemStartX - 8, 80);
    for (int i = 1; i <= n; ++i) {
        ArrayBlock* item = new ArrayBlock(i, w[i], QString::number(i));
        item->setPos(itemStartX + (i - 1) * 100, 135);
        scene->addItem(item);
        m_itemBlocks.push_back(item);
        QGraphicsTextItem* valueLabel = scene->addText(QString("v = %1").arg(v[i]));
        valueLabel->setFont(QFont("Consolas", 10, QFont::Bold));
        valueLabel->setDefaultTextColor(QColor(70, 90, 120));
        valueLabel->setPos(item->pos().x() + 8, item->pos().y() + 48);
    }
    QGraphicsTextItem* gridTitle = scene->addText("dp[i][j]：行 = 物品个数 i，列 = 容量 j");
    gridTitle->setFont(labelFont);
    gridTitle->setPos(25, 248);
    m_gridStartX = 460 - (W + 1) * 30;
    m_gridStartY = 340;
    m_dpGrid.assign(n + 1, std::vector<ArrayBlock*>(W + 1, nullptr));
    for (int i = 0; i <= n; ++i) {
        QGraphicsTextItem* rowLabel = scene->addText(QString::number(i));
        rowLabel->setFont(QFont("Consolas", 13, QFont::Bold));
        rowLabel->setDefaultTextColor(QColor(70, 80, 95));
        rowLabel->setPos(m_gridStartX - 40, m_gridStartY + i * 74 + 12);
        for (int j = 0; j <= W; ++j) {
            ArrayBlock* block = new ArrayBlock(i * (W + 1) + j, 0, QString::number(j));
            block->setShowLabel(i == 0);
            block->setPos(m_gridStartX + j * 60, m_gridStartY + i * 74);
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
    jAxis->setPos(m_gridStartX + (W + 1) * 30 - 8, m_gridStartY - 58);
}

std::vector<AnimFrame> Knapsack01Module::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(W + 1, 0));
    auto encode = [this](int i, int j) { return i * (W + 1) + j; };
    m_frames.push_back({ActionType::KN_RESET, 0, 0, 0, 0, {}, "初始化空集合的状态行", "dp[0][j] = 0"});
    for (int i = 1; i <= n; ++i) {
        for (int j = 0; j <= W; ++j) {
            int skip = dp[i - 1][j];
            m_frames.push_back({ActionType::KN_CANDIDATE, encode(i - 1, j), encode(i, j), skip, 0, {0}, QString("物品 %1：计算不选的价值").arg(i), QString("skip = dp[%1][%2] = %3").arg(i - 1).arg(j).arg(skip)});
            int best = skip;
            int take = -1;
            if (j >= w[i]) {
                take = dp[i - 1][j - w[i]] + v[i];
                m_frames.push_back({ActionType::KN_CANDIDATE, encode(i - 1, j - w[i]), encode(i, j), take, 1, {1}, QString("物品 %1：计算选入后的价值").arg(i), QString("take = dp[%1][%2] + %3 = %4").arg(i - 1).arg(j - w[i]).arg(v[i]).arg(take)});
                best = std::max(skip, take);
            }
            dp[i][j] = best;
            bool chooseTake = take > skip;
            QString formula = j >= w[i]
                ? QString("dp[%1][%2] = max(%3, %4) = %5").arg(i).arg(j).arg(skip).arg(take).arg(best)
                : QString("容量不足，dp[%1][%2] = %3").arg(i).arg(j).arg(best);
            m_frames.push_back({ActionType::KN_DECIDE, i, j, best, chooseTake ? 1 : 0, {}, QString("确定 dp[%1][%2]").arg(i).arg(j), formula});
        }
    }
    m_frames.push_back({ActionType::KN_DECIDE, n, W, dp[n][W], 2, {}, QString("最优价值为 %1").arg(dp[n][W]), QString("答案 = dp[%1][%2] = %3").arg(n).arg(W).arg(dp[n][W])});
    return m_frames;
}

void Knapsack01Module::renderFrame(const AnimFrame& frame) {
    if (!m_mathText) return;
    m_mathText->setPlainText(frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    if (frame.type == ActionType::KN_RESET) {
        for (int i = 0; i <= n; ++i) for (int j = 0; j <= W; ++j) {
            m_dpGrid[i][j]->value = 0;
            m_dpGrid[i][j]->showValue = true;
            m_dpGrid[i][j]->color = Qt::white;
            m_dpGrid[i][j]->update();
        }
        for (ArrayBlock* item : m_itemBlocks) { item->color = Qt::white; item->update(); }
    } else if (frame.type == ActionType::KN_CANDIDATE) {
        for (auto& row : m_dpGrid) for (ArrayBlock* block : row) { block->color = Qt::white; block->update(); }
        int sourceRow = frame.u / (W + 1), sourceCol = frame.u % (W + 1);
        int targetRow = frame.v / (W + 1), targetCol = frame.v % (W + 1);
        ArrayBlock* source = m_dpGrid[sourceRow][sourceCol];
        ArrayBlock* target = m_dpGrid[targetRow][targetCol];
        QColor color = frame.val2 == 1 ? QColor(255, 205, 125) : QColor(155, 210, 245);
        m_itemBlocks[targetRow - 1]->color = color;
        m_itemBlocks[targetRow - 1]->update();
        source->color = color;
        source->update();
        playValueFlight(source->pos(), target, frame.val1, color, true);
    } else if (frame.type == ActionType::KN_DECIDE) {
        if (frame.val2 == 2) {
            m_dpGrid[frame.u][frame.v]->value = frame.val1;
            m_dpGrid[frame.u][frame.v]->color = QColor(150, 220, 175);
            m_dpGrid[frame.u][frame.v]->showValue = true;
            m_dpGrid[frame.u][frame.v]->update();
            return;
        }
        ArrayBlock* target = m_dpGrid[frame.u][frame.v];
        target->value = frame.val1;
        target->showValue = true;
        target->color = frame.val2 ? QColor(150, 220, 175) : QColor(205, 225, 245);
        target->update();
        if (frame.u > 0) { m_itemBlocks[frame.u - 1]->color = Qt::white; m_itemBlocks[frame.u - 1]->update(); }
    }
}

void Knapsack01Module::reset() {
    m_frames.clear();
    m_dpGrid.clear();
    m_itemBlocks.clear();
    m_mathText = nullptr;
    m_gridStartX = 0;
    m_gridStartY = 0;
    W = 0;
    n = 0;
    w.clear();
    v.clear();
    m_valid = false;
}
void Knapsack01Module::releaseSceneItems() {
    m_dpGrid.clear();
    m_itemBlocks.clear();
    m_mathText = nullptr;
}