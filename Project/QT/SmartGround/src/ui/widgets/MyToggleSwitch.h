#ifndef MYTOGGLESWITCH_H
#define MYTOGGLESWITCH_H
#include <QAbstractButton>
#include <QColor>
#include <QPropertyAnimation>

class MyToggleSwitch : public QAbstractButton {
    Q_OBJECT
    Q_PROPERTY(qreal position READ position WRITE setPosition)
    Q_PROPERTY(bool checked READ isChecked WRITE setChecked NOTIFY toggled)
    Q_PROPERTY(QString offText READ offText WRITE setOffText)
    Q_PROPERTY(QString onText READ onText WRITE setOnText)
    Q_PROPERTY(QColor onColor READ onColor WRITE setOnColor)
    Q_PROPERTY(qreal ratio READ ratio WRITE setRatio)
public:
    explicit MyToggleSwitch(QWidget* parent = nullptr);

    bool isChecked() const { return m_checked; }
    void setChecked(bool on);
    qreal position() const { return m_position; }
    void setPosition(qreal v);
    QString offText() const { return m_offText; }
    void setOffText(const QString& text);
    QString onText() const { return m_onText; }
    void setOnText(const QString& text);
    QColor onColor() const { return m_onColor; }
    void setOnColor(const QColor& c);
    qreal ratio() const { return m_ratio; }
    void setRatio(qreal v);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

signals:
    void toggled(bool checked);

protected:
    void paintEvent(QPaintEvent*) override;
    void enterEvent(QEnterEvent*) override;
    void leaveEvent(QEvent*) override;
    void mousePressEvent(QMouseEvent*) override;

private:
    QRectF trackRect() const;
    QRectF knobRect(qreal t) const;

    bool m_checked = false;
    bool m_hover = false;
    qreal m_position = 0.0;
    qreal m_ratio = 1.0;
    QString m_offText;
    QString m_onText;
    QColor m_onColor;
    QPropertyAnimation* m_anim = nullptr;
};

#endif
