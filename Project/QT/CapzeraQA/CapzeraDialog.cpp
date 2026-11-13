#include "CapzeraDialog.h"
#include <QApplication>

CapzeraDialog::CapzeraDialog(const QString &title, const QString &text, const QString &typeColor, const QString &iconText, ButtonType btnType, QWidget *parent)
    : QDialog(parent), m_result(false)
{
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setMinimumWidth(380);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    QWidget *card = new QWidget(this);
    // 🚀 核心修复：在最顶层父容器强行阻断 QLabel 的边框污染！
    card->setStyleSheet(QString("QWidget#Card { background: white; border-radius: 12px; border-top: 5px solid %1; } QLabel { border: none; background: transparent; }").arg(typeColor));
    card->setObjectName("Card");

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20); shadow->setColor(QColor(0, 0, 0, 40)); shadow->setOffset(0, 4);
    card->setGraphicsEffect(shadow);

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(25, 25, 25, 20); cardLayout->setSpacing(15);

    QHBoxLayout *titleLayout = new QHBoxLayout();
    QLabel *iconLabel = new QLabel(iconText); iconLabel->setFont(QFont("Microsoft YaHei", 18));
    QLabel *titleLabel = new QLabel(title); titleLabel->setFont(QFont("Microsoft YaHei", 15, QFont::Bold)); titleLabel->setStyleSheet("color: #1E293B;");
    titleLayout->addWidget(iconLabel); titleLayout->addWidget(titleLabel); titleLayout->addStretch();
    cardLayout->addLayout(titleLayout);

    QLabel *textLabel = new QLabel(text); textLabel->setFont(QFont("Microsoft YaHei", 14));
    textLabel->setStyleSheet("color: #475569; line-height: 1.5;"); textLabel->setWordWrap(true); textLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    cardLayout->addWidget(textLabel, 1);

    QHBoxLayout *btnLayout = new QHBoxLayout(); btnLayout->addStretch();
    QString btnBaseQss = "QPushButton { font-family: 'Microsoft YaHei'; font-size: 14px; font-weight: bold; border-radius: 6px; padding: 8px 20px; }";

    if (btnType == Ok) {
        QPushButton *btnOk = new QPushButton("我知道了"); btnOk->setCursor(Qt::PointingHandCursor);
        btnOk->setStyleSheet(btnBaseQss + QString("QPushButton { background: %1; color: white; border: none; } QPushButton:hover { opacity: 0.8; }").arg(typeColor));
        connect(btnOk, &QPushButton::clicked, [this]() { m_result = true; accept(); }); btnLayout->addWidget(btnOk);
    } else if (btnType == YesNo) {
        QPushButton *btnCancel = new QPushButton("取消"); btnCancel->setCursor(Qt::PointingHandCursor);
        btnCancel->setStyleSheet(btnBaseQss + "QPushButton { background: #F1F5F9; color: #475569; border: 1px solid #CBD5E1; } QPushButton:hover { background: #E2E8F0; }");
        QPushButton *btnYes = new QPushButton("确定"); btnYes->setCursor(Qt::PointingHandCursor);
        btnYes->setStyleSheet(btnBaseQss + QString("QPushButton { background: %1; color: white; border: none; } QPushButton:hover { opacity: 0.8; }").arg(typeColor));
        connect(btnCancel, &QPushButton::clicked, [this]() { m_result = false; reject(); });
        connect(btnYes, &QPushButton::clicked, [this]() { m_result = true; accept(); });
        btnLayout->addWidget(btnCancel); btnLayout->addWidget(btnYes);
    }
    cardLayout->addLayout(btnLayout); mainLayout->addWidget(card);
}

void CapzeraDialog::information(QWidget *parent, const QString &title, const QString &text) { CapzeraDialog dlg(title, text, "#3B82F6", "💡", Ok, parent); dlg.exec(); }
void CapzeraDialog::warning(QWidget *parent, const QString &title, const QString &text) { CapzeraDialog dlg(title, text, "#F59E0B", "⚠️", Ok, parent); dlg.exec(); }
void CapzeraDialog::critical(QWidget *parent, const QString &title, const QString &text) { CapzeraDialog dlg(title, text, "#EF4444", "❌", Ok, parent); dlg.exec(); }
bool CapzeraDialog::question(QWidget *parent, const QString &title, const QString &text) { CapzeraDialog dlg(title, text, "#10B981", "❓", YesNo, parent); return dlg.exec() == QDialog::Accepted; }
void CapzeraDialog::mousePressEvent(QMouseEvent *event) { if (event->button() == Qt::LeftButton) m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft(); event->accept(); }
void CapzeraDialog::mouseMoveEvent(QMouseEvent *event) { if (event->buttons() & Qt::LeftButton) move(event->globalPosition().toPoint() - m_dragPosition); event->accept(); }