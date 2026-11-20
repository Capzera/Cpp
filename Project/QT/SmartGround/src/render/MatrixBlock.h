#ifndef MATRIXBLOCK_H
#define MATRIXBLOCK_H
#include <QGraphicsObject>
#include <QVector>
#include <QString>
#include <QPointer>

class QGraphicsTextItem;

class MatrixBlock : public QGraphicsObject {
    Q_OBJECT
public:
    enum { Type = UserType + 7 };
    enum Shape { PlainMatrix, Determinant, BracketedTable };

    int type() const override { return Type; }
    int rows = 1;
    int cols = 1;
    double cell = 52.0;
    double cellHeight = 52.0;
    QVector<QVector<QString>> data;
    QString title;
    QColor headerColor = QColor(226, 234, 248);
    QColor cellColor = QColor(255, 255, 255);
    QColor gridColor = QColor(150, 162, 180);
    QColor barColor = QColor(40, 60, 95);
    Shape shape = PlainMatrix;

    explicit MatrixBlock(int r, int c, QGraphicsItem* parent = nullptr);
    void setTitle(const QString& text);
    void setShape(Shape s);
    Shape blockShape() const { return shape; }
    void setCell(int r, int c, const QString& text);
    QString cellAt(int r, int c) const;
    void fillRandom(int seed);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    void setEditable(bool on);
    bool isEditable() const { return m_editable; }
    bool cellAtPoint(const QPointF& localPos, int& row, int& col) const;
    void resizeGrid(int rows, int cols);
    void setCellSize(double size) { if (qFuzzyCompare(cell, size)) return; prepareGeometryChange(); cell = size; update(); }
    void setCellHeight(double height) { if (qFuzzyCompare(cellHeight, height)) return; prepareGeometryChange(); cellHeight = height; update(); }
    double cellWidth() const { return cell; }
    double cellHeightValue() const { return cellHeight; }
    double headerHeight() const;
    double sideMargin() const;

signals:
    void cellEdited(int row, int col, const QString& text);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

private:
    void beginCellEdit(int row, int col);
    void commitCellEdit();
    void cancelCellEdit();

    bool m_editable = false;
    int m_editRow = -1;
    int m_editCol = -1;
    QPointer<QGraphicsTextItem> m_editor;
};

#endif
