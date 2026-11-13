#include "CapzeraButton.h"

CapzeraButton::CapzeraButton(const QString &text, Theme theme, QWidget *parent)
    : QPushButton(text, parent), m_theme(theme) 
{
    setCursor(Qt::PointingHandCursor);
    setFont(QFont("Microsoft YaHei", 14, QFont::Bold));
    setFixedHeight(45);

    // 分配大厂调色盘
    switch(theme) {
        case Primary:   m_normalColor = QColor("#3B82F6"); m_hoverColor = QColor("#2563EB"); break;
        case Success:   m_normalColor = QColor("#10B981"); m_hoverColor = QColor("#059669"); break;
        case Warning:   m_normalColor = QColor("#F59E0B"); m_hoverColor = QColor("#D97706"); break;
        case Danger:    m_normalColor = QColor("#EF4444"); m_hoverColor = QColor("#DC2626"); break;
        case Secondary: m_normalColor = QColor("#F1F5F9"); m_hoverColor = QColor("#E2E8F0"); break;
    }
    
    m_currentColor = m_normalColor;
    setBgColor(m_normalColor);

    m_anim = new QPropertyAnimation(this, "bgColor", this);
    m_anim->setDuration(200); // 0.2秒丝滑过渡
}

QColor CapzeraButton::bgColor() const { return m_currentColor; }

void CapzeraButton::setBgColor(const QColor &color) {
    m_currentColor = color;
    QString colorStr = color.name(QColor::HexArgb);
    
    // 如果是次要按钮，文字用深灰色；其他主题文字用白色
    QString textColor = (m_theme == Secondary) ? "#475569" : "white";
    QString borderStr = (m_theme == Secondary) ? "border: 1px solid #CBD5E1;" : "border: none;";

    setStyleSheet(QString("QPushButton { background-color: %1; color: %2; %3 border-radius: 8px; padding: 0 12px; }")
                    .arg(colorStr, textColor, borderStr));
}

void CapzeraButton::enterEvent(QEnterEvent *event) {
    m_anim->stop();
    m_anim->setStartValue(m_currentColor);
    m_anim->setEndValue(m_hoverColor);
    m_anim->start();
    QPushButton::enterEvent(event);
}

void CapzeraButton::leaveEvent(QEvent *event) {
    m_anim->stop();
    m_anim->setStartValue(m_currentColor);
    m_anim->setEndValue(m_normalColor);
    m_anim->start();
    QPushButton::leaveEvent(event);
}

void CapzeraButton::setFontSize(int x) {
    setFont(QFont("Microsoft YaHei", x, QFont::Bold));
}

void CapzeraButton::setTheme(Theme n_theme) {
    m_theme = n_theme;
    switch(n_theme) {
    case Primary:   m_normalColor = QColor("#3B82F6"); m_hoverColor = QColor("#2563EB"); break;
    case Success:   m_normalColor = QColor("#10B981"); m_hoverColor = QColor("#059669"); break;
    case Warning:   m_normalColor = QColor("#F59E0B"); m_hoverColor = QColor("#D97706"); break;
    case Danger:    m_normalColor = QColor("#EF4444"); m_hoverColor = QColor("#DC2626"); break;
    case Secondary: m_normalColor = QColor("#F1F5F9"); m_hoverColor = QColor("#E2E8F0"); break;
    }

    m_anim->stop();

    setBgColor(m_normalColor);
}