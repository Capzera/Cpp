#ifndef CANVASTABLEITEM_H
#define CANVASTABLEITEM_H
#include "AnnotationItem.h"
#include <QVector>
#include <QString>

class CanvasTableItem : public AnnotationItem {
    Q_OBJECT
public:
    enum { Type = UserType + 26 };
    int type() const override { return Type; }

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

    bool beginEditCell(int r, int c);
    bool isEditingCell() const { return m_editingCell; }
    void commitEditCell();
    void cancelEditCell();

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;
    void applyStyle() override;
    QRectF contentBounds() const override;
    QJsonObject toJson() const override;
    void fromJson(const QJsonObject& obj) override;

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

private:
    void rebuildCells();
    int hitCell(const QPointF& local) const;
    qreal headerHeight() const;

    int m_rows = 3;
    int m_cols = 3;
    qreal m_cell = 46.0;
    QString m_title;
    QVector<QVector<QString>> m_data;
    QGraphicsTextItem* m_editor = nullptr;
    int m_editRow = -1;
    int m_editCol = -1;
    bool m_editingCell = false;
};

#endif
