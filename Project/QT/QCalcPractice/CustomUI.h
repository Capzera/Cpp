#ifndef CUSTOMUI_H
#define CUSTOMUI_H

#include <QPushButton>
#include <QWidget>
#include <QColor>
#include <QPropertyAnimation>

// ================= 1. 大厂级渐变按钮 =================
class GradientButton : public QPushButton {
    Q_OBJECT
public:
    explicit GradientButton(const QString &text, QWidget *parent = nullptr);

    // 支持自定义渐变色，方便不同模式区分颜色（比如极限模式用红色系，无尽用蓝色系）
    void setGradientColors(const QColor &start, const QColor &end);

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override; // 注意：Qt6 这里是 QEnterEvent
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QColor m_startColor;
    QColor m_endColor;
    bool m_isHovered;
    bool m_isPressed;
};


// ================= 2. iOS 风格滑动开关 =================
class SlideSwitch : public QWidget {
    Q_OBJECT
    Q_PROPERTY(int circleOffset READ getCircleOffset WRITE setCircleOffset)

public:
    explicit SlideSwitch(QWidget *parent = nullptr);

    bool isChecked() const;
    void setChecked(bool checked, bool anim = false);

signals:
    void toggled(bool checked);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    int getCircleOffset() const;
    void setCircleOffset(int offset);
    void updateOffset();

    bool m_checked;
    int m_circleOffset;
    QPropertyAnimation *m_anim;
};

#endif // CUSTOMUI_H