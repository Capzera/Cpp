#include "DifferenceModule.h"
#include <QRegularExpression>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QPointer>
#include <QFont>
DifferenceModule::DifferenceModule() : m_n(0), m_m(0) {}
QString DifferenceModule::getModuleName() const { return "差分"; }
QString DifferenceModule::getDefaultInput() const { return "5 2\n1 1 1 1 1\n2 4 10\n1 3 5"; }
QString DifferenceModule::getInputHint() const {
    return "第一行输入两个正整数：数组长度 n、操作次数 m\n"
           "第二行输入 n 个整数，即原数组 a[1..n]\n"
           "接下来 m 行，每行三个整数 l, r, x，表示把区间 [l, r] 内每个元素都加上 x";
}
void DifferenceModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
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
    m_dataA.assign(m_n + 2, 0); m_dataD.assign(m_n + 2, 0);
    QStringList arrStrs = lines[1].split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    for (int i = 0; i < m_n && i < arrStrs.size(); ++i) m_dataA[i + 1] = arrStrs[i].toInt();
    for (int i = 2; i < 2 + m_m && i < lines.size(); ++i) {
        QStringList q = lines[i].split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (q.size() >= 3) {
            int l = q[0].toInt(), r = q[1].toInt();
            if (l < 1) l = 1;
            if (r > m_n) r = m_n;
            if (l > r) l = r;
            m_queries.push_back({l, r, q[2].toInt()});
        }
    }
    int startX = 400 - ((m_n + 2) * 60) / 2;
    QGraphicsTextItem *labelA = scene->addText("原数组 a");
    labelA->setFont(QFont("Microsoft YaHei", 12, QFont::Bold)); labelA->setPos(startX - 100, 200);
    QGraphicsTextItem *labelD = scene->addText("差分数组 d");
    labelD->setFont(QFont("Microsoft YaHei", 12, QFont::Bold)); labelD->setPos(startX - 100, 350);
    m_mathText = scene->addText("");
    m_mathText->setFont(QFont("Consolas", 18, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(180, 50, 50));
    m_mathText->setPos(300, 50);
    for (int i = 1; i <= m_n + 1; ++i) {
        ArrayBlock* ba = new ArrayBlock(i, m_dataA[i], QString::number(i)); ba->setPos(startX + i * 60, 200);
        if(i > m_n) ba->showValue = false;
        m_scene->addItem(ba); m_blocks[i] = ba;
        registerArraySlot(ba, "a", i);
        ArrayBlock* bd = new ArrayBlock(i, 0, QString::number(i)); bd->setPos(startX + i * 60, 350);
        m_scene->addItem(bd); m_blocks[100 + i] = bd;
        registerArraySlot(bd, "d", i);
    }
}
std::vector<AnimFrame> DifferenceModule::runAlgorithm() {
    m_frames.clear();
    for (int i = 1; i <= m_n; ++i) m_dataA[i] = liveValue("a", i, m_dataA[i]);
    m_frames.push_back({ActionType::PD_MATH_TEXT, 0, 0, 0, 0, {}, "1. 预处理构建差分数组 d", "d[i] = a[i] - a[i-1]"});
    for (int i = 1; i <= m_n + 1; ++i) {
        m_dataD[i] = m_dataA[i] - m_dataA[i - 1];
        m_frames.push_back({ActionType::PD_HIGHLIGHT, 0, 0, 0, 0, {i, i - 1, 100 + i}, QString("d[%1] = a[%1] - a[%2]").arg(i).arg(i-1), QString("d[%1] = %2 - %3 = %4").arg(i).arg(m_dataA[i]).arg(m_dataA[i-1]).arg(m_dataD[i])});
        m_frames.push_back({ActionType::PD_UPDATE_VAL, 100 + i, 0, m_dataD[i], 0, {i, i - 1}, "", ""});
        m_frames.push_back({ActionType::PD_RESET, 0, 0, 0, 0, {i, i - 1, 100 + i}, "", ""});
    }
    for (int q = 0; q < m_queries.size(); ++q) {
        int l = m_queries[q].l, r = m_queries[q].r, x = m_queries[q].x;
        m_frames.push_back({ActionType::PD_MATH_TEXT, 0, 0, 0, 0, {}, QString("修改 %1: 区间 [%2, %3] 加上 %4").arg(q+1).arg(l).arg(r).arg(x), ""});
        std::vector<int> hl;
        for (int i = l; i <= r; ++i) {
            m_dataA[i] += x; hl.push_back(i);
            m_frames.push_back({ActionType::PD_HIGHLIGHT, 0, 0, 0, 0, {i}, QString("暴力修改 a[%1]").arg(i), QString("暴力 O(N): a[%1] 变为 %2").arg(i).arg(m_dataA[i])});
            m_frames.push_back({ActionType::PD_UPDATE_VAL, i, 0, m_dataA[i], 0, {}, "", ""});
        }
        m_frames.push_back({ActionType::PD_RESET, 0, 0, 0, 0, hl, "", ""});
        m_dataD[l] += x;
        m_frames.push_back({ActionType::PD_HIGHLIGHT, 0, 0, 0, 0, {100 + l}, QString("优化: d[%1] += %2").arg(l).arg(x), QString("差分 O(1): d[%1] = d[%1] + %2 = %3").arg(l).arg(x).arg(m_dataD[l])});
        m_frames.push_back({ActionType::PD_UPDATE_VAL, 100 + l, 0, m_dataD[l], 0, {}, "", ""});
        m_frames.push_back({ActionType::PD_RESET, 0, 0, 0, 0, {100 + l}, "", ""});
        if (r + 1 <= m_n + 1) {
            m_dataD[r + 1] -= x;
            m_frames.push_back({ActionType::PD_HIGHLIGHT, 0, 0, 0, 0, {100 + r + 1}, QString("优化: d[%1] -= %2").arg(r+1).arg(x), QString("差分 O(1): d[%1] = d[%1] - %2 = %3").arg(r+1).arg(x).arg(m_dataD[r+1])});
            m_frames.push_back({ActionType::PD_UPDATE_VAL, 100 + r + 1, 0, m_dataD[r+1], 0, {}, "", ""});
            m_frames.push_back({ActionType::PD_RESET, 0, 0, 0, 0, {100 + r + 1}, "", ""});
        }
    }
    return m_frames;
}
ArrayBlock* DifferenceModule::getBlock(int id) { return m_blocks.count(id) ? m_blocks[id] : nullptr; }
void DifferenceModule::renderFrame(const AnimFrame& step) { // DifferenceModule中改名为 DifferenceModule::renderFrame
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
void DifferenceModule::reset() {
	m_dataA.clear();
	m_dataD.clear();
	m_blocks.clear();
	m_frames.clear();
	m_queries.clear();
}
void DifferenceModule::releaseSceneItems() {
    m_blocks.clear();
    m_mathText = nullptr;
}
