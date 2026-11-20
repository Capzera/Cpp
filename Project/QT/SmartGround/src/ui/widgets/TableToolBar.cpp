#include "TableToolBar.h"
#include "TableIconButton.h"
#include "MyTextInput.h"
#include "ui/UiTheme.h"
#include "canvas/CanvasTableItem.h"
#include "canvas/CanvasView.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QMenu>
#include <QAction>
#include <QEvent>
#include <QDialog>
#include <QDialogButtonBox>

namespace {

class TableCard : public QWidget {
public:
    explicit TableCard(QWidget* parent = nullptr) : QWidget(parent) {
        setAttribute(Qt::WA_TranslucentBackground, true);
    }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        const QRectF box = QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5);
        QPainterPath path;
        path.addRoundedRect(box, 11, 11);
        painter.setPen(QPen(QColor(0, 0, 0, 30), 1));
        painter.setBrush(QColor(255, 255, 255, 252));
        painter.drawPath(path);
    }
};

}  // namespace

TableToolBar::TableToolBar(QWidget* parent) : QWidget(parent) {
    setObjectName("tableToolBar");
    setAttribute(Qt::WA_TranslucentBackground, true);
    buildUi();
    hide();
}

void TableToolBar::buildUi() {
    m_bar = new TableCard(this);
    m_bar->setObjectName("tableToolBarCard");

    QHBoxLayout* layout = new QHBoxLayout(m_bar);
    layout->setContentsMargins(7, 5, 7, 5);
    layout->setSpacing(2);

    auto addButton = [this, layout](TableIconButton::Icon icon, const QString& tip, const QString& name,
                                    const std::function<void()>& sink) {
        TableIconButton* button = new TableIconButton(icon, tip, m_bar);
        button->setObjectName(name);
        connect(button, &QAbstractButton::clicked, this, [sink]() { sink(); });
        layout->addWidget(button);
        return button;
    };

    auto addSeparator = [layout]() {
        QFrame* line = new QFrame();
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Plain);
        line->setFixedHeight(16);
        line->setStyleSheet("color: rgba(150,162,180,90);");
        layout->addWidget(line);
        layout->addSpacing(3);
    };

    m_alignLeft = addButton(TableIconButton::AlignLeft, "左对齐", "tableAlignLeft", [this]() { applyAlign(1); });
    m_alignCenter = addButton(TableIconButton::AlignCenter, "居中对齐", "tableAlignCenter", [this]() { applyAlign(0); });
    m_alignRight = addButton(TableIconButton::AlignRight, "右对齐", "tableAlignRight", [this]() { applyAlign(2); });
    m_alignTop = addButton(TableIconButton::AlignTop, "顶端对齐", "tableAlignTop", [this]() { applyAlign(3); });

    addSeparator();

    m_cell = addButton(TableIconButton::SelectCell, "选中单元格", "tableSelectCell", [this]() { selectCell(); });
    m_row = addButton(TableIconButton::SelectRow, "选中行", "tableSelectRow", [this]() { selectRow(); });
    m_col = addButton(TableIconButton::SelectColumn, "选中列", "tableSelectCol", [this]() { selectColumn(); });

    addSeparator();

    m_insertRow = addButton(TableIconButton::InsertRow, "插入行", "tableInsertRow", [this]() {
        rebuildMenus();
        if (m_insertMenu) m_insertMenu->popup(m_insertRow->mapToGlobal(QPoint(0, m_insertRow->height())));
    });
    m_insertCol = addButton(TableIconButton::InsertColumn, "插入列", "tableInsertCol", [this]() {
        rebuildMenus();
        if (m_insertMenu) m_insertMenu->popup(m_insertCol->mapToGlobal(QPoint(0, m_insertCol->height())));
    });
    m_deleteRow = addButton(TableIconButton::DeleteRow, "删除行", "tableDeleteRow", [this]() {
        rebuildMenus();
        if (m_deleteMenu) m_deleteMenu->popup(m_deleteRow->mapToGlobal(QPoint(0, m_deleteRow->height())));
    });
    m_deleteCol = addButton(TableIconButton::DeleteColumn, "删除列", "tableDeleteCol", [this]() {
        rebuildMenus();
        if (m_deleteMenu) m_deleteMenu->popup(m_deleteCol->mapToGlobal(QPoint(0, m_deleteCol->height())));
    });

    QHBoxLayout* outer = new QHBoxLayout(this);
    outer->setContentsMargins(0, 0, 0, 0);
    outer->addWidget(m_bar);
}

void TableToolBar::setTable(CanvasTableItem* table) {
    if (m_table == table) return;
    m_table = table;
    if (!m_table) {
        hide();
        m_insertMenu = nullptr;
        m_deleteMenu = nullptr;
        return;
    }
    show();
    raise();
    refreshButtons();
}

