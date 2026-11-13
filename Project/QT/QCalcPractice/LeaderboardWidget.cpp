#include "LeaderboardWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QSettings>
#include <QPainterPath>
#include <QVariantList>

// ================= 自绘折线图控件实现 =================
LineChartWidget::LineChartWidget(QWidget *parent) : QWidget(parent) {
    this->setMinimumHeight(200);
    // 默认给 5 个 0 作为初始数据
    scores = {0, 0, 0, 0, 0};
}

void LineChartWidget::setScores(const QVector<int> &newScores) {
    scores = newScores;
    // 补齐 5 个数据点，保证图表不断掉
    while (scores.size() < 5) scores.prepend(0);
    if (scores.size() > 5) scores = scores.mid(scores.size() - 5);
    update(); // 触发重绘
}

void LineChartWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int w = width();
    int h = height();
    int margin = 30;

    // 画背景网格和坐标轴
    painter.setPen(QPen(QColor(220, 220, 220), 1, Qt::DashLine));
    for (int i = 0; i < 4; ++i) {
        int y = margin + i * (h - 2 * margin) / 3;
        painter.drawLine(margin, y, w - margin, y);
    }
    painter.setPen(QPen(Qt::black, 2));
    painter.drawLine(margin, h - margin, w - margin, h - margin); // X轴

    // 找最大值，确定Y轴比例
    int maxScore = 1; // 避免除以0
    for (int s : scores) maxScore = qMax(maxScore, s);
    maxScore = qMax(maxScore, 10); // 最小刻度给到10，避免数据太小时折线顶到天花板

    // 计算5个点的坐标
    QVector<QPoint> points;
    int stepX = (w - 2 * margin) / 4;
    for (int i = 0; i < 5; ++i) {
        int px = margin + i * stepX;
        // 留出上下边距，计算Y轴
        int py = (h - margin) - (scores[i] * (h - 2 * margin) / maxScore);
        points.append(QPoint(px, py));
    }

    // 1. 画折线
    QPainterPath path;
    path.moveTo(points[0]);
    for (int i = 1; i < 5; ++i) path.lineTo(points[i]);
    painter.setPen(QPen(QColor(25, 118, 210), 3, Qt::SolidLine));
    painter.drawPath(path);

    // 2. 画数据点和文字
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    for (int i = 0; i < 5; ++i) {
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(211, 47, 47)); // 红点
        painter.drawEllipse(points[i], 6, 6);

        painter.setPen(Qt::black);
        painter.drawText(points[i].x() - 10, points[i].y() - 15, QString::number(scores[i]));

        // 画 X 轴的 "最近 1-5 次"
        painter.drawText(points[i].x() - 10, h - margin + 20, QString("T-%1").arg(5 - i));
    }
}


// ================= 排行榜主界面实现 =================
LeaderboardWidget::LeaderboardWidget(QWidget *parent) : QWidget(parent) {
    setupUI();
    refreshData();
}

void LeaderboardWidget::setupUI() {
    QFont normalFont("Microsoft YaHei", 12, QFont::Bold);

    // 顶部返回按钮
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->setContentsMargins(10, 10, 10, 0);
    backButton = new QPushButton("⬅ 返回菜单");
    backButton->setFont(normalFont);
    backButton->setFixedSize(120, 40);
    connect(backButton, &QPushButton::clicked, this, &LeaderboardWidget::backToMenuRequested);
    topLayout->addWidget(backButton);
    topLayout->addStretch();

    // 核心标签页
    tabWidget = new QTabWidget();
    tabWidget->setFont(normalFont);
    tabWidget->setStyleSheet("QTabBar::tab { height: 40px; width: 150px; }");

    tabWidget->addTab(createEndlessTab(), "无尽模式");
    tabWidget->addTab(createChallengeTab(), "挑战模式");
    tabWidget->addTab(createExtremeTab(), "极限模式");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(tabWidget);
}

