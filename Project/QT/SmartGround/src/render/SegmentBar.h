#ifndef SEGMENTBAR_H
#define SEGMENTBAR_H
#include <QGraphicsObject>
#include <QColor>
#include <QString>

class SegmentBar : public QGraphicsObject {
    Q_OBJECT
public:
    enum { Type = UserType + 21 };
    int type() const override { return Type; }

    SegmentBar(double x1, double x2, double height);

    void setBar(const QString& text);
    void setColor(const QColor& c);
    void setMuted(bool on);
    bool isMuted() const { return m_muted; }
    void setRadius(qreal r);
    void setFontSize(int size);

    double barWidth() const { return m_x2 - m_x1; }
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;

private:
    double m_x1 = 0.0;
    double m_x2 = 0.0;
    double m_height = 30.0;
    qreal m_radius = 8.0;
    QColor m_color = QColor(206, 218, 234);
    QString m_text;
    int m_fontSize = 12;
    bool m_muted = false;
};

#endif
