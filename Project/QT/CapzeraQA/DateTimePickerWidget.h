#ifndef DATETIMEPICKERWIDGET_H
#define DATETIMEPICKERWIDGET_H

#include <QWidget>
#include <QDateTime>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QListWidget>

// 🚀 纯手绘：Element UI 风格高定日历弹出面板
class CustomCalendarPopup : public QWidget {
    Q_OBJECT
public:
    explicit CustomCalendarPopup(QWidget* parent = nullptr);
    void setDate(const QDate& date);

signals:
    void dateSelected(const QDate& date);

private slots:
    void onPrevYear();
    void onPrevMonth();
    void onNextMonth();
    void onNextYear();
    void onDayClicked();

private:
    void refreshGrid();

    QLabel *lblYearMonth;
    QVector<QPushButton*> dayButtons;
    QDate m_currentViewDate;
    QDate m_selectedDate;
};

// 🚀 纯手绘：洛谷风格双列无极滚轮时间弹出面板
class CustomTimePopup : public QWidget {
    Q_OBJECT
public:
    explicit CustomTimePopup(QWidget* parent = nullptr);
    void setTime(const QTime& time);

signals:
    void timeSelected(const QTime& time);

private slots:
    void onConfirmClicked();

private:
    QListWidget *hourList;
    QListWidget *minList;
};

// 🚀 对外暴露的极简组合容器
class DateTimePickerWidget : public QWidget {
    Q_OBJECT
public:
    explicit DateTimePickerWidget(QWidget *parent = nullptr);
    QDateTime dateTime() const;
    void setDateTime(const QDateTime &dt);

private slots:
    void showCalendar();
    void showTimePicker();
    void updateDisplay();

private:
    QPushButton *btnDate;
    QPushButton *btnTime;

    CustomCalendarPopup *calPopup;
    CustomTimePopup *timePopup;

    QDateTime m_dateTime;
};

#endif // DATETIMEPICKERWIDGET_H