#include "CanvasTableItem.h"
#include <QPainter>
#include <QFontMetrics>
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>
#include <QJsonArray>
#include <QJsonObject>
#include "ui/UiFonts.h"

CanvasTableItem::CanvasTableItem(int rows, int cols, QGraphicsItem* parent) : AnnotationItem(parent) {
    m_rows = qMax(1, rows);
    m_cols = qMax(1, cols);
    setZValue(10);
    rebuildCells();
    applyStyle();
}

void CanvasTableItem::rebuildCells() {
    m_data.resize(m_rows);
    m_aligns.resize(m_rows);
    for (int i = 0; i < m_rows; ++i) {
        const int oldSize = m_data[i].size();
        m_data[i].resize(m_cols);
        m_aligns[i].resize(m_cols);
        for (int j = oldSize; j < m_cols; ++j) {
            m_data[i][j].clear();
            m_aligns[i][j] = TableAlign::Unset;
        }
    }
    prepareGeometryChange();
    update();
}

void CanvasTableItem::setDimensions(int rows, int cols) {
    rows = qMax(1, rows);
    cols = qMax(1, cols);
    if (rows == m_rows && cols == m_cols) return;
    m_rows = rows;
    m_cols = cols;
    rebuildCells();
}

QString CanvasTableItem::cellAt(int r, int c) const {
    if (r < 0 || r >= m_rows || c < 0 || c >= m_cols) return QString();
    if (r >= m_data.size() || c >= m_data[r].size()) return QString();
    return m_data[r][c];
}

void CanvasTableItem::setCell(int r, int c, const QString& text) {
    if (r < 0 || r >= m_rows || c < 0 || c >= m_cols) return;
    if (r >= m_data.size() || c >= m_data[r].size()) return;
    if (m_data[r][c] == text) return;
    m_data[r][c] = text;
    update();
}

void CanvasTableItem::setTitle(const QString& title) {
    if (m_title == title) return;
    m_title = title;
    prepareGeometryChange();
    update();
}

void CanvasTableItem::setCellSize(qreal s) {
    if (qFuzzyCompare(m_cell, s)) return;
    m_cell = s;
    prepareGeometryChange();
    update();
}

qreal CanvasTableItem::headerHeight() const {
    return m_title.isEmpty() ? 0.0 : 24.0;
}

int CanvasTableItem::hitCell(const QPointF& local) const {
    const int col = static_cast<int>(std::floor(local.x() / m_cell));
    const int row = static_cast<int>(std::floor((local.y() - headerHeight()) / m_cell));
    if (row < 0 || row >= m_rows || col < 0 || col >= m_cols) return -1;
    return row * m_cols + col;
}

bool CanvasTableItem::beginEditCell(int r, int c) {
    if (m_locked) return false;
    if (r < 0 || r >= m_rows || c < 0 || c >= m_cols) return false;
    cancelEditCell();
    m_editRow = r;
    m_editCol = c;
    m_editingCell = true;
    if (!m_editor) {
        m_editor = new QGraphicsTextItem();
        m_editor->setFont(UiFonts::mono(12, true));
        m_editor->setDefaultTextColor(QColor(35, 45, 60));
        m_editor->setTextInteractionFlags(Qt::TextEditorInteraction);
        m_editor->setZValue(30);
        m_editor->setFlag(QGraphicsItem::ItemIsSelectable, false);
        m_editor->setParentItem(this);
    }
    m_editor->setPlainText(m_data[r][c]);
    m_editor->setTextInteractionFlags(Qt::TextEditorInteraction);
    m_editor->setPos(c * m_cell + 4.0, headerHeight() + r * m_cell + 4.0);
    m_editor->setFocus();
    QTextCursor cursor = m_editor->textCursor();
    cursor.select(QTextCursor::Document);
    m_editor->setTextCursor(cursor);
    update();
    return true;
}

void CanvasTableItem::commitEditCell() {
    if (!m_editingCell) return;
    if (m_editor && m_editRow >= 0 && m_editRow < m_rows && m_editCol >= 0 && m_editCol < m_cols) {
        m_data[m_editRow][m_editCol] = m_editor->toPlainText();
    }
    m_editingCell = false;
    m_editRow = -1;
    m_editCol = -1;
    if (m_editor) m_editor->setTextInteractionFlags(Qt::NoTextInteraction);
    update();
}

void CanvasTableItem::cancelEditCell() {
    if (!m_editingCell) return;
    m_editingCell = false;
    m_editRow = -1;
    m_editCol = -1;
    if (m_editor) m_editor->setTextInteractionFlags(Qt::NoTextInteraction);
    update();
}

void CanvasTableItem::setContentAlign(TableAlign align) {
    if (m_align == align) return;
    m_align = align;
    update();
}

TableAlign CanvasTableItem::contentAlign() const {
    return m_align;
}

