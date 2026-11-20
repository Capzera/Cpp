#include "LISModule.h"
#include <QFont>
#include <QRegularExpression>
#include <algorithm>

LISModule::LISModule() : m_ptrI(nullptr), m_ptrJ(nullptr), m_mathText(nullptr), m_startX(0), m_valid(false) {}

QString LISModule::getModuleName() const { return "LIS 最长递增子序列"; }
QString LISModule::getDefaultInput() const { return "8\n10 9 2 5 3 7 101 18"; }
QString LISModule::getInputHint() const {
    return "第一行输入一个正整数 n，表示数组长度（n <= 12）\n"
           "第二行输入 n 个整数，即待求最长严格递增子序列的数组";
}

void LISModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;
    QStringList tokens = inputData.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    bool ok = false;
    if (!tokens.isEmpty()) {
        int n = tokens[0].toInt(&ok);
        m_valid = ok && n >= 1 && n <= 12 && tokens.size() >= n + 1;
        if (m_valid) {
            for (int i = 0; i < n; ++i) {
                int value = tokens[i + 1].toInt(&ok);
                if (!ok) { m_valid = false; break; }
                m_data.push_back(value);
            }
        }
    }
    m_mathText = createScriptText(scene, m_valid ? "LIS：先比较 O(N²) 动态规划，再观察 O(N log N) 的 g 数组" : "输入格式：n，随后输入 n 个整数；限制 n≤12");
    m_mathText->setFont(QFont("Consolas", 14, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(75, 18);
    if (!m_valid) return;
    int n = static_cast<int>(m_data.size());
    m_startX = std::max(140, 430 - n * 30);
    QFont labelFont("Microsoft YaHei", 10, QFont::Bold);
    auto addLabel = [scene, labelFont](const QString& text, QPointF pos) {
        QGraphicsTextItem* item = scene->addText(text);
        item->setFont(labelFont);
        item->setDefaultTextColor(QColor(60, 75, 95));
        item->setPos(pos);
    };
    addLabel("a[]", QPointF(30, 125));
    addLabel("dp", QPointF(30, 235));
    addLabel("g", QPointF(30, 425));
    for (int i = 0; i < n; ++i) {
        ArrayBlock* value = new ArrayBlock(i, m_data[i], QString::number(i));
        value->setPos(m_startX + i * 60, 145);
        scene->addItem(value);
        m_arrBlocks[i] = value;
        registerArraySlot(value, "a", i);
        ArrayBlock* dp = new ArrayBlock(i, 0, QString::number(i));
        dp->showValue = false;
        dp->setPos(m_startX + i * 60, 255);
        scene->addItem(dp);
        m_dpBlocks[i] = dp;
        ArrayBlock* tail = new ArrayBlock(i, 0, QString::number(i));
        tail->showValue = false;
        tail->color = QColor(225, 228, 232);
        tail->setPos(m_startX + i * 60, 455);
        scene->addItem(tail);
        m_tailBlocks[i] = tail;
    }
    m_ptrI = new PointerBlock("i", QColor(210, 80, 55));
    m_ptrJ = new PointerBlock("j", QColor(45, 130, 210));
    m_ptrI->setOpacity(0);
    m_ptrJ->setOpacity(0);
    scene->addItem(m_ptrI);
    scene->addItem(m_ptrJ);
}

std::vector<AnimFrame> LISModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;
    int n = static_cast<int>(m_data.size());
    for (int i = 0; i < n; ++i) m_data[i] = liveValue("a", i, m_data[i]);
    std::vector<int> dp(n, 1);
    int answer = 1;
    m_frames.push_back({ActionType::LIS_RESET, 0, 0, 0, 0, {}, "开始计算严格递增子序列", "dp[i] = 1"});
    for (int i = 0; i < n; ++i) {
        m_frames.push_back({ActionType::LIS_WRITE, i, 0, 1, 0, {0}, QString("以 a[%1] 结尾的子序列至少包含自身").arg(i), QString("dp[%1] = 1").arg(i)});
        for (int j = 0; j < i; ++j) {
            m_frames.push_back({ActionType::LIS_COMPARE, i, j, m_data[i], m_data[j], {0}, QString("比较 a[%1]=%2 与 a[%3]=%4").arg(j).arg(m_data[j]).arg(i).arg(m_data[i]), QString("a[%1] %2 a[%3]").arg(j).arg(m_data[j] < m_data[i] ? "<" : "≥").arg(i)});
            if (m_data[j] < m_data[i] && dp[j] + 1 > dp[i]) {
                dp[i] = dp[j] + 1;
                m_frames.push_back({ActionType::LIS_UPDATE, i, j, dp[i], 0, {}, QString("找到更长的前驱链，更新 dp[%1]").arg(i), QString("dp[%1] = dp[%2] + 1 = %3").arg(i).arg(j).arg(dp[i])});
            }
        }
        m_frames.push_back({ActionType::LIS_WRITE, i, 0, dp[i], 1, {}, QString("确定 dp[%1]=%2").arg(i).arg(dp[i]), QString("dp[%1] = %2").arg(i).arg(dp[i])});
        answer = std::max(answer, dp[i]);
    }
    std::vector<int> tails(n);
    int length = 0;
    for (int i = 0; i < n; ++i) {
        int left = 0, right = length;
        while (left < right) {
            int mid = left + (right - left) / 2;
            m_frames.push_back({ActionType::LIS_TAIL_CHECK, i, mid, tails[mid], m_data[i], {left, right}, QString("在 g 数组中二分查找首个不小于 %1 的位置").arg(m_data[i]), QString("g[%1]=%2，比较 a[%3]=%4").arg(mid).arg(tails[mid]).arg(i).arg(m_data[i])});
            if (tails[mid] < m_data[i]) left = mid + 1;
            else right = mid;
        }
        int oldLength = length;
        tails[left] = m_data[i];
        if (left == length) ++length;
        m_frames.push_back({ActionType::LIS_TAIL_SET, i, left, m_data[i], oldLength, {}, left == oldLength ? QString("扩展 g 数组长度到 %1").arg(length) : QString("用更小的结尾值替换 g[%1]").arg(left), QString("g[%1] = %2，当前长度 %3").arg(left).arg(m_data[i]).arg(length)});
    }
    m_frames.push_back({ActionType::LIS_DONE, answer, length, 0, 0, {}, QString("两种方法都得到 LIS 长度 %1").arg(answer), QString("O(N²)=%1，O(N log N)=%2").arg(answer).arg(length)});
    return m_frames;
}