QWidget* LeaderboardWidget::createEndlessTab() {
    QWidget *w = new QWidget();
    QVBoxLayout *l = new QVBoxLayout(w);
    l->setAlignment(Qt::AlignCenter);

    endlessAccuracyLabel = new QLabel("历史正确率：暂无数据");
    endlessAccuracyLabel->setFont(QFont("Microsoft YaHei", 24, QFont::Bold));
    endlessAccuracyLabel->setStyleSheet("color: #455a64;");

    QLabel *hint = new QLabel("(注：无尽模式的正确率将在后续更新中接入记录)");
    hint->setFont(QFont("Microsoft YaHei", 12));
    hint->setStyleSheet("color: #9e9e9e;");

    l->addWidget(endlessAccuracyLabel, 0, Qt::AlignCenter);
    l->addSpacing(20);
    l->addWidget(hint, 0, Qt::AlignCenter);
    return w;
}

QWidget* LeaderboardWidget::createChallengeTab() {
    QWidget *w = new QWidget();
    QVBoxLayout *l = new QVBoxLayout(w);

    challengeHighLabel = new QLabel("挑战模式 (全部难度) 历史最高：0 题");
    challengeHighLabel->setFont(QFont("Microsoft YaHei", 20, QFont::Bold));
    challengeHighLabel->setStyleSheet("color: #d32f2f;");

    QLabel *chartTitle = new QLabel("最近 5 次游玩成绩折线图：");
    chartTitle->setFont(QFont("Microsoft YaHei", 14, QFont::Bold));

    challengeChart = new LineChartWidget();

    l->addSpacing(20);
    l->addWidget(challengeHighLabel, 0, Qt::AlignCenter);
    l->addSpacing(40);
    l->addWidget(chartTitle);
    l->addWidget(challengeChart, 1);
    return w;
}

QWidget* LeaderboardWidget::createExtremeTab() {
    QWidget *w = new QWidget();
    QVBoxLayout *l = new QVBoxLayout(w);

    extremeHighLabel = new QLabel("极限模式 (全部题型) 历史最高：0 题");
    extremeHighLabel->setFont(QFont("Microsoft YaHei", 20, QFont::Bold));
    extremeHighLabel->setStyleSheet("color: #388e3c;");

    QLabel *chartTitle = new QLabel("最近 5 次游玩成绩折线图：");
    chartTitle->setFont(QFont("Microsoft YaHei", 14, QFont::Bold));

    extremeChart = new LineChartWidget();

    l->addSpacing(20);
    l->addWidget(extremeHighLabel, 0, Qt::AlignCenter);
    l->addSpacing(40);
    l->addWidget(chartTitle);
    l->addWidget(extremeChart, 1);
    return w;
}

void LeaderboardWidget::refreshData() {
    QSettings settings("TrachtenbergApp", "SpeedMath");

    // --- 1. 读取挑战模式最高分 ---
    int bestChallenge = 0;
    // 遍历所有难度(0-2)和题型(0-5)，找到全局最高分
    for(int diff = 0; diff < 3; ++diff) {
        for(int lvl = 0; lvl < 6; ++lvl) {
            QString key = QString("Challenge_HighScore_Diff%1_Level%2").arg(diff).arg(lvl);
            bestChallenge = qMax(bestChallenge, settings.value(key, 0).toInt());
        }
    }
    challengeHighLabel->setText(QString("挑战模式 (全局) 历史最高： %1 题").arg(bestChallenge));

    // 读取最近5次挑战记录（由于之前的模块我们还没写推入近5次记录的代码，这里先尝试读取，若没有则画空图）
    QVariantList cList = settings.value("Challenge_Recent5").toList();
    QVector<int> cScores;
    for(const QVariant &v : cList) cScores.append(v.toInt());
    challengeChart->setScores(cScores);

    // --- 2. 读取极限模式最高分 ---
    int bestExtreme = 0;
    for(int lvl = 0; lvl < 6; ++lvl) {
        QString key = QString("Extreme_HighScore_Level%1").arg(lvl);
        bestExtreme = qMax(bestExtreme, settings.value(key, 0).toInt());
    }
    extremeHighLabel->setText(QString("极限模式 (全局) 历史最高： %1 题").arg(bestExtreme));

    QVariantList eList = settings.value("Extreme_Recent5").toList();
    QVector<int> eScores;
    for(const QVariant &v : eList) eScores.append(v.toInt());
    extremeChart->setScores(eScores);
}