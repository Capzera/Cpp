#include "DifferenceModule.h"
#include <QRegularExpression>
#include <QFont>

DifferenceModule::DifferenceModule() {}

QString DifferenceModule::getModuleName() const { return "差分"; }
QString DifferenceModule::getDefaultInput() const { return "5 2\n1 1 1 1 1\n2 4 10\n1 3 5"; }

QString DifferenceModule::getInputHint() const {
    return "第一行输入两个正整数：数组长度 n、操作次数 m\n"
           "第二行输入 n 个整数，即原数组 a[1..n]\n"
           "接下来 m 行，每行三个整数 l, r, x，表示把区间 [l, r] 内每个元素都加上 x";
}

void DifferenceModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    m_blocks.clear();
    m_frames.clear();
    m_queries.clear();
    m_dataA.clear();
    m_dataD.clear();

    QString error;
    parseTwoRowInput(inputData, m_n, m_m, m_dataA, error);
    if (!error.isEmpty()) {
        m_mathText = scene->addText("输入错误：" + error);
        m_mathText->setFont(QFont("Microsoft YaHei UI", 13, QFont::Bold));
        m_mathText->setDefaultTextColor(QColor(180, 50, 50));
        m_mathText->setPos(50, 30);
        return;
    }

    QStringList lines = inputData.split('\n', Qt::SkipEmptyParts);
    for (int i = 2; i < 2 + m_m && i < lines.size(); ++i) {
        QStringList q = lines[i].split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (q.size() < 3) continue;
        int l = q[0].toInt();
        int r = q[1].toInt();
        if (l < 1) l = 1;
        if (r > m_n + 1) r = m_n + 1;
        if (l > r) std::swap(l, r);
        m_queries.push_back({l, r, q[2].toInt()});
    }

    m_mathText = scene->addText("");
    m_mathText->setFont(QFont("Consolas", 18, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(180, 50, 50));
    m_mathText->setPos(300, 50);

    std::vector<RowSpec> rows;
    RowSpec rowA;
    rowA.row = "a";
    rowA.title = "原数组 a";
    rowA.startIndex = 1;
    rowA.count = m_n + 1;
    rowA.offset = 0;
    rowA.y = 200.0;
    rowA.hideTail = 1;
    rows.push_back(rowA);

    RowSpec rowD;
    rowD.row = "d";
    rowD.title = "差分数组 d";
    rowD.startIndex = 1;
    rowD.count = m_n + 1;
    rowD.offset = 100;
    rowD.y = 350.0;
    rows.push_back(rowD);

    buildTwoRowLayout(scene, m_dataA, rows);
}

void DifferenceModule::onLiveDataChanged(const QString& row, int index, int value) {
    if (row == "a" && index >= 1 && index <= m_n) {
        m_dataA[index] = value;
        return;
    }
    if (row == "d" && index >= 1 && index <= m_n + 1) {
        m_dataD[index] = value;
    }
}

std::vector<AnimFrame> DifferenceModule::runAlgorithm() {
    m_frames.clear();
    for (int i = 1; i <= m_n; ++i) m_dataA[i] = liveValue("a", i, m_dataA[i]);
    m_dataD.assign(m_n + 2, 0);
    m_frames.push_back({ActionType::PD_MATH_TEXT, 0, 0, 0, 0, {}, "1. 预处理构建差分数组 d", "d[i] = a[i] - a[i-1]"});
    for (int i = 1; i <= m_n + 1; ++i) {
        m_dataD[i] = m_dataA[i] - m_dataA[i - 1];
        m_frames.push_back({ActionType::PD_HIGHLIGHT, 0, 0, 0, 0, {i, i - 1, 100 + i},
                            QString("d[%1] = a[%1] - a[%2]").arg(i).arg(i - 1),
                            QString("d[%1] = %2 - %3 = %4").arg(i).arg(m_dataA[i]).arg(m_dataA[i - 1]).arg(m_dataD[i])});
        m_frames.push_back({ActionType::PD_UPDATE_VAL, 100 + i, 0, m_dataD[i], 0, {i, i - 1}, "", ""});
        m_frames.push_back({ActionType::PD_RESET, 0, 0, 0, 0, {i, i - 1, 100 + i}, "", ""});
    }
    for (int q = 0; q < static_cast<int>(m_queries.size()); ++q) {
        const int l = m_queries[q].l;
        const int r = m_queries[q].r;
        const int x = m_queries[q].x;
        m_frames.push_back({ActionType::PD_MATH_TEXT, 0, 0, 0, 0, {},
                            QString("修改 %1: 区间 [%2, %3] 加上 %4").arg(q + 1).arg(l).arg(r).arg(x), ""});
        std::vector<int> hl;
        for (int i = l; i <= r && i <= m_n; ++i) {
            m_dataA[i] += x;
            hl.push_back(i);
            m_frames.push_back({ActionType::PD_HIGHLIGHT, 0, 0, 0, 0, {i},
                                QString("暴力修改 a[%1]").arg(i),
                                QString("暴力 O(N): a[%1] 变为 %2").arg(i).arg(m_dataA[i])});
            m_frames.push_back({ActionType::PD_UPDATE_VAL, i, 0, m_dataA[i], 0, {}, "", ""});
        }
        m_frames.push_back({ActionType::PD_RESET, 0, 0, 0, 0, hl, "", ""});
        m_dataD[l] += x;
        m_frames.push_back({ActionType::PD_HIGHLIGHT, 0, 0, 0, 0, {100 + l},
                            QString("优化: d[%1] += %2").arg(l).arg(x),
                            QString("差分 O(1): d[%1] = d[%1] + %2 = %3").arg(l).arg(x).arg(m_dataD[l])});
        m_frames.push_back({ActionType::PD_UPDATE_VAL, 100 + l, 0, m_dataD[l], 0, {}, "", ""});
        m_frames.push_back({ActionType::PD_RESET, 0, 0, 0, 0, {100 + l}, "", ""});
        if (r + 1 <= m_n + 1) {
            m_dataD[r + 1] -= x;
            m_frames.push_back({ActionType::PD_HIGHLIGHT, 0, 0, 0, 0, {100 + r + 1},
                                QString("优化: d[%1] -= %2").arg(r + 1).arg(x),
                                QString("差分 O(1): d[%1] = d[%1] - %2 = %3").arg(r + 1).arg(x).arg(m_dataD[r + 1])});
            m_frames.push_back({ActionType::PD_UPDATE_VAL, 100 + r + 1, 0, m_dataD[r + 1], 0, {}, "", ""});
            m_frames.push_back({ActionType::PD_RESET, 0, 0, 0, 0, {100 + r + 1}, "", ""});
        }
    }
    return m_frames;
}

void DifferenceModule::renderFrame(const AnimFrame& frame) {
    renderTwoRowFrame(frame);
}
