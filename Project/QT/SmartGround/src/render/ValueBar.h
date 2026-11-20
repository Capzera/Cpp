#ifndef VALUEBAR_H
#define VALUEBAR_H
#include <QGraphicsObject>
#include <QColor>
#include <QString>

class ValueBar : public QGraphicsObject {
    Q_OBJECT
public:
    enum { Type = UserType + 20 };
    int type() const override { return Type; }

    ValueBar(double maxWidth = 300.0);

    void setValue(double v);
    double value() const { return m_value; }
    void setMaxValue(double v);
    double maxValue() const { return m_maxValue; }
    void setMaxWidth(double w);
    double maxWidth() const { return m_maxWidth; }
    void setColor(const QColor& c);
    void setLabel(const QString& text);
    void setSubLabel(const QString& text);
    void setValueFontSize(int size);
    void setGhost(bool on);
    bool isGhost() const { return m_ghost; }
    void setDimmed(bool on);
    bool isDimmed() const { return m_dimmed; }
    void setFlip(bool on);

    double barWidth() const;
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;

private:
    double m_value = 0.0;
    double m_maxValue = 1.0;
    double m_maxWidth = 300.0;
    double m_height = 46.0;
    QColor m_color = QColor(120, 165, 230);
    QString m_label;
    QString m_subLabel;
    int m_valueFontSize = 15;
    bool m_ghost = false;
    bool m_dimmed = false;
    bool m_flip = false;
};

#endif
