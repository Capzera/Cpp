#ifndef TABLETOOLBAR_H
#define TABLETOOLBAR_H
#include <QWidget>
#include <QSet>

class CanvasTableItem;
class MyButton;
class QMenu;

class TableToolBar : public QWidget {
    Q_OBJECT
public:
    explicit TableToolBar(QWidget* parent = nullptr);

    void setTable(CanvasTableItem* table);
    CanvasTableItem* table() const { return m_table; }

    void showContextMenu(const QPoint& globalPos, int row, int col);

    int currentRow() const;
    int currentCol() const;

    void applyAlign(int mode);
    void selectCell();
    void selectRow();
    void selectColumn();
    void insertRowsAbove();
    void insertRowsBelow();
    void insertColumnsLeft();
    void insertColumnsRight();
    void deleteCurrentRow();
    void deleteCurrentColumn();
    void promptInsertRows();
    void promptInsertCols();

signals:
    void tableChanged();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    void buildUi();
    void rebuildMenus();
    void refreshButtons();

    CanvasTableItem* m_table = nullptr;
    QWidget* m_bar = nullptr;
    MyButton* m_alignCenter = nullptr;
    MyButton* m_alignLeft = nullptr;
    MyButton* m_alignRight = nullptr;
    MyButton* m_alignTop = nullptr;
    MyButton* m_cell = nullptr;
    MyButton* m_row = nullptr;
    MyButton* m_col = nullptr;
    MyButton* m_insert = nullptr;
    MyButton* m_delete = nullptr;
    QMenu* m_insertMenu = nullptr;
    QMenu* m_deleteMenu = nullptr;
};

#endif
