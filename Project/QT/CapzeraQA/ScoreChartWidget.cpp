#include "ScoreChartWidget.h"
#include <QGraphicsDropShadowEffect>

ScoreChartWidget::ScoreChartWidget(QWidget *parent) : QWidget(parent) {
    this->setStyleSheet("QWidget { background-color: #F8FAFC; font-family: 'Microsoft YaHei'; }");
    initUI();
}

void ScoreChartWidget::initUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(25, 25, 25, 25);
    mainLayout->setSpacing(20);

    // ==========================================
    // 1. 顶部宏观数据卡片阵列 (Overview Cards)
    // ==========================================
    QHBoxLayout *cardsLayout = new QHBoxLayout();
    cardsLayout->setSpacing(20);

    cardsLayout->addWidget(createDataCard("🚀 累计发布测验", "12 场", "#3B82F6"));
    cardsLayout->addWidget(createDataCard("📝 累计收到答卷", "356 份", "#10B981"));
    cardsLayout->addWidget(createDataCard("📊 全班平均分", "84.5 分", "#F59E0B"));
    cardsLayout->addWidget(createDataCard("🏆 最高纪录分", "100 分", "#EF4444"));

    mainLayout->addLayout(cardsLayout, 1);

    // ==========================================
    // 2. 底部双雄图表展示 (Charts)
    // ==========================================
    QHBoxLayout *chartsLayout = new QHBoxLayout();
    chartsLayout->setSpacing(20);

    // 左侧：柱状图 (近期 5 场考试平均分趋势)
    QFrame *barFrame = new QFrame();
    barFrame->setStyleSheet("QFrame { background: white; border-radius: 12px; border: 1px solid #E2E8F0; }");
    QVBoxLayout *barL = new QVBoxLayout(barFrame);
    barChartView = createRecentExamBarChart();
    barL->addWidget(barChartView);
    chartsLayout->addWidget(barFrame, 6); // 柱状图占 6 份宽

    // 右侧：饼图 (整体及格率分布)
    QFrame *pieFrame = new QFrame();
    pieFrame->setStyleSheet("QFrame { background: white; border-radius: 12px; border: 1px solid #E2E8F0; }");
    QVBoxLayout *pieL = new QVBoxLayout(pieFrame);
    pieChartView = createPassRatePieChart();
    pieL->addWidget(pieChartView);
    chartsLayout->addWidget(pieFrame, 4); // 饼图占 4 份宽

    mainLayout->addLayout(chartsLayout, 4);
}

// 辅助函数：创建具有悬浮阴影感的数据卡片
QFrame* ScoreChartWidget::createDataCard(const QString &title, const QString &value, const QString &color) {
    QFrame *card = new QFrame();
    card->setStyleSheet(QString("QFrame { background: white; border-radius: 12px; border-left: 6px solid %1; border-top: 1px solid #E2E8F0; border-right: 1px solid #E2E8F0; border-bottom: 1px solid #E2E8F0; }").arg(color));

    QVBoxLayout *l = new QVBoxLayout(card);
    l->setContentsMargins(20, 20, 20, 20);

    QLabel *lblTitle = new QLabel(title);
    lblTitle->setStyleSheet("color: #64748B; font-size: 15px; font-weight: bold; border: none;");

    QLabel *lblVal = new QLabel(value);
    lblVal->setStyleSheet(QString("color: %1; font-size: 28px; font-weight: bold; font-family: 'Consolas'; border: none;").arg(color));

    l->addWidget(lblTitle);
    l->addStretch();
    l->addWidget(lblVal);

    return card;
}

// 辅助函数：创建柱状图 (暂时塞入静态假数据)
QChartView* ScoreChartWidget::createRecentExamBarChart() {
    QBarSet *set0 = new QBarSet("平均分");
    set0->setColor(QColor("#3B82F6"));
    *set0 << 75 << 82 << 78 << 88 << 85; // 假数据

    QBarSeries *series = new QBarSeries();
    series->append(set0);
    series->setBarWidth(0.5);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("📈 近 5 场测验全班平均分走势");
    chart->setTitleFont(QFont("Microsoft YaHei", 14, QFont::Bold));
    chart->setAnimationOptions(QChart::SeriesAnimations); // 开启炫酷出场动画

    QStringList categories;
    categories << "周测一" << "周测二" << "期中考" << "周测三" << "综合模拟";
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 100);
    axisY->setTickCount(6);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(false);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setStyleSheet("background: transparent; border: none;");
    return chartView;
}

// 辅助函数：创建饼图
QChartView* ScoreChartWidget::createPassRatePieChart() {
    QPieSeries *series = new QPieSeries();

    QPieSlice *slice1 = series->append("优秀 (90-100)", 25);
    slice1->setColor(QColor("#10B981")); // 绿

    QPieSlice *slice2 = series->append("良好 (75-89)", 45);
    slice2->setColor(QColor("#3B82F6")); // 蓝
    slice2->setExploded(); // 把这一块弹出来

    QPieSlice *slice3 = series->append("及格 (60-74)", 20);
    slice3->setColor(QColor("#F59E0B")); // 黄

    QPieSlice *slice4 = series->append("不及格", 10);
    slice4->setColor(QColor("#EF4444")); // 红

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("🎯 历史成绩等级分布");
    chart->setTitleFont(QFont("Microsoft YaHei", 14, QFont::Bold));
    chart->setAnimationOptions(QChart::SeriesAnimations);

    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->legend()->setFont(QFont("Microsoft YaHei", 10));

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setStyleSheet("background: transparent; border: none;");
    return chartView;
}