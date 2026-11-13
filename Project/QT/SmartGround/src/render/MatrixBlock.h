#ifndef MATRIXBLOCK_H
#define MATRIXBLOCK_H
#include <QGraphicsObject>
#include <QVector>
#include <QString>
class MatrixBlock : public QGraphicsObject {
    Q_OBJECT
public:
    enum { Type = UserType + 7 };
    int type() const override { return Type; }
    int rows = 1;
    int cols = 1;
    double cell = 46.0;
    QVector<QVector<QString>> data;
    QString title;
    QColor headerColor = QColor(226, 234, 248);
    QColor cellColor = QColor(255, 255, 255);
    QColor gridColor = QColor(150, 162, 180);

    explicit MatrixBlock(int r, int c, QGraphicsItem* parent = nullptr);
    void setTitle(const QString& text);
    void setCell(int r, int c, const QString& text);
    QString cellAt(int r, int c) const;
    void fillRandom(int seed);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

private:
    double headerHeight() const;
};

#endif
