#ifndef MYBUTTON_H
#define MYBUTTON_H
#include <QAbstractButton>
#include <QColor>
#include <QString>

class MyButton : public QAbstractButton {
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText)
    Q_PROPERTY(QColor accentColor READ accentColor WRITE setAccentColor)
    Q_PROPERTY(bool dangerMode READ isDangerMode WRITE setDangerMode)
public:
    enum Variant { Solid, Soft, Ghost };
    Q_ENUM(Variant)

    explicit MyButton(const QString& text = QString(), QWidget* parent = nullptr);

    QString text() const { return m_text; }
    void setText(const QString& text);
    QColor accentColor() const { return m_accent; }
    void setAccentColor(const QColor& c);
    bool isDangerMode() const { return m_danger; }
    void setDangerMode(bool on);
    Variant variant() const { return m_variant; }
    void setVariant(Variant v);
    void setIconGlyph(const QString& glyph);
    void setCompact(bool on);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    virtual void paintContent(QPainter& painter, const QRectF& rect) const;
    void paintEvent(QPaintEvent*) override;
    void enterEvent(QEnterEvent*) override;
    void leaveEvent(QEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void keyPressEvent(QKeyEvent*) override;

private:
    QString m_text;
    QString m_glyph;
    QColor m_accent;
    Variant m_variant = Solid;
    bool m_danger = false;
    bool m_compact = false;
    bool m_hover = false;
    bool m_pressed = false;
};

#endif
