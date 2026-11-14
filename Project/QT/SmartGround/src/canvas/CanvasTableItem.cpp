#include "CanvasTableItem.h"
#include <QPainter>
#include <QFontMetrics>
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>
#include <QJsonArray>
#include <QJsonObject>

CanvasTableItem::CanvasTableItem(int rows, int cols, QGraphicsItem* parent) : AnnotationItem(parent) {
    m_rows = qMax(1, rows);
    m_cols = qMax(1, cols);
    setZValue(10);
    rebuildCells();
    applyStyle();
}

void CanvasTableItem::rebuildCells() {
    m_data.resize(m_rows);
    for (int i = 0; i < m_rows; ++i) {
        const int oldSize = m_data[i].size();
        m_data[i].resize(m_cols);
        for (int j = oldSize; j < m_cols; ++j) {
            m_data[i][j].clear();
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
    return m_data[r][c];
}

void CanvasTableItem::setCell(int r, int c, const QString& text) {
    if (r < 0 || r >= m_rows || c < 0 || c >= m_cols) return;
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
        m_editor->setFont(QFont("Consolas", 12, QFont::Bold));
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

void CanvasTableItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    const qreal h = headerHeight();

    if (!m_title.isEmpty()) {
        painter->setFont(QFont("Microsoft YaHei UI", 9, QFont::Bold));
        painter->setPen(QColor(40, 70, 110));
        painter->drawText(QRectF(0, -h, m_cols * m_cell, h), Qt::AlignLeft | Qt::AlignVCenter, m_title);
    }

    for (int i = 0; i < m_rows; ++i) {
        for (int j = 0; j < m_cols; ++j) {
            const QRectF r(j * m_cell, h + i * m_cell, m_cell, m_cell);
            const bool editing = (m_editingCell && i == m_editRow && j == m_editCol);
            painter->setBrush(editing ? QColor(238, 245, 255) : QColor(255, 255, 255));
            painter->setPen(QPen(editing ? QColor(90, 145, 220) : QColor(150, 162, 180), editing ? 1.8 : 1.4));
            painter->drawRect(r);
            if (m_data[i][j].isEmpty()) continue;
            painter->setFont(QFont("Consolas", 12, QFont::Bold));
            painter->setPen(QColor(35, 45, 60));
            painter->drawText(r, Qt::AlignCenter, m_data[i][j]);
        }
    }
    painter->restore();
    drawSelectionFrame(painter);
}

AnnotationItem* CanvasTableItem::cloneItem() const {
    auto* copy = new CanvasTableItem(m_rows, m_cols);
    copy->m_data = m_data;
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
    QJsonArray rows;
    for (int i = 0; i < m_rows; ++i) {
        QJsonArray cols;
        for (int j = 0; j < m_cols; ++j) {
            cols.push_back(m_data[i][j]);
        }
        rows.push_back(cols);
    }
    obj["data"] = rows;
    return obj;
}

void CanvasTableItem::fromJson(const QJsonObject& obj) {
    AnnotationItem::fromJson(obj);
    m_rows = qMax(1, obj.value("rows").toInt(m_rows));
    m_cols = qMax(1, obj.value("cols").toInt(m_cols));
    m_cell = obj.value("cell").toDouble(m_cell);
    m_title = obj.value("title").toString(m_title);
    m_data.assign(m_rows, QVector<QString>(m_cols));
    const QJsonArray rows = obj.value("data").toArray();
    for (int i = 0; i < m_rows && i < rows.size(); ++i) {
        const QJsonArray cols = rows.at(i).toArray();
        for (int j = 0; j < m_cols && j < cols.size(); ++j) {
            m_data[i][j] = cols.at(j).toString();
        }
    }
    prepareGeometryChange();
    applyStyle();
    update();
}
