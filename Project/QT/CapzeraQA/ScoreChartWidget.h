#ifndef SCORECHARTWIDGET_H
#define SCORECHARTWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>

// 引入 Qt Charts 模块
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarCategoryAxis>

// 高定大屏：学情分析独立组件
class ScoreChartWidget : public QWidget {
    Q_OBJECT
public:
    explicit ScoreChartWidget(QWidget *parent = nullptr);

    // 后期咱们在这里暴露更新数据的接口
    void updateDashboardData();

    template <typename T>
    void setScores(const T &scores) {
        Q_UNUSED(scores);
        // 此处为兼容性占位符，等咱们后续重构 TeacherStudentPage 时再打通数据
    }

private:
    void initUI();
    QFrame* createDataCard(const QString &title, const QString &value, const QString &color);
    QChartView* createPassRatePieChart();
    QChartView* createRecentExamBarChart();

    // 顶部四个核心数据卡片
    QLabel *lblTotalExams;
    QLabel *lblTotalSubmissions;
    QLabel *lblAvgScore;
    QLabel *lblTopScore;

    // 图表视图
    QChartView *pieChartView;
    QChartView *barChartView;
};

#endif // SCORECHARTWIDGET_H