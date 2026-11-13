#include "StudentDashboardWidget.h"
#include "CapzeraButton.h"
#include <QFrame>

StudentDashboardWidget::StudentDashboardWidget(double dpi, QWidget *parent)
    : QWidget(parent), m_dpi(dpi) {
    initUI();
}

void StudentDashboardWidget::initUI() {
    this->setStyleSheet("QWidget { background-color: #F8FAFC; font-family: 'Microsoft YaHei'; }");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    QLabel *title = new QLabel("🎯 考试大厅");
    title->setStyleSheet("color: #1E293B; font-weight: bold; font-size: 26px; border: none; background: transparent;");
    mainLayout->addWidget(title);

    m_scrollArea = new QScrollArea();
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setStyleSheet("QScrollArea { border: none; background: transparent; }");

    m_container = new QWidget();
    m_container->setStyleSheet("background: transparent;");
    m_containerLayout = new QVBoxLayout(m_container);
    m_containerLayout->setContentsMargins(0, 10, 0, 10);
    m_containerLayout->setSpacing(15);

    m_scrollArea->setWidget(m_container);
    mainLayout->addWidget(m_scrollArea);
}

void StudentDashboardWidget::updateExams(const QJsonArray &exams, const QJsonObject &myRecord) {
    // 清空旧卡片
    QLayoutItem *child;
    while ((child = m_containerLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }

    if (exams.isEmpty()) {
        QLabel *empty = new QLabel("当前没有任何考试任务！");
        empty->setStyleSheet("color: #94A3B8; font-size: 18px; font-weight: bold; background: transparent;");
        empty->setAlignment(Qt::AlignCenter);
        m_containerLayout->addWidget(empty);
        return;
    }

    for (int i = 0; i < exams.size(); ++i) {
        QJsonObject exam = exams[i].toObject();
        QString examId = exam["examId"].toString();
        QString title = exam["title"].toString();
        QString startTime = exam["startTime"].toString();
        QString endTime = exam["endTime"].toString();
        QString status = "未参加";
        if (exam["status"].toString() != "启用") continue;
        if (myRecord.contains(examId)) {
            QString s = myRecord[examId].toObject()["status"].toString();
            if (s == "SUBMITTED") status = "已交卷";
            else if (s == "SAVED") status = "答题中 (暂存)";
        }
        QFrame *card = new QFrame();
        card->setStyleSheet("QFrame { background: white; border-radius: 12px; border: 1px solid #E2E8F0; padding: 7px; } "
                            "QFrame:hover { border-color: #3B82F6; box-shadow: 0 4px 15px rgba(59,130,246,0.1); }");

        QHBoxLayout *cardL = new QHBoxLayout(card);
        cardL->setContentsMargins(0,0,0,0);

        QVBoxLayout *infoL = new QVBoxLayout();
        QLabel *lblTitle = new QLabel(title);
        lblTitle->setStyleSheet("color: #1E293B; font-family: HYWenHei-85W; font-size: 24px; font-weight: bold; border: none;");

        QLabel *lblStartTime = new QLabel("🕒 开考时间：" + startTime);
        lblStartTime->setStyleSheet("color: #64748B; font-size: 18px; border: none;");

        QLabel *lblEndTime = new QLabel("🕒 结束时间：" + endTime);
        lblEndTime->setStyleSheet("color: #64748B; font-size: 18px; border: none;");

        QLabel *lblStatus = new QLabel();
        if (status == "已交卷") lblStatus->setStyleSheet("color: #059669; background: #DCFCE7; padding: 4px 10px; border-radius: 6px; font-weight: bold; font-size: 18px;");
        else if (status.contains("暂存")) lblStatus->setStyleSheet("color: #B45309; background: #FEF3C7; padding: 4px 10px; border-radius: 6px; font-weight: bold; font-size: 18px;");
        else lblStatus->setStyleSheet("color: #1D4ED8; background: #DBEAFE; padding: 4px 10px; border-radius: 6px; font-weight: bold; font-size: 18px;");
        lblStatus->setText(status);
        lblStatus->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

        infoL->addWidget(lblTitle);
        infoL->addWidget(lblStartTime);
        infoL->addWidget(lblEndTime);
        infoL->addWidget(lblStatus);

        cardL->addLayout(infoL);
        cardL->addStretch();

        // 按钮组装
        CapzeraButton *btn = new CapzeraButton(status == "未参加" ? "开始考试" : (status == "已交卷" ? "✅ 查看详情" : "▶ 继续考试"), status == "已交卷" ? CapzeraButton::Secondary : status == "未参加" ? CapzeraButton::Primary : CapzeraButton::Success);
        btn->setFixedSize(140, 45);
        if (status == "已交卷") {
            btn->setEnabled(false);
            btn->setText("已交卷");
        } else {
            connect(btn, &QPushButton::clicked, this, [this, examId]() { emit startExamRequested(examId); });
        }

        cardL->addWidget(btn);
        m_containerLayout->addWidget(card);
    }
    m_containerLayout->addStretch();
}