#ifndef LATEXBLOCK_H
#define LATEXBLOCK_H
#include <QGraphicsObject>
#include <QString>
#include <QVector>

class LatexBlock : public QGraphicsObject {
    Q_OBJECT
public:
    enum { Type = UserType + 8 };
    int type() const override { return Type; }
    struct Formula {
        QString tex;
        QString svg;
        double width = 0.0;
        double height = 0.0;
    };

    explicit LatexBlock(QGraphicsItem* parent = nullptr);

    static bool loadLibrary(const QString& jsonPath);
    static const QVector<Formula>& library();

    void setFormulaIndex(int index);
    int formulaIndex() const { return m_index; }
    void setScaleFactor(qreal s);
    qreal scaleFactor() const { return m_scale; }
    void setTargetHeight(qreal h);
    void setTint(const QColor& color);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

private:
    int m_index = 0;
    qreal m_scale = 1.0;
    qreal m_targetHeight = 0.0;
    QColor m_tint = QColor(31, 39, 51);
};

#endif
