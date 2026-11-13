#include "DateTimePickerWidget.h"
#include <QApplication>
#include <QStyle>

// ==========================================================
// 🚀 一、纯手绘工业级日历引擎 (CustomCalendarPopup)
// ==========================================================
CustomCalendarPopup::CustomCalendarPopup(QWidget* parent) : QWidget(parent) {
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet("CustomCalendarPopup { background: #FFFFFF; border: 1px solid #CBD5E1; border-radius: 8px; }");
    setFixedSize(320, 340);

    QVBoxLayout *mainL = new QVBoxLayout(this);
    mainL->setContentsMargins(15, 15, 15, 15); mainL->setSpacing(10);

    // --- 1. 头部控制区 (<<  <  2026年6月  >  >>) ---
    QHBoxLayout *headerL = new QHBoxLayout();
    QString btnStyle = "QPushButton { color: #64748B; font-weight: bold; font-size: 14px; border: none; background: transparent; border-radius: 4px; padding: 4px; } QPushButton:hover { color: #3B82F6; background: #F1F5F9; }";

    QPushButton *btnPrevY = new QPushButton("≪"); btnPrevY->setFixedSize(30, 30); btnPrevY->setStyleSheet(btnStyle);
    QPushButton *btnPrevM = new QPushButton("＜"); btnPrevM->setFixedSize(30, 30); btnPrevM->setStyleSheet(btnStyle);
    lblYearMonth = new QLabel("2026 年 6 月"); lblYearMonth->setAlignment(Qt::AlignCenter);
    lblYearMonth->setStyleSheet("color: #1E293B; font-family: 'Microsoft YaHei'; font-size: 15px; font-weight: bold; border: none;");
    QPushButton *btnNextM = new QPushButton("＞"); btnNextM->setFixedSize(30, 30); btnNextM->setStyleSheet(btnStyle);
    QPushButton *btnNextY = new QPushButton("≫"); btnNextY->setFixedSize(30, 30); btnNextY->setStyleSheet(btnStyle);

    connect(btnPrevY, &QPushButton::clicked, this, &CustomCalendarPopup::onPrevYear);
    connect(btnPrevM, &QPushButton::clicked, this, &CustomCalendarPopup::onPrevMonth);
    connect(btnNextM, &QPushButton::clicked, this, &CustomCalendarPopup::onNextMonth);
    connect(btnNextY, &QPushButton::clicked, this, &CustomCalendarPopup::onNextYear);

    headerL->addWidget(btnPrevY); headerL->addWidget(btnPrevM); headerL->addWidget(lblYearMonth, 1); headerL->addWidget(btnNextM); headerL->addWidget(btnNextY);
    mainL->addLayout(headerL);

    // --- 2. 星期表头 ---
    QGridLayout *gridL = new QGridLayout(); gridL->setSpacing(4);
    QStringList weekDays = {"一", "二", "三", "四", "五", "六", "日"};
    for (int i = 0; i < 7; ++i) {
        QLabel *lbl = new QLabel(weekDays[i]); lbl->setAlignment(Qt::AlignCenter);
        lbl->setStyleSheet("color: #94A3B8; font-weight: bold; font-size: 13px; margin-bottom: 5px;");
        gridL->addWidget(lbl, 0, i);
    }

    // --- 3. 生成 42 个复用日期按钮 ---
    for (int i = 0; i < 42; ++i) {
        QPushButton *btn = new QPushButton(); btn->setFixedSize(34, 34);
        connect(btn, &QPushButton::clicked, this, &CustomCalendarPopup::onDayClicked);
        dayButtons.append(btn);
        gridL->addWidget(btn, i / 7 + 1, i % 7);
    }
    mainL->addLayout(gridL);

    m_currentViewDate = QDate::currentDate();
    m_selectedDate = QDate::currentDate();
    refreshGrid();
}

void CustomCalendarPopup::setDate(const QDate& date) { m_selectedDate = date; m_currentViewDate = date; refreshGrid(); }
void CustomCalendarPopup::onPrevYear() { m_currentViewDate = m_currentViewDate.addYears(-1); refreshGrid(); }
void CustomCalendarPopup::onPrevMonth() { m_currentViewDate = m_currentViewDate.addMonths(-1); refreshGrid(); }
void CustomCalendarPopup::onNextMonth() { m_currentViewDate = m_currentViewDate.addMonths(1); refreshGrid(); }
void CustomCalendarPopup::onNextYear() { m_currentViewDate = m_currentViewDate.addYears(1); refreshGrid(); }

