#ifndef LEADERBOARDWIDGET_H
#define LEADERBOARDWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QPushButton>
#include <QVector>
#include <QLabel>

class LineChartWidget : public QWidget {
    Q_OBJECT
public:
    explicit LineChartWidget(QWidget *parent = nullptr);
    void setScores(const QVector<int> &newScores);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<int> scores;
};

// 排行榜主界面
class LeaderboardWidget : public QWidget {
    Q_OBJECT
public:
    explicit LeaderboardWidget(QWidget *parent = nullptr);
    void refreshData();

signals:
    void backToMenuRequested();

private:
    void setupUI();
    QWidget* createEndlessTab();
    QWidget* createChallengeTab();
    QWidget* createExtremeTab();

    QPushButton *backButton;
    QTabWidget *tabWidget;

    QLabel *endlessAccuracyLabel;
    QLabel *challengeHighLabel;
    QLabel *extremeHighLabel;

    LineChartWidget *challengeChart;
    LineChartWidget *extremeChart;
};

#endif // LEADERBOARDWIDGET_H