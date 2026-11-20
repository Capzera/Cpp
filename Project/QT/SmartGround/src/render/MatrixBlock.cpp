#include "MatrixBlock.h"
#include <QPainter>
#include <QFontMetrics>
#include <QFont>
#include <QPolygonF>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QTextCursor>
#include <QCursor>
#include <functional>
#include "ui/UiFonts.h"

namespace {
void installMatrixWatcher(QGraphicsTextItem* editor, std::function<void(bool)> onDone) {
    static QObject* keeper = new QObject();
    class Watcher : public QObject {
    public:
        explicit Watcher(QObject* parent) : QObject(parent) {}
        std::function<void(bool)> onDone;
        bool eventFilter(QObject* obj, QEvent* event) override {
            if (event->type() == QEvent::KeyPress) {
                QKeyEvent* ke = static_cast<QKeyEvent*>(event);
                if (ke->key() == Qt::Key_Return || ke->key() == Qt::Key_Enter) {
                    if (onDone) onDone(true);
                    return true;
                }
                if (ke->key() == Qt::Key_Escape) {
                    if (onDone) onDone(false);
                    return true;
                }
            }
            if (event->type() == QEvent::FocusOut) {
                if (onDone) onDone(true);
                return true;
            }
            return QObject::eventFilter(obj, event);
        }
    };
    Watcher* watcher = new Watcher(keeper);
    watcher->onDone = onDone;
    editor->installEventFilter(watcher);
    QObject::connect(editor, &QObject::destroyed, keeper, [watcher]() { watcher->deleteLater(); });
}
}

MatrixBlock::MatrixBlock(int r, int c, QGraphicsItem* parent) : QGraphicsObject(parent) {
    rows = qMax(1, r);
    cols = qMax(1, c);
    cellHeight = cell;
    data.resize(rows);
    for (int i = 0; i < rows; ++i) {
        data[i].resize(cols);
        for (int j = 0; j < cols; ++j) {
            data[i][j] = QString::number(i * cols + j + 1);
        }
    }
    setZValue(1);
    setFlag(QGraphicsItem::ItemIsFocusable, false);
}

void MatrixBlock::setTitle(const QString& text) {
    if (title == text) return;
    prepareGeometryChange();
    title = text;
    update();
}

void MatrixBlock::setShape(Shape s) {
    if (shape == s) return;
    prepareGeometryChange();
    shape = s;
    update();
}

void MatrixBlock::setCell(int r, int c, const QString& text) {
    if (r < 0 || r >= rows || c < 0 || c >= cols) return;
    data[r][c] = text;
    update();
}

QString MatrixBlock::cellAt(int r, int c) const {
    if (r < 0 || r >= rows || c < 0 || c >= cols) return QString();
    return data[r][c];
}

void MatrixBlock::fillRandom(int seed) {
    int state = qAbs(seed) + 1;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            state = (state * 1103515245 + 12345) & 0x7fffffff;
            data[i][j] = QString::number(state % 90 + 10);
        }
    }
    update();
}

double MatrixBlock::headerHeight() const {
    return title.isEmpty() ? 0.0 : 28.0;
}

double MatrixBlock::sideMargin() const {
    if (shape == Determinant) return 18.0;
    if (shape == BracketedTable) return 4.0;
    if (shape == PlainMatrix) return 26.0;
    return 0.0;
}

QRectF MatrixBlock::boundingRect() const {
    const double h = headerHeight();
    const double m = sideMargin();
    const double vPad = (shape == PlainMatrix) ? 13.0 : 4.0;
    return QRectF(-4 - m, -vPad - h, cols * cell + 8 + m * 2, rows * cellHeight + h + vPad * 2);
}

bool MatrixBlock::cellAtPoint(const QPointF& localPos, int& row, int& col) const {
    const double m = sideMargin();
    const double x = localPos.x() - m;
    const double y = localPos.y() - headerHeight();
    if (x < 0 || y < 0) return false;
    col = static_cast<int>(std::floor(x / cell));
    row = static_cast<int>(std::floor(y / cellHeight));
    return row >= 0 && row < rows && col >= 0 && col < cols;
}

void MatrixBlock::resizeGrid(int newRows, int newCols) {
    newRows = qMax(1, newRows);
    newCols = qMax(1, newCols);
    if (newRows == rows && newCols == cols) return;
    prepareGeometryChange();
    const QVector<QVector<QString>> oldData = data;
    rows = newRows;
    cols = newCols;
    data.resize(rows);
    for (int i = 0; i < rows; ++i) {
        data[i].resize(cols);
        for (int j = 0; j < cols; ++j) {
            if (i < oldData.size() && j < oldData[i].size()) {
                data[i][j] = oldData[i][j];
            } else {
                data[i][j] = QString::number(i * cols + j + 1);
            }
        }
    }
    update();
}

void MatrixBlock::setEditable(bool on) {
    m_editable = on;
    setToolTip(on ? "双击单元格可直接修改数值" : "");
    setCursor(on ? QCursor(Qt::IBeamCursor) : QCursor(Qt::ArrowCursor));
    update();
}

void MatrixBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    painter->setRenderHint(QPainter::Antialiasing);
    const double h = headerHeight();
    const double m = sideMargin();
    const double gridW = cols * cell;
    const double gridH = rows * cellHeight;

    if (!title.isEmpty()) {
        painter->setFont(UiFonts::cjk(11, true));
        painter->setPen(QColor(40, 70, 110));
        painter->drawText(QRectF(m, -h, gridW, h), Qt::AlignLeft | Qt::AlignVCenter, title);
    }

    const double top = h + 4.0;
    const double bottom = h + gridH - 4.0;

    if (shape == Determinant) {
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(barColor, 3.0, Qt::SolidLine, Qt::RoundCap));
        painter->drawLine(QPointF(m - 9.0, top), QPointF(m - 9.0, bottom));
        painter->drawLine(QPointF(m + gridW + 9.0, top), QPointF(m + gridW + 9.0, bottom));
        painter->setPen(QPen(barColor, 2.2));
        painter->drawLine(QPointF(m - 9.0, top - 6.0), QPointF(m - 9.0, top));
        painter->drawLine(QPointF(m + gridW + 9.0, top - 6.0), QPointF(m + gridW + 9.0, top));
    } else if (shape == PlainMatrix) {
        const double over = 9.0;
        const double bTop = h - over;
        const double bBottom = h + gridH + over;
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(barColor, 2.6, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        const double arm = 14.0;
        const double lip = 7.0;
        painter->drawPolyline(QPolygonF({
            QPointF(m - 3.0, bTop), QPointF(m - 3.0 - arm, bTop), QPointF(m - 3.0 - arm, bTop + lip),
            QPointF(m - 3.0 - arm, bBottom - lip), QPointF(m - 3.0 - arm, bBottom), QPointF(m - 3.0, bBottom)}));
        painter->drawPolyline(QPolygonF({
            QPointF(m + gridW + 3.0, bTop), QPointF(m + gridW + 3.0 + arm, bTop),
            QPointF(m + gridW + 3.0 + arm, bTop + lip), QPointF(m + gridW + 3.0 + arm, bBottom - lip),
            QPointF(m + gridW + 3.0 + arm, bBottom), QPointF(m + gridW + 3.0, bBottom)}));
    }

    painter->setFont(UiFonts::mono(13, true));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            const QRectF r(m + j * cell, h + i * cellHeight, cell, cellHeight);
            const bool editing = (m_editor && m_editRow == i && m_editCol == j);
            painter->setBrush(editing ? QColor(255, 255, 255) : cellColor);
            painter->setPen(QPen(gridColor, 1.2));
            painter->drawRect(r);
            if (!editing) {
                painter->setPen(QColor(28, 38, 54));
                painter->drawText(r, Qt::AlignCenter, data[i][j]);
            }
        }
    }
}

void MatrixBlock::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) {
    if (m_editable) {
        int row = -1;
        int col = -1;
        if (cellAtPoint(event->pos(), row, col)) {
            beginCellEdit(row, col);
            event->accept();
            return;
        }
    }
    QGraphicsObject::mouseDoubleClickEvent(event);
}

void MatrixBlock::beginCellEdit(int row, int col) {
    if (!m_editor.isNull()) {
        commitCellEdit();
    }
    if (!scene()) return;
    const QString current = data[row][col];
    m_editor = scene()->addText(current);
    m_editor->setFont(UiFonts::mono(13, true));
    m_editor->setDefaultTextColor(QColor(20, 30, 45));
    m_editor->setTextInteractionFlags(Qt::TextEditorInteraction);
    m_editor->setZValue(20);
    m_editor->setParentItem(this);
    const double m = sideMargin();
    const QFontMetrics fm(m_editor->font());
    const QRectF r(m + col * cell, headerHeight() + row * cellHeight, cell, cellHeight);
    m_editor->setPos(r.left() + (cell - fm.horizontalAdvance(current)) / 2.0,
                     r.top() + (cellHeight - fm.height()) / 2.0);
    m_editRow = row;
    m_editCol = col;
    m_editor->setFocus();
    QPointer<MatrixBlock> self(this);
    QGraphicsTextItem* editor = m_editor.data();
    installMatrixWatcher(m_editor, [self, editor](bool accepted) {
        if (self.isNull()) return;
        if (self->m_editor.data() != editor) return;
        if (accepted) self->commitCellEdit();
        else self->cancelCellEdit();
    });
    QTextCursor cursor = m_editor->textCursor();
    cursor.select(QTextCursor::Document);
    m_editor->setTextCursor(cursor);
    update();
}

void MatrixBlock::commitCellEdit() {
    if (m_editor.isNull()) return;
    const QString text = m_editor->toPlainText().trimmed();
    QGraphicsTextItem* editor = m_editor.data();
    m_editor = nullptr;
    editor->setParentItem(nullptr);
    if (editor->scene()) editor->scene()->removeItem(editor);
    editor->deleteLater();
    const int row = m_editRow;
    const int col = m_editCol;
    m_editRow = -1;
    m_editCol = -1;
    update();
    if (text.isEmpty()) return;
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        data[row][col] = text;
        update();
        emit cellEdited(row, col, text);
    }
}

void MatrixBlock::cancelCellEdit() {
    if (m_editor.isNull()) return;
    QGraphicsTextItem* editor = m_editor.data();
    m_editor = nullptr;
    editor->setParentItem(nullptr);
    if (editor->scene()) editor->scene()->removeItem(editor);
    editor->deleteLater();
    m_editRow = -1;
    m_editCol = -1;
    update();
}
