#ifndef MYSLIDER_H
#define MYSLIDER_H
#include <QAbstractSlider>
#include <QColor>

class MySlider : public QAbstractSlider {
    Q_OBJECT
    Q_PROPERTY(qreal glow READ glow WRITE setGlow)
public:
    explicit MySlider(QWidget* parent = nullptr);

    int minimum() const { return m_min; }
    int maximum() const { return m_max; }
    void setRange(int min, int max);
    void setValue(int value);
    int value() const { return m_value; }

    qreal glow() const { return m_glow; }
    void setGlow(qreal v);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

signals:
    void valueChanged(int value);
    void sliderReleased(int value);
    void sliderPressed(int value);

protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void wheelEvent(QWheelEvent*) override;
    void keyPressEvent(QKeyEvent*) override;
    void enterEvent(QEnterEvent*) override;
    void leaveEvent(QEvent*) override;

private:
    QRectF trackRect() const;
    QRectF handleRect() const;
    qreal handleCenterX(qreal t) const;
    int valueAtX(qreal x) const;
    void emitIfChanged(int value);

    int m_min = 0;
    int m_max = 0;
    int m_value = 0;
    qreal m_glow = 0.0;
    qreal m_hover = 0.0;
    bool m_dragging = false;
};

#endif
