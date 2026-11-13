#include "CapzeraToggle.h"

CapzeraToggle::CapzeraToggle(QWidget *parent)
    : QWidget(parent), m_checked(false), m_sliderPos(3), m_isHovered(false)
{
    setFixedSize(50, 26); // 标准 iOS 开关比例
    setCursor(Qt::PointingHandCursor);

    m_animation = new QPropertyAnimation(this, "sliderPosition", this);
    m_animation->setEasingCurve(QEasingCurve::OutBack); // 物理阻尼曲线
    m_animation->setDuration(250);
}

bool CapzeraToggle::isChecked() const { return m_checked; }

void CapzeraToggle::setChecked(bool checked, bool animated) {
    if (m_checked == checked) return;
    m_checked = checked;
    int endPos = m_checked ? width() - height() + 3 : 3;

    if (animated) {
        m_animation->stop();
        m_animation->setStartValue(m_sliderPos);
        m_animation->setEndValue(endPos);
        m_animation->start();
    } else {
        setSliderPosition(endPos);
    }
    emit toggled(m_checked);
}

int CapzeraToggle::sliderPosition() const { return m_sliderPos; }
void CapzeraToggle::setSliderPosition(int pos) { m_sliderPos = pos; update(); }

void CapzeraToggle::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制背景跑道
    QPainterPath path;
    path.addRoundedRect(rect(), height() / 2, height() / 2);

    QColor bgColor = m_checked ? QColor("#10B981") : QColor("#E2E8F0"); // 绿 ON / 灰 OFF
    if (m_isHovered && !m_checked) bgColor = QColor("#CBD5E1"); // 悬浮加深
    if (m_isHovered && m_checked) bgColor = QColor("#059669");

    painter.fillPath(path, bgColor);

    // 绘制滑块圆球
    QColor sliderColor = Qt::white;
    int sliderSize = height() - 6;
    QRect sliderRect(m_sliderPos, 3, sliderSize, sliderSize);

    // 给滑块加微阴影
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 40));
    painter.drawEllipse(sliderRect.translated(0, 1));

    painter.setBrush(sliderColor);
    painter.drawEllipse(sliderRect);
}

void CapzeraToggle::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) setChecked(!m_checked);
    QWidget::mouseReleaseEvent(event);
}

void CapzeraToggle::enterEvent(QEnterEvent *event) { m_isHovered = true; update(); QWidget::enterEvent(event); }
void CapzeraToggle::leaveEvent(QEvent *event) { m_isHovered = false; update(); QWidget::leaveEvent(event); }