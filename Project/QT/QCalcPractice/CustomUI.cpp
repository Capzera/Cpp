#include "CustomUI.h"
#include <QPainter>
#include <QPainterPath>
#include <QEnterEvent>
#include <QMouseEvent>

// ================= GradientButton 实现 =================
GradientButton::GradientButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent), m_isHovered(false), m_isPressed(false) {
    // 默认蓝紫色渐变，充满现代感
    m_startColor = QColor(106, 17, 203);
    m_endColor = QColor(37, 117, 252);

    // 设置基础属性
    setCursor(Qt::PointingHandCursor);
    // 去除 Qt 默认的按钮焦点虚线框
    setFocusPolicy(Qt::NoFocus);
}

void GradientButton::setGradientColors(const QColor &start, const QColor &end) {
    m_startColor = start;
    m_endColor = end;
    update();
}

void GradientButton::enterEvent(QEnterEvent *event) {
    m_isHovered = true;
    update();
    QPushButton::enterEvent(event);
}

void GradientButton::leaveEvent(QEvent *event) {
    m_isHovered = false;
    update();
    QPushButton::leaveEvent(event);
}

void GradientButton::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_isPressed = true;
        update();
    }
    QPushButton::mousePressEvent(event);
}

void GradientButton::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_isPressed = false;
        update();
    }
    QPushButton::mouseReleaseEvent(event);
}

void GradientButton::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 核心动效计算：按下时按钮整体下沉 2px，悬浮时颜色变量
    QRect drawRect = rect();
    if (m_isPressed) {
        drawRect.translate(0, 2);
    }

    QColor currentStart = m_startColor;
    QColor currentEnd = m_endColor;

    if (m_isHovered && !m_isPressed) {
        // 悬浮时颜色提亮 110%
        currentStart = currentStart.lighter(110);
        currentEnd = currentEnd.lighter(110);
    }

    // 绘制渐变背景
    QLinearGradient gradient(drawRect.topLeft(), drawRect.bottomRight());
    gradient.setColorAt(0.0, currentStart);
    gradient.setColorAt(1.0, currentEnd);

    painter.setPen(Qt::NoPen);
    painter.setBrush(gradient);
    painter.drawRoundedRect(drawRect, 10, 10); // 10px 圆角

    // 绘制文字
    painter.setPen(Qt::white); // 强制白字，最高级
    painter.setFont(font());
    painter.drawText(drawRect, Qt::AlignCenter, text());
}


// ================= SlideSwitch 实现 =================
SlideSwitch::SlideSwitch(QWidget *parent)
    : QWidget(parent), m_checked(false), m_circleOffset(3) {

    setFixedSize(60, 32); // 经典比例
    setCursor(Qt::PointingHandCursor);

    m_anim = new QPropertyAnimation(this, "circleOffset", this);
    m_anim->setDuration(150); // 150毫秒丝滑滑动
    m_anim->setEasingCurve(QEasingCurve::InOutQuad);
}

bool SlideSwitch::isChecked() const {
    return m_checked;
}

void SlideSwitch::setChecked(bool checked, bool anim) {
    if (m_checked == checked) return;

    m_checked = checked;
    if (anim) {
        m_anim->stop();
        m_anim->setStartValue(m_circleOffset);
        m_anim->setEndValue(m_checked ? width() - height() + 3 : 3);
        m_anim->start();
    } else {
        updateOffset();
    }

    emit toggled(m_checked);
}

void SlideSwitch::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        setChecked(!m_checked, true);
    }
    QWidget::mouseReleaseEvent(event);
}

void SlideSwitch::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制背景槽（开为绿色，关为灰色）
    QColor bgColor = m_checked ? QColor(52, 199, 89) : QColor(229, 229, 234);
    painter.setPen(Qt::NoPen);
    painter.setBrush(bgColor);
    painter.drawRoundedRect(rect(), height() / 2, height() / 2);

    // 绘制白色小滑块
    painter.setBrush(Qt::white);

    // 给滑块加一点极浅的阴影提升质感
    painter.setPen(QPen(QColor(0, 0, 0, 30), 1));
    int r = height() - 6;
    painter.drawEllipse(m_circleOffset, 3, r, r);
}

int SlideSwitch::getCircleOffset() const {
    return m_circleOffset;
}

void SlideSwitch::setCircleOffset(int offset) {
    m_circleOffset = offset;
    update();
}

void SlideSwitch::updateOffset() {
    m_circleOffset = m_checked ? width() - height() + 3 : 3;
    update();
}