void CustomCalendarPopup::onDayClicked() {
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int day = btn->text().toInt();
    bool isCurrentMonth = btn->property("isCurrentMonth").toBool();

    if (isCurrentMonth) {
        // 点击当前月的日期
        m_selectedDate = QDate(m_currentViewDate.year(), m_currentViewDate.month(), day);
    } else if (day > 15) {
        // 点击上个月的尾巴 (如 28, 29, 30, 31)
        QDate prevMonth = m_currentViewDate.addMonths(-1);
        m_selectedDate = QDate(prevMonth.year(), prevMonth.month(), day);
    } else {
        // 点击下个月的开头 (如 1, 2, 3, 4)
        QDate nextMonth = m_currentViewDate.addMonths(1);
        m_selectedDate = QDate(nextMonth.year(), nextMonth.month(), day);
    }

    emit dateSelected(m_selectedDate);
    close(); // 选择后自动关闭弹出层
}

void CustomCalendarPopup::refreshGrid() {
    lblYearMonth->setText(QString("%1 年 %2 月").arg(m_currentViewDate.year()).arg(m_currentViewDate.month()));

    QDate firstDay(m_currentViewDate.year(), m_currentViewDate.month(), 1);
    int startDayOfWeek = firstDay.dayOfWeek(); // 1(Mon) - 7(Sun)
    int daysInPrevMonth = firstDay.addDays(-1).day();
    int daysInCurrentMonth = m_currentViewDate.daysInMonth();

    int currentDay = 1; int nextMonthDay = 1;

    // 🚀 核心 QSS 动态染色引擎
    QString normalStyle = "QPushButton { color: #334155; border-radius: 17px; background: transparent; font-size: 14px; font-family:'Consolas'; } QPushButton:hover { background: #E0E7FF; color: #3B82F6; font-weight:bold; }";
    QString grayStyle = "QPushButton { color: #CBD5E1; border-radius: 17px; background: transparent; font-size: 14px; font-family:'Consolas'; } QPushButton:hover { background: #F1F5F9; }";
    QString selectedStyle = "QPushButton { color: #FFFFFF; border-radius: 17px; background: #3B82F6; font-size: 14px; font-weight:bold; font-family:'Consolas'; box-shadow: 0 4px 6px -1px rgba(59, 130, 246, 0.5); }";

    for (int i = 0; i < 42; ++i) {
        QPushButton* btn = dayButtons[i];
        if (i < startDayOfWeek - 1) { // 上个月的尾巴
            btn->setText(QString::number(daysInPrevMonth - (startDayOfWeek - 1 - i) + 1));
            btn->setProperty("isCurrentMonth", false); btn->setStyleSheet(grayStyle);
        } else if (currentDay <= daysInCurrentMonth) { // 这个月
            btn->setText(QString::number(currentDay));
            btn->setProperty("isCurrentMonth", true);
            if (m_currentViewDate.year() == m_selectedDate.year() && m_currentViewDate.month() == m_selectedDate.month() && currentDay == m_selectedDate.day()) {
                btn->setStyleSheet(selectedStyle);
            } else {
                btn->setStyleSheet(normalStyle);
            }
            currentDay++;
        } else { // 下个月的开头
            btn->setText(QString::number(nextMonthDay++));
            btn->setProperty("isCurrentMonth", false); btn->setStyleSheet(grayStyle);
        }
    }
}

// ==========================================================
// 🚀 二、洛谷风格无极滚轮时间引擎 (CustomTimePopup)
// ==========================================================
CustomTimePopup::CustomTimePopup(QWidget* parent) : QWidget(parent) {
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet("CustomTimePopup { background: #FFFFFF; border: 1px solid #CBD5E1; border-radius: 8px; }");
    setFixedSize(200, 260);

    QVBoxLayout *mainL = new QVBoxLayout(this);
    mainL->setContentsMargins(10, 15, 10, 10); mainL->setSpacing(10);

    QHBoxLayout *listsL = new QHBoxLayout(); listsL->setSpacing(5);

    // 极其丝滑的列表 QSS（隐藏滚轮，突出选中项）
    QString listStyle =
        "QListWidget { border: none; background: transparent; outline: none; font-family:'Consolas'; font-size:16px; color:#64748B; }"
        "QListWidget::item { padding: 8px; text-align: center; border-radius:4px; margin: 0 4px; }"
        "QListWidget::item:hover { background: #F1F5F9; color:#3B82F6; }"
        "QListWidget::item:selected { background: #E0E7FF; color: #3B82F6; font-weight: bold; }";

    hourList = new QListWidget(); hourList->setStyleSheet(listStyle);
    hourList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 隐藏滚动条
    for(int i=0; i<=23; ++i) {
        QListWidgetItem *item = new QListWidgetItem(QString::number(i).rightJustified(2, '0'));
        item->setTextAlignment(Qt::AlignCenter); hourList->addItem(item);
    }

    minList = new QListWidget(); minList->setStyleSheet(listStyle);
    minList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 隐藏滚动条
    for(int i=0; i<=59; ++i) {
        QListWidgetItem *item = new QListWidgetItem(QString::number(i).rightJustified(2, '0'));
        item->setTextAlignment(Qt::AlignCenter); minList->addItem(item);
    }

    listsL->addWidget(hourList);
    QLabel *colon = new QLabel(":"); colon->setStyleSheet("font-size:18px; font-weight:bold; color:#94A3B8; border:none;");
    listsL->addWidget(colon);
    listsL->addWidget(minList);
    mainL->addLayout(listsL);

    QPushButton *btnConfirm = new QPushButton("✅ 确定时间");
    btnConfirm->setFixedHeight(36);
    btnConfirm->setStyleSheet("QPushButton{background:#3B82F6; color:white; font-weight:bold; font-size:14px; border-radius:6px; border:none;} QPushButton:hover{background:#2563EB;}");
    connect(btnConfirm, &QPushButton::clicked, this, &CustomTimePopup::onConfirmClicked);
    mainL->addWidget(btnConfirm);
}

