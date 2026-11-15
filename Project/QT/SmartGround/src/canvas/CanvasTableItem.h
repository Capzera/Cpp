#ifndef CANVASTABLEITEM_H
#define CANVASTABLEITEM_H
#include "AnnotationItem.h"
#include <QVector>
#include <QString>
#include <QSet>

class QGraphicsTextItem;

enum class TableAlign { Unset, Center, Left, Right, Top };

class CanvasTableItem : public AnnotationItem {
    Q_OBJECT
public:
    enum { Type = UserType + 26 };
    int type() const override { return Type; }

    using ContentAlign = TableAlign;

    explicit CanvasTableItem(int rows = 3, int cols = 3, QGraphicsItem* parent = nullptr);

    int rows() const { return m_rows; }
    int cols() const { return m_cols; }
    void setDimensions(int rows, int cols);

    QString cellAt(int r, int c) const;
    void setCell(int r, int c, const QString& text);

    void setTitle(const QString& title);
    QString title() const { return m_title; }

    qreal cellSize() const { return m_cell; }
    void setCellSize(qreal s);
    qreal headerHeight() const;

    bool beginEditCell(int r, int c);
    bool isEditingCell() const { return m_editingCell; }
    void commitEditCell();
    void cancelEditCell();

    void setContentAlign(TableAlign align);
    TableAlign contentAlign() const;

    void setCellAlign(int r, int c, TableAlign align);
    TableAlign cellAlign(int r, int c) const;

    void selectCells(const QSet<int>& cells);
    void clearCellSelection();
    const QSet<int>& selectedCells() const { return m_selectedCells; }
    int selectedRow() const { return m_selectedRow; }
    int selectedCol() const { return m_selectedCol; }
    void setSelectedRow(int r);
    void setSelectedCol(int c);

    bool insertRows(int at, int count);
    bool insertColumns(int at, int count);
    bool removeRows(int at, int count);
    bool removeColumns(int at, int count);

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;
    void applyStyle() override;
    QRectF contentBounds() const override;
    AnnotationItem* cloneItem() const override;
    QJsonObject toJson() const override;
    void fromJson(const QJsonObject& obj) override;

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

private:
    void rebuildCells();
    int hitCell(const QPointF& local) const;
    void drawCellText(QPainter* painter, const QRectF& r, const QString& text, TableAlign align) const;

    int m_rows = 3;
    int m_cols = 3;
    qreal m_cell = 46.0;
    QString m_title;
    QVector<QVector<QString>> m_data;
    QVector<QVector<ContentAlign>> m_aligns;
    ContentAlign m_align = TableAlign::Unset;
    QSet<int> m_selectedCells;
    int m_selectedRow = -1;
    int m_selectedCol = -1;
    QGraphicsTextItem* m_editor = nullptr;
    int m_editRow = -1;
    int m_editCol = -1;
    bool m_editingCell = false;
};

#endif