int TableToolBar::currentRow() const {
    if (!m_table) return -1;
    const QSet<int>& cells = m_table->selectedCells();
    if (!cells.isEmpty()) {
        const int idx = *cells.constBegin();
        return idx / m_table->cols();
    }
    return m_table->selectedRow();
}

int TableToolBar::currentCol() const {
    if (!m_table) return -1;
    const QSet<int>& cells = m_table->selectedCells();
    if (!cells.isEmpty()) {
        const int idx = *cells.constBegin();
        return idx % m_table->cols();
    }
    return m_table->selectedCol();
}

void TableToolBar::applyAlign(int mode) {
    if (!m_table) return;
    TableAlign align = TableAlign::Center;
    switch (mode) {
    case 1: align = TableAlign::Left; break;
    case 2: align = TableAlign::Right; break;
    case 3: align = TableAlign::Top; break;
    default: align = TableAlign::Center; break;
    }
    const int r = currentRow();
    const int c = currentCol();
    if (r >= 0 && c >= 0) {
        m_table->setCellAlign(r, c, align);
    } else {
        m_table->setContentAlign(align);
    }
    refreshButtons();
    emit tableChanged();
}

void TableToolBar::selectCell() {
    if (!m_table) return;
    const int r = currentRow();
    const int c = currentCol();
    if (r < 0 || c < 0) return;
    QSet<int> cells;
    cells.insert(r * m_table->cols() + c);
    m_table->selectCells(cells);
    refreshButtons();
    emit tableChanged();
}

void TableToolBar::selectRow() {
    if (!m_table) return;
    const int r = currentRow();
    if (r < 0) return;
    QSet<int> cells;
    for (int c = 0; c < m_table->cols(); ++c) cells.insert(r * m_table->cols() + c);
    m_table->selectCells(cells);
    m_table->setSelectedRow(r);
    refreshButtons();
    emit tableChanged();
}

void TableToolBar::selectColumn() {
    if (!m_table) return;
    const int c = currentCol();
    if (c < 0) return;
    QSet<int> cells;
    for (int r = 0; r < m_table->rows(); ++r) cells.insert(r * m_table->cols() + c);
    m_table->selectCells(cells);
    m_table->setSelectedCol(c);
    refreshButtons();
    emit tableChanged();
}

void TableToolBar::insertRowsAbove() {
    if (!m_table) return;
    const int r = currentRow();
    m_table->insertRows(r < 0 ? 0 : r, 1);
    refreshButtons();
    emit tableChanged();
}

void TableToolBar::insertRowsBelow() {
    if (!m_table) return;
    const int r = currentRow();
    m_table->insertRows(r < 0 ? m_table->rows() : r + 1, 1);
    refreshButtons();
    emit tableChanged();
}

void TableToolBar::insertColumnsLeft() {
    if (!m_table) return;
    const int c = currentCol();
    m_table->insertColumns(c < 0 ? 0 : c, 1);
    refreshButtons();
    emit tableChanged();
}

void TableToolBar::insertColumnsRight() {
    if (!m_table) return;
    const int c = currentCol();
    m_table->insertColumns(c < 0 ? m_table->cols() : c + 1, 1);
    refreshButtons();
    emit tableChanged();
}

void TableToolBar::deleteCurrentRow() {
    if (!m_table) return;
    const int r = currentRow();
    if (r < 0) return;
    m_table->removeRows(r, 1);
    m_table->clearCellSelection();
    refreshButtons();
    emit tableChanged();
}

void TableToolBar::deleteCurrentColumn() {
    if (!m_table) return;
    const int c = currentCol();
    if (c < 0) return;
    m_table->removeColumns(c, 1);
    m_table->clearCellSelection();
    refreshButtons();
    emit tableChanged();
}

void TableToolBar::showContextMenu(const QPoint& globalPos, int row, int col) {
    if (!m_table) return;
    if (row >= 0 && col >= 0) {
        QSet<int> cells;
        cells.insert(row * m_table->cols() + col);
        m_table->selectCells(cells);
        refreshButtons();
        emit tableChanged();
    }
    QMenu menu(this);
    menu.setObjectName("tableContextMenu");
    QAction* up = menu.addAction("在上方插入行");
    connect(up, &QAction::triggered, this, [this]() { insertRowsAbove(); });
    QAction* down = menu.addAction("在下方插入行");
    connect(down, &QAction::triggered, this, [this]() { insertRowsBelow(); });
    QAction* left = menu.addAction("在左侧插入列");
    connect(left, &QAction::triggered, this, [this]() { insertColumnsLeft(); });
    QAction* right = menu.addAction("在右侧插入列");
    connect(right, &QAction::triggered, this, [this]() { insertColumnsRight(); });
    menu.addSeparator();
    QAction* kRows = menu.addAction("插入 k 行...");
    connect(kRows, &QAction::triggered, this, [this]() { promptInsertRows(); });
    QAction* kCols = menu.addAction("插入 k 列...");
    connect(kCols, &QAction::triggered, this, [this]() { promptInsertCols(); });
    menu.addSeparator();
    QAction* delRow = menu.addAction("删除当前行");
    delRow->setEnabled(currentRow() >= 0);
    connect(delRow, &QAction::triggered, this, [this]() { deleteCurrentRow(); });
    QAction* delCol = menu.addAction("删除当前列");
    delCol->setEnabled(currentCol() >= 0);
    connect(delCol, &QAction::triggered, this, [this]() { deleteCurrentColumn(); });
    menu.exec(globalPos);
}