void CanvasTableItem::setCellAlign(int r, int c, TableAlign align) {
    if (r < 0 || r >= m_rows || c < 0 || c >= m_cols) return;
    if (r >= m_aligns.size() || c >= m_aligns[r].size()) return;
    if (m_aligns[r][c] == align) return;
    m_aligns[r][c] = align;
    update();
}

TableAlign CanvasTableItem::cellAlign(int r, int c) const {
    if (r < 0 || r >= m_rows || c < 0 || c >= m_cols) return TableAlign::Unset;
    if (r >= m_aligns.size() || c >= m_aligns[r].size()) return TableAlign::Unset;
    if (m_aligns[r][c] != TableAlign::Unset) return m_aligns[r][c];
    return m_align;
}

void CanvasTableItem::selectCells(const QSet<int>& cells) {
    m_selectedCells = cells;
    m_selectedRow = -1;
    m_selectedCol = -1;
    update();
}

void CanvasTableItem::clearCellSelection() {
    m_selectedCells.clear();
    m_selectedRow = -1;
    m_selectedCol = -1;
    update();
}

void CanvasTableItem::setSelectedRow(int r) {
    m_selectedRow = (r >= 0 && r < m_rows) ? r : -1;
    m_selectedCol = -1;
    update();
}

void CanvasTableItem::setSelectedCol(int c) {
    m_selectedCol = (c >= 0 && c < m_cols) ? c : -1;
    m_selectedRow = -1;
    update();
}

bool CanvasTableItem::insertRows(int at, int count) {
    if (count <= 0) return false;
    at = qBound(0, at, m_rows);
    for (int k = 0; k < count; ++k) {
        QVector<QString> row(m_cols);
        QVector<TableAlign> arow(m_cols, TableAlign::Unset);
        m_data.insert(at + k, row);
        m_aligns.insert(at + k, arow);
    }
    m_rows += count;
    prepareGeometryChange();
    update();
    return true;
}

bool CanvasTableItem::insertColumns(int at, int count) {
    if (count <= 0) return false;
    at = qBound(0, at, m_cols);
    for (int i = 0; i < m_data.size(); ++i) {
        for (int k = 0; k < count; ++k) {
            m_data[i].insert(at + k, QString());
            m_aligns[i].insert(at + k, TableAlign::Unset);
        }
    }
    m_cols += count;
    rebuildCells();
    return true;
}

bool CanvasTableItem::removeRows(int at, int count) {
    if (count <= 0 || at < 0 || at >= m_rows) return false;
    count = qMin(count, m_rows - at);
    for (int k = 0; k < count; ++k) {
        m_data.removeAt(at);
        m_aligns.removeAt(at);
    }
    m_rows -= count;
    rebuildCells();
    return true;
}

bool CanvasTableItem::removeColumns(int at, int count) {
    if (count <= 0 || at < 0 || at >= m_cols) return false;
    count = qMin(count, m_cols - at);
    for (int i = 0; i < m_data.size(); ++i) {
        for (int k = 0; k < count; ++k) {
            m_data[i].removeAt(at);
            m_aligns[i].removeAt(at);
        }
    }
    m_cols -= count;
    rebuildCells();
    return true;
}

void CanvasTableItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) {
    if (!m_locked) {
        const QPointF local = event->pos();
        const int idx = hitCell(local);
        if (idx >= 0) {
            beginEditCell(idx / m_cols, idx % m_cols);
            event->accept();
            return;
        }
    }
    QGraphicsObject::mouseDoubleClickEvent(event);
}

QRectF CanvasTableItem::contentBounds() const {
    const qreal h = headerHeight();
    return QRectF(0, -h, m_cols * m_cell, m_rows * m_cell + h);
}

QRectF CanvasTableItem::boundingRect() const {
    return contentBounds().adjusted(-4, -4, 4, 4);
}

void CanvasTableItem::applyStyle() {
    setAcceptedMouseButtons(Qt::NoButton);
}

void CanvasTableItem::drawCellText(QPainter* painter, const QRectF& r, const QString& text, TableAlign align) const {
    painter->setFont(UiFonts::mono(12, true));
    painter->setPen(QColor(35, 45, 60));
    if (align == TableAlign::Left) {
        painter->drawText(r.adjusted(6, 0, -6, 0), Qt::AlignLeft | Qt::AlignVCenter, text);
        return;
    }
    if (align == TableAlign::Right) {
        painter->drawText(r.adjusted(6, 0, -6, 0), Qt::AlignRight | Qt::AlignVCenter, text);
        return;
    }
    if (align == TableAlign::Top) {
        painter->drawText(r.adjusted(4, 3, -4, 0), Qt::AlignLeft | Qt::AlignTop, text);
        return;
    }
    painter->drawText(r, Qt::AlignCenter, text);
}

void CanvasTableItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    const qreal h = headerHeight();

    if (!m_title.isEmpty()) {
        painter->setFont(UiFonts::cjk(9, true));
        painter->setPen(QColor(40, 70, 110));
        painter->drawText(QRectF(0, -h, m_cols * m_cell, h), Qt::AlignLeft | Qt::AlignVCenter, m_title);
    }

    for (int i = 0; i < m_rows; ++i) {
        if (i >= m_data.size() || i >= m_aligns.size()) break;
        for (int j = 0; j < m_cols; ++j) {
            if (j >= m_data[i].size() || j >= m_aligns[i].size()) break;
            const QRectF r(j * m_cell, h + i * m_cell, m_cell, m_cell);
            const bool editing = (m_editingCell && i == m_editRow && j == m_editCol);
            const bool selected = m_selectedCells.contains(i * m_cols + j);
            const bool rowSel = (m_selectedRow == i);
            const bool colSel = (m_selectedCol == j);

            QColor fill(255, 255, 255);
            if (selected) fill = QColor(226, 238, 253);
            else if (rowSel || colSel) fill = QColor(238, 244, 253);
            if (editing) fill = QColor(238, 245, 255);

            painter->setBrush(fill);
            QColor edge(150, 162, 180);
            qreal edgeWidth = 1.4;
            if (selected) { edge = QColor(59, 125, 221); edgeWidth = 1.9; }
            else if (rowSel || colSel) { edge = QColor(120, 160, 225); edgeWidth = 1.7; }
            if (editing) { edge = QColor(90, 145, 220); edgeWidth = 1.8; }
            painter->setPen(QPen(edge, edgeWidth));
            painter->drawRect(r);
            if (m_data[i][j].isEmpty()) continue;
            drawCellText(painter, r, m_data[i][j], cellAlign(i, j));
        }
    }
    painter->restore();
    drawSelectionFrame(painter);
}

AnnotationItem* CanvasTableItem::cloneItem() const {
    auto* copy = new CanvasTableItem(m_rows, m_cols);
    copy->m_data = m_data;
    copy->m_aligns = m_aligns;
    copy->m_align = m_align;
    copy->setTitle(m_title);
    copy->setCellSize(m_cell);
    copy->setPenColor(m_penColor);
    copy->setPenWidth(m_penWidth);
    copy->setFillOpacity(m_fillOpacity);
    copy->setLocked(m_locked);
    return copy;
}

QJsonObject CanvasTableItem::toJson() const {
    QJsonObject obj = AnnotationItem::toJson();
    obj["rows"] = m_rows;
    obj["cols"] = m_cols;
    obj["cell"] = m_cell;
    obj["title"] = m_title;
    obj["align"] = static_cast<int>(m_align);
    QJsonArray rows;
    for (int i = 0; i < m_rows; ++i) {
        QJsonArray cols;
        for (int j = 0; j < m_cols; ++j) {
            cols.push_back(m_data[i][j]);
        }
        rows.push_back(cols);
    }
    obj["data"] = rows;
    QJsonArray aligns;
    for (int i = 0; i < m_rows; ++i) {
        QJsonArray row;
        for (int j = 0; j < m_cols; ++j) {
            row.push_back(static_cast<int>(m_aligns[i][j]));
        }
        aligns.push_back(row);
    }
    obj["aligns"] = aligns;
    return obj;
}

void CanvasTableItem::fromJson(const QJsonObject& obj) {
    AnnotationItem::fromJson(obj);
    m_rows = qMax(1, obj.value("rows").toInt(m_rows));
    m_cols = qMax(1, obj.value("cols").toInt(m_cols));
    m_cell = obj.value("cell").toDouble(m_cell);
    m_title = obj.value("title").toString(m_title);
    m_align = static_cast<TableAlign>(obj.value("align").toInt(static_cast<int>(m_align)));
    m_data.assign(m_rows, QVector<QString>(m_cols));
    m_aligns.assign(m_rows, QVector<TableAlign>(m_cols, TableAlign::Unset));
    const QJsonArray rows = obj.value("data").toArray();
    for (int i = 0; i < m_rows && i < rows.size(); ++i) {
        const QJsonArray cols = rows.at(i).toArray();
        for (int j = 0; j < m_cols && j < cols.size(); ++j) {
            m_data[i][j] = cols.at(j).toString();
        }
    }
    const QJsonArray aligns = obj.value("aligns").toArray();
    for (int i = 0; i < m_rows && i < aligns.size(); ++i) {
        const QJsonArray row = aligns.at(i).toArray();
        for (int j = 0; j < m_cols && j < row.size(); ++j) {
            m_aligns[i][j] = static_cast<TableAlign>(row.at(j).toInt());
        }
    }
    prepareGeometryChange();
    applyStyle();
    update();
}
