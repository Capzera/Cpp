#ifndef STUDENTDASHBOARDWIDGET_H
#define STUDENTDASHBOARDWIDGET_H

#include <QWidget>
#include <QJsonArray>
#include <QJsonObject>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>

class StudentDashboardWidget : public QWidget {
    Q_OBJECT
public:
    explicit StudentDashboardWidget(double dpi, QWidget *parent = nullptr);
    void updateExams(const QJsonArray &exams, const QJsonObject &myRecord); // 接收主中枢投喂数据

signals:
    void startExamRequested(const QString &examId); // 告诉主中枢：我要开考了！

private:
    void initUI();

    double m_dpi;
    QScrollArea *m_scrollArea;
    QWidget *m_container;
    QVBoxLayout *m_containerLayout;
};

#endif // STUDENTDASHBOARDWIDGET_H