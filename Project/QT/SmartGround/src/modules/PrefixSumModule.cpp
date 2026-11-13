#include "PrefixSumModule.h"
#include <QRegularExpression>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QPointer>
#include <QFont>
PrefixSumModule::PrefixSumModule() : m_n(0), m_m(0) {}
QString PrefixSumModule::getModuleName() const { return "前缀和"; }
QString PrefixSumModule::getDefaultInput() const { return "5 2\n3 1 2 5 4\n2 4\n1 5"; }
QString PrefixSumModule::getInputHint() const {
    return "第一行输入两个正整数：数组长度 n、查询次数 m\n"
           "第二行输入 n 个整数，即原数组 a[1..n]\n"
           "接下来 m 行，每行两个整数 l, r，表示一次区间 [l, r] 求和查询";
}
void PrefixSumModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene; m_blocks.clear(); m_frames.clear(); m_queries.clear();
    QStringList lines = inputData.split('\n', Qt::SkipEmptyParts);
    if (lines.size() < 2) return;
    QStringList nm = lines[0].split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    if (nm.size() < 2) return;
    m_n = nm[0].toInt(); m_m = nm[1].toInt();
    if (m_n < 0) m_n = 0;
    if (m_n > 200) m_n = 200;
    if (m_m < 0) m_m = 0;
    if (m_m > 200) m_m = 200;
    m_dataA.assign(m_n + 1, 0); m_dataS.assign(m_n + 1, 0);
    QStringList arrStrs = lines[1].split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    for (int i = 0; i < m_n && i < arrStrs.size(); ++i) m_dataA[i + 1] = arrStrs[i].toInt();
    for (int i = 2; i < 2 + m_m && i < lines.size(); ++i) {
        QStringList q = lines[i].split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (q.size() >= 2) {
            int l = q[0].toInt(), r = q[1].toInt();
            if (l < 1) l = 1;
            if (r > m_n) r = m_n;
            if (l > r) l = r;
            m_queries.push_back({l, r});
        }
    }
    int startX = 400 - ((m_n + 1) * 60) / 2;
    QGraphicsTextItem *labelA = scene->addText("原数组 a");
    labelA->setFont(QFont("Microsoft YaHei", 12, QFont::Bold)); labelA->setPos(startX - 100, 200);
    QGraphicsTextItem *labelS = scene->addText("前缀和 s");
    labelS->setFont(QFont("Microsoft YaHei", 12, QFont::Bold)); labelS->setPos(startX - 100, 350);
    m_mathText = scene->addText("");
    m_mathText->setFont(QFont("Consolas", 18, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(180, 50, 50));
    m_mathText->setPos(300, 50);
    for (int i = 1; i <= m_n; ++i) {
        ArrayBlock* ba = new ArrayBlock(i, m_dataA[i], QString::number(i)); ba->setPos(startX + i * 60, 200);
        m_scene->addItem(ba); m_blocks[i] = ba;
        registerArraySlot(ba, "a", i);
    }
    for (int i = 0; i <= m_n; ++i) {
        ArrayBlock* bs = new ArrayBlock(i, 0, QString::number(i)); bs->setPos(startX + i * 60, 350);
        m_scene->addItem(bs); m_blocks[100 + i] = bs;
        registerArraySlot(bs, "s", i);
    }
}
QString PrefixSumModule::editableRowFor(ArrayBlock* block) const {
    if (!block) return QString();
    return IModule::editableRowFor(block);
}
void PrefixSumModule::onLiveDataChanged(const QString& row, int index, int value) {
    if (row == "a" && index >= 1 && index <= m_n) {
        m_dataA[index] = value;
        return;
    }
    if (row == "s" && index >= 0 && index <= m_n) {
        m_dataS[index] = value;
    }
}
std::vector<AnimFrame> PrefixSumModule::runAlgorithm() {
    m_frames.clear();
    for (int i = 1; i <= m_n; ++i) {
        m_dataA[i] = liveValue("a", i, m_dataA[i]);
    }
    m_dataS.assign(m_n + 1, 0);
    m_frames.push_back({ActionType::PD_MATH_TEXT, 0, 0, 0, 0, {}, "1. 预处理构建前缀和数组 S", "S[i] = S[i-1] + a[i]"});
    for (int i = 1; i <= m_n; ++i) {
        m_dataS[i] = m_dataS[i - 1] + m_dataA[i];
        int computed = m_dataS[i];
        m_dataS[i] = liveValue("s", i, computed);
        m_frames.push_back({ActionType::PD_HIGHLIGHT, 0, 0, 0, 0, {i, 100 + i - 1, 100 + i}, QString("S[%1] = S[%2] + a[%1]").arg(i).arg(i-1), QString("S[%1] = %2 + %3 = %4").arg(i).arg(m_dataS[i-1]).arg(m_dataA[i]).arg(m_dataS[i])});
        m_frames.push_back({ActionType::PD_UPDATE_VAL, 100 + i, 0, m_dataS[i], 0, {i, 100 + i - 1}, "", ""});
        m_frames.push_back({ActionType::PD_RESET, 0, 0, 0, 0, {i, 100 + i - 1, 100 + i}, "", ""});
    }
    for (int q = 0; q < m_queries.size(); ++q) {
        int l = m_queries[q].l, r = m_queries[q].r;
        m_frames.push_back({ActionType::PD_MATH_TEXT, 0, 0, 0, 0, {}, QString("查询 %1: 区间 [%2, %3]").arg(q+1).arg(l).arg(r), ""});
        int sum = 0; QString bfStr = "暴力: "; std::vector<int> hl;
        for (int i = l; i <= r; ++i) {
            sum += m_dataA[i]; hl.push_back(i);
            QString currentMath = bfStr + (i == l ? QString::number(m_dataA[i]) : QString(" + %1").arg(m_dataA[i]));
            m_frames.push_back({ActionType::PD_HIGHLIGHT, 0, 0, 0, 0, {i}, QString("暴力扫描 a[%1]").arg(i), currentMath + QString(" = %1").arg(sum)});
            bfStr = currentMath;
        }
        m_frames.push_back({ActionType::PD_RESET, 0, 0, 0, 0, hl, "", ""});
        int fast = m_dataS[r] - m_dataS[l - 1];
        m_frames.push_back({ActionType::PD_HIGHLIGHT, 0, 0, 0, 0, {100 + r, 100 + l - 1}, QString("优化: O(1) 查询 S[%1] - S[%2]").arg(r).arg(l-1), QString("O(1): S[%1] - S[%2] = %3 - %4 = %5").arg(r).arg(l-1).arg(m_dataS[r]).arg(m_dataS[l-1]).arg(fast)});
        m_frames.push_back({ActionType::PD_RESET, 0, 0, 0, 0, {100 + r, 100 + l - 1}, "", ""});
    }
    return m_frames;
}
ArrayBlock* PrefixSumModule::getBlock(int id) { return m_blocks.count(id) ? m_blocks[id] : nullptr; }
void PrefixSumModule::renderFrame(const AnimFrame& step) {
    if (!step.mathText.isEmpty()) m_mathText->setPlainText(step.mathText);
    if (step.type == ActionType::PD_HIGHLIGHT) {
        for (int id : step.group) { if (getBlock(id)) { getBlock(id)->color = QColor(255, 200, 100); getBlock(id)->update(); } }
    } else if (step.type == ActionType::PD_UPDATE_VAL) {
        ArrayBlock* target = getBlock(step.u);
        if (target) {
            int finalVal = step.val1;
            QPointer<ArrayBlock> safeTarget = target;
            if (m_animated) {
                if (!step.group.empty()) {
                    target->showValue = false; target->update();
                    QParallelAnimationGroup* par = new QParallelAnimationGroup(this);
                    std::vector<QPointer<FloatingText>> texts;
                    for (int srcId : step.group) {
                        ArrayBlock* src = getBlock(srcId);
                        if (src) {
                            QPointer<FloatingText> ft = new FloatingText(QString::number(src->value));
                            ft->setPos(src->pos()); m_scene->addItem(ft.data()); texts.push_back(ft);
                            QPropertyAnimation* anim = new QPropertyAnimation(ft.data(), "pos");
                            anim->setEndValue(target->pos()); anim->setDuration(400);
                            anim->setEasingCurve(QEasingCurve::InOutQuad); par->addAnimation(anim);
                        }
                    }
                    connect(par, &QParallelAnimationGroup::finished, [this, safeTarget, texts, finalVal]() {
                        for (auto& t : texts) { if (t) { if (t->scene()) t->scene()->removeItem(t.data()); delete t.data(); } }
                        if (!safeTarget) return;
                        safeTarget->value = finalVal; safeTarget->color = QColor(150, 255, 150); safeTarget->showValue = true; safeTarget->update();
                        playScalePulse(safeTarget, 1.3, 300);
                    });
                    par->start(QAbstractAnimation::DeleteWhenStopped);
                } else {
                    target->value = finalVal; target->color = QColor(150, 255, 150); target->showValue = true; target->update();
                    playScalePulse(target, 1.3, 300);
                }
            } else {
                target->value = finalVal; target->color = QColor(150, 255, 150); target->showValue = true; target->update();
            }
        }
    } else if (step.type == ActionType::PD_RESET) {
        for (int id : step.group) { if (getBlock(id)) { getBlock(id)->color = Qt::white; getBlock(id)->update(); } }
    }
}
void PrefixSumModule::reset() {
	m_dataA.clear();
	m_dataS.clear();
	m_blocks.clear();
	m_frames.clear();
	m_queries.clear();
}
void PrefixSumModule::releaseSceneItems() {
    m_blocks.clear();
    m_mathText = nullptr;
}