void TableToolBar::rebuildMenus() {
    if (!m_table) return;
    const int r = currentRow();
    const int c = currentCol();

    if (!m_insertMenu) {
        m_insertMenu = new QMenu(this);
        m_insertMenu->setObjectName("tableInsertMenu");
    } else {
        m_insertMenu->clear();
    }
    if (!m_deleteMenu) {
        m_deleteMenu = new QMenu(this);
        m_deleteMenu->setObjectName("tableDeleteMenu");
    } else {
        m_deleteMenu->clear();
    }

    QAction* up = m_insertMenu->addAction("在上方插入行");
    connect(up, &QAction::triggered, this, [this]() { insertRowsAbove(); });
    QAction* down = m_insertMenu->addAction("在下方插入行");
    connect(down, &QAction::triggered, this, [this]() { insertRowsBelow(); });
    QAction* left = m_insertMenu->addAction("在左侧插入列");
    connect(left, &QAction::triggered, this, [this]() { insertColumnsLeft(); });
    QAction* right = m_insertMenu->addAction("在右侧插入列");
    connect(right, &QAction::triggered, this, [this]() { insertColumnsRight(); });
    m_insertMenu->addSeparator();
    QAction* kRows = m_insertMenu->addAction("插入 k 行...");
    connect(kRows, &QAction::triggered, this, [this]() { promptInsertRows(); });
    QAction* kCols = m_insertMenu->addAction("插入 k 列...");
    connect(kCols, &QAction::triggered, this, [this]() { promptInsertCols(); });

    QAction* delRow = m_deleteMenu->addAction("删除当前行");
    delRow->setEnabled(r >= 0);
    connect(delRow, &QAction::triggered, this, [this]() { deleteCurrentRow(); });
    QAction* delCol = m_deleteMenu->addAction("删除当前列");
    delCol->setEnabled(c >= 0);
    connect(delCol, &QAction::triggered, this, [this]() { deleteCurrentColumn(); });
}

void TableToolBar::promptInsertRows() {
    if (!m_table) return;
    QDialog dialog(this);
    dialog.setWindowTitle("插入行");
    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    QLabel* label = new QLabel("插入行数", &dialog);
    layout->addWidget(label);
    MyTextInput* input = new MyTextInput(&dialog);
    input->setText("1");
    input->setMaxLength(3);
    layout->addWidget(input);
    QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    layout->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    if (dialog.exec() != QDialog::Accepted) return;
    const int count = input->text().toInt();
    if (count <= 0) return;
    const int r = currentRow();
    m_table->insertRows(r < 0 ? m_table->rows() : r + 1, count);
    refreshButtons();
    emit tableChanged();
}

void TableToolBar::promptInsertCols() {
    if (!m_table) return;
    QDialog dialog(this);
    dialog.setWindowTitle("插入列");
    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    QLabel* label = new QLabel("插入列数", &dialog);
    layout->addWidget(label);
    MyTextInput* input = new MyTextInput(&dialog);
    input->setText("1");
    input->setMaxLength(3);
    layout->addWidget(input);
    QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    layout->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    if (dialog.exec() != QDialog::Accepted) return;
    const int count = input->text().toInt();
    if (count <= 0) return;
    const int c = currentCol();
    m_table->insertColumns(c < 0 ? m_table->cols() : c + 1, count);
    refreshButtons();
    emit tableChanged();
}

void TableToolBar::refreshButtons() {
    if (!m_table) return;
    const int r = currentRow();
    const int c = currentCol();
    m_row->setEnabled(r >= 0);
    m_col->setEnabled(c >= 0);
    m_cell->setEnabled(r >= 0 && c >= 0);
    m_insertRow->setEnabled(true);
    m_insertCol->setEnabled(true);
    m_deleteRow->setEnabled(r >= 0);
    m_deleteCol->setEnabled(c >= 0);
}

void TableToolBar::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);
}
