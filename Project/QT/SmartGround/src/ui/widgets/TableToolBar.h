#ifndef TABLETOOLBAR_H
#define TABLETOOLBAR_H
#include <QWidget>
#include <QSet>

class CanvasTableItem;
class TableIconButton;
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
    TableIconButton* m_alignCenter = nullptr;
    TableIconButton* m_alignLeft = nullptr;
    TableIconButton* m_alignRight = nullptr;
    TableIconButton* m_alignTop = nullptr;
    TableIconButton* m_cell = nullptr;
    TableIconButton* m_row = nullptr;
    TableIconButton* m_col = nullptr;
    TableIconButton* m_insertRow = nullptr;
    TableIconButton* m_insertCol = nullptr;
    TableIconButton* m_deleteRow = nullptr;
    TableIconButton* m_deleteCol = nullptr;
    QMenu* m_insertMenu = nullptr;
    QMenu* m_deleteMenu = nullptr;
};

#endif
