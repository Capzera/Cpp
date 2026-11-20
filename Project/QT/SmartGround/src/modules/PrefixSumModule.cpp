#include "PrefixSumModule.h"
#include <QRegularExpression>
#include <QFont>

PrefixSumModule::PrefixSumModule() {}

QString PrefixSumModule::getModuleName() const { return "前缀和"; }
QString PrefixSumModule::getDefaultInput() const { return "5 2\n3 1 2 5 4\n2 4\n1 5"; }

QString PrefixSumModule::getInputHint() const {
    return "第一行输入两个正整数：数组长度 n、查询次数 m\n"
           "第二行输入 n 个整数，即原数组 a[1..n]\n"
           "接下来 m 行，每行两个整数 l, r，表示一次区间 [l, r] 求和查询";
}

void PrefixSumModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    m_blocks.clear();
    m_frames.clear();
    m_queries.clear();
    m_dataA.clear();
    m_dataS.clear();

    QString error;
    parseTwoRowInput(inputData, m_n, m_m, m_dataA, error);
    if (!error.isEmpty()) {
        m_mathText = createScriptText(scene, "输入错误：" + error);
        m_mathText->setFont(QFont("Microsoft YaHei UI", 13, QFont::Bold));
        m_mathText->setDefaultTextColor(QColor(180, 50, 50));
        m_mathText->setPos(50, 30);
        return;
    }

    QStringList lines = inputData.split('\n', Qt::SkipEmptyParts);
    for (int i = 2; i < 2 + m_m && i < lines.size(); ++i) {
        QStringList q = lines[i].split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (q.size() < 2) continue;
        int l = q[0].toInt();
        int r = q[1].toInt();
        if (l < 1) l = 1;
        if (r > m_n) r = m_n;
        if (l > r) std::swap(l, r);
        m_queries.push_back({l, r});
    }

    m_mathText = createScriptText(scene, "");
    m_mathText->setFont(QFont("Consolas", 18, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(180, 50, 50));
    m_mathText->setPos(300, 50);

    std::vector<RowSpec> rows;
    RowSpec rowA;
    rowA.row = "a";
    rowA.title = "原数组 a";
    rowA.startIndex = 1;
    rowA.count = m_n;
    rowA.offset = 0;
    rowA.y = 200.0;
    rows.push_back(rowA);

    RowSpec rowS;
    rowS.row = "s";
    rowS.title = "前缀和 s";
    rowS.startIndex = 0;
    rowS.count = m_n + 1;
    rowS.offset = 100;
    rowS.y = 350.0;
    rows.push_back(rowS);

    buildTwoRowLayout(scene, m_dataA, rows);
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
    for (int i = 1; i <= m_n; ++i) m_dataA[i] = liveValue("a", i, m_dataA[i]);
    m_dataS.assign(m_n + 1, 0);
    m_frames.push_back({ActionType::PD_MATH_TEXT, 0, 0, 0, 0, {}, "1. 预处理构建前缀和数组 S", "S[i] = S[i-1] + a[i]"});
    for (int i = 1; i <= m_n; ++i) {
        m_dataS[i] = m_dataS[i - 1] + m_dataA[i];
        const int computed = m_dataS[i];
        m_dataS[i] = liveValue("s", i, computed);
        m_frames.push_back({ActionType::PD_HIGHLIGHT, 0, 0, 0, 0, {i, 100 + i - 1, 100 + i},
                            QString("S[%1] = S[%2] + a[%1]").arg(i).arg(i - 1),
                            QString("S[%1] = %2 + %3 = %4").arg(i).arg(m_dataS[i - 1]).arg(m_dataA[i]).arg(m_dataS[i])});
        m_frames.push_back({ActionType::PD_UPDATE_VAL, 100 + i, 0, m_dataS[i], 0, {i, 100 + i - 1}, "", ""});
        m_frames.push_back({ActionType::PD_RESET, 0, 0, 0, 0, {i, 100 + i - 1, 100 + i}, "", ""});
    }
    for (int q = 0; q < static_cast<int>(m_queries.size()); ++q) {
        const int l = m_queries[q].l;
        const int r = m_queries[q].r;
        m_frames.push_back({ActionType::PD_MATH_TEXT, 0, 0, 0, 0, {},
                            QString("查询 %1: 区间 [%2, %3]").arg(q + 1).arg(l).arg(r), ""});
        int sum = 0;
        QString bfStr = "暴力: ";
        std::vector<int> hl;
        for (int i = l; i <= r; ++i) {
            sum += m_dataA[i];
            hl.push_back(i);
            const QString currentMath = bfStr + (i == l ? QString::number(m_dataA[i]) : QString(" + %1").arg(m_dataA[i]));
            m_frames.push_back({ActionType::PD_HIGHLIGHT, 0, 0, 0, 0, {i},
                                QString("暴力扫描 a[%1]").arg(i), currentMath + QString(" = %1").arg(sum)});
            bfStr = currentMath;
        }
        m_frames.push_back({ActionType::PD_RESET, 0, 0, 0, 0, hl, "", ""});
        const int fast = m_dataS[r] - m_dataS[l - 1];
        m_frames.push_back({ActionType::PD_HIGHLIGHT, 0, 0, 0, 0, {100 + r, 100 + l - 1},
                            QString("优化: O(1) 查询 S[%1] - S[%2]").arg(r).arg(l - 1),
                            QString("O(1): S[%1] - S[%2] = %3 - %4 = %5").arg(r).arg(l - 1).arg(m_dataS[r]).arg(m_dataS[l - 1]).arg(fast)});
        m_frames.push_back({ActionType::PD_RESET, 0, 0, 0, 0, {100 + r, 100 + l - 1}, "", ""});
    }
    return m_frames;
}

void PrefixSumModule::renderFrame(const AnimFrame& frame) {
    renderTwoRowFrame(frame);
}