void LISModule::renderFrame(const AnimFrame& frame) {
    if (!m_mathText) return;
    setScriptText(m_mathText, frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    if (frame.type == ActionType::LIS_RESET) {
        for (int i = 0; i < static_cast<int>(m_data.size()); ++i) {
            m_arrBlocks[i]->color = Qt::white;
            m_arrBlocks[i]->update();
            m_dpBlocks[i]->value = 0;
            m_dpBlocks[i]->showValue = false;
            m_dpBlocks[i]->color = Qt::white;
            m_dpBlocks[i]->update();
            m_tailBlocks[i]->value = 0;
            m_tailBlocks[i]->showValue = false;
            m_tailBlocks[i]->color = QColor(225, 228, 232);
            m_tailBlocks[i]->update();
        }
        m_ptrI->setOpacity(0);
        m_ptrJ->setOpacity(0);
    } else if (frame.type == ActionType::LIS_WRITE) {
        for (auto& pair : m_arrBlocks) { pair.second->color = Qt::white; pair.second->update(); }
        ArrayBlock* target = m_dpBlocks[frame.u];
        target->value = frame.val1;
        target->showValue = true;
        target->color = QColor(185, 225, 245);
        target->update();
        playCellLanding(target, QColor(185, 225, 245), 420);
        m_ptrI->setOpacity(1);
        playPointerArrange({m_ptrI}, {m_arrBlocks[frame.u]});
        m_ptrJ->setOpacity(0);
    } else if (frame.type == ActionType::LIS_COMPARE) {
        for (auto& pair : m_arrBlocks) { pair.second->color = Qt::white; pair.second->update(); }
        m_arrBlocks[frame.u]->color = QColor(255, 220, 160);
        m_arrBlocks[frame.v]->color = QColor(155, 210, 245);
        m_arrBlocks[frame.u]->update();
        m_arrBlocks[frame.v]->update();
        playNodePop(m_arrBlocks[frame.u], 1.14, 240);
        playNodePop(m_arrBlocks[frame.v], 1.14, 240);
        m_ptrI->setOpacity(1);
        m_ptrJ->setOpacity(1);
        m_ptrJ->setLabel("j");
        m_ptrJ->update();
        playPointerArrange({m_ptrI, m_ptrJ}, {m_arrBlocks[frame.u], m_arrBlocks[frame.v]});
    } else if (frame.type == ActionType::LIS_UPDATE) {
        playValueFlight(m_dpBlocks[frame.v]->centerPos(), m_dpBlocks[frame.u], frame.val1, QColor(155, 220, 175), true);
        playCellLanding(m_dpBlocks[frame.u], QColor(155, 220, 175), 400);
    } else if (frame.type == ActionType::LIS_TAIL_CHECK) {
        for (auto& pair : m_arrBlocks) { pair.second->color = Qt::white; pair.second->update(); }
        m_ptrI->setOpacity(1);
        m_ptrJ->setOpacity(1);
        m_ptrJ->setLabel("mid");
        m_ptrJ->update();
        playPointerArrange({m_ptrI, m_ptrJ}, {m_arrBlocks[frame.u], m_tailBlocks[frame.v]});
        m_arrBlocks[frame.u]->color = QColor(255, 220, 160);
        m_tailBlocks[frame.v]->color = QColor(155, 210, 245);
        m_arrBlocks[frame.u]->update();
        m_tailBlocks[frame.v]->update();
        playNodePop(m_arrBlocks[frame.u], 1.14, 240);
        playNodePop(m_tailBlocks[frame.v], 1.14, 240);
    } else if (frame.type == ActionType::LIS_TAIL_SET) {
        ArrayBlock* target = m_tailBlocks[frame.v];
        QColor color = frame.v < frame.val2 ? QColor(255, 210, 135) : QColor(155, 220, 175);
        playValueFlight(m_arrBlocks[frame.u]->centerPos(), target, frame.val1, color, true);
        playCellLanding(target, color, 400);
        m_arrBlocks[frame.u]->color = QColor(255, 220, 160);
        m_arrBlocks[frame.u]->update();
        m_ptrI->setOpacity(1);
        playPointerArrange({m_ptrI, m_ptrJ}, {m_arrBlocks[frame.u], m_arrBlocks[frame.u]});
    } else if (frame.type == ActionType::LIS_DONE) {
        for (auto& pair : m_dpBlocks) { pair.second->color = QColor(175, 220, 185); pair.second->update(); }
        for (auto& pair : m_tailBlocks) if (pair.second->showValue) { pair.second->color = QColor(175, 220, 185); pair.second->update(); }
        for (auto& pair : m_dpBlocks) playNodePop(pair.second, 1.10, 240);
        m_ptrI->setOpacity(0);
        m_ptrJ->setOpacity(0);
    }
}

void LISModule::reset() {
    m_frames.clear();
    m_arrBlocks.clear();
    m_dpBlocks.clear();
    m_tailBlocks.clear();
    m_ptrI = nullptr;
    m_ptrJ = nullptr;
    m_mathText = nullptr;
    m_startX = 0;
    m_data.clear();
    m_valid = false;
}
void LISModule::releaseSceneItems() {
    m_arrBlocks.clear();
    m_dpBlocks.clear();
    m_tailBlocks.clear();
    m_ptrI = nullptr;
    m_ptrJ = nullptr;
    m_mathText = nullptr;
}