void CustomTimePopup::setTime(const QTime& time) {
    hourList->setCurrentRow(time.hour()); hourList->scrollToItem(hourList->item(time.hour()), QAbstractItemView::PositionAtCenter);
    minList->setCurrentRow(time.minute()); minList->scrollToItem(minList->item(time.minute()), QAbstractItemView::PositionAtCenter);
}

void CustomTimePopup::onConfirmClicked() {
    int h = hourList->currentRow(); int m = minList->currentRow();
    emit timeSelected(QTime(h, m)); close(); // 选择后自动关闭
}

// ==========================================================
// 🚀 三、对外暴露的组合中枢 (DateTimePickerWidget)
// ==========================================================
DateTimePickerWidget::DateTimePickerWidget(QWidget *parent) : QWidget(parent) {
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0); layout->setSpacing(10);

    // 高端伪装按钮 QSS
    QString pseudoInputQss =
        "QPushButton { text-align: left; padding-left: 15px; background: #FFFFFF; border: 1px solid #DCDFE6; border-radius: 4px; color: #303133; font-family: 'Consolas'; font-size: 15px; font-weight:bold; min-height:38px; }"
        "QPushButton:hover { border-color: #409EFF; }";

    btnDate = new QPushButton("📅 --"); btnDate->setStyleSheet(pseudoInputQss); btnDate->setCursor(Qt::PointingHandCursor);
    btnTime = new QPushButton("🕒 --:--"); btnTime->setStyleSheet(pseudoInputQss); btnTime->setCursor(Qt::PointingHandCursor);

    calPopup = new CustomCalendarPopup();
    timePopup = new CustomTimePopup();

    connect(btnDate, &QPushButton::clicked, this, &DateTimePickerWidget::showCalendar);
    connect(btnTime, &QPushButton::clicked, this, &DateTimePickerWidget::showTimePicker);

    // 监听子组件的确认回调
    connect(calPopup, &CustomCalendarPopup::dateSelected, this, [this](const QDate &d){
        m_dateTime.setDate(d); updateDisplay();
    });
    connect(timePopup, &CustomTimePopup::timeSelected, this, [this](const QTime &t){
        m_dateTime.setTime(t); updateDisplay();
    });

    layout->addWidget(btnDate, 6); layout->addWidget(btnTime, 4);

    setDateTime(QDateTime::currentDateTime());
}

void DateTimePickerWidget::setDateTime(const QDateTime &dt) { m_dateTime = dt; updateDisplay(); }
QDateTime DateTimePickerWidget::dateTime() const { return m_dateTime; }

void DateTimePickerWidget::updateDisplay() {
    btnDate->setText("📅 " + m_dateTime.toString("yyyy-MM-dd"));
    btnTime->setText("🕒 " + m_dateTime.toString("HH:mm"));
}

void DateTimePickerWidget::showCalendar() {
    QPoint pos = btnDate->mapToGlobal(QPoint(0, btnDate->height() + 5)); // 悬浮于输入框正下方
    calPopup->move(pos); calPopup->setDate(m_dateTime.date()); calPopup->show();
}

void DateTimePickerWidget::showTimePicker() {
    QPoint pos = btnTime->mapToGlobal(QPoint(0, btnTime->height() + 5));
    timePopup->move(pos); timePopup->setTime(m_dateTime.time()); timePopup->show();
}