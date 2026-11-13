#ifndef CAPZERATOGGLE_H
#define CAPZERATOGGLE_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QColor>
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>

// 🚀 高定 iOS 级滑动开关控件
class CapzeraToggle : public QWidget {
    Q_OBJECT
    Q_PROPERTY(int sliderPosition READ sliderPosition WRITE setSliderPosition)

public:
    explicit CapzeraToggle(QWidget *parent = nullptr);
    bool isChecked() const;
    void setChecked(bool checked, bool animated = true);

signals:
    void toggled(bool checked);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    int sliderPosition() const;
    void setSliderPosition(int pos);

    bool m_checked;
    int m_sliderPos;
    QPropertyAnimation *m_animation;
    bool m_isHovered;
};

#endif // CAPZERATOGGLE_H