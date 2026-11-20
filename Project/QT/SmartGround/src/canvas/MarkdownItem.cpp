#include "MarkdownItem.h"
#include "InlineTextEdit.h"
#include "ui/widgets/AnnotationEditor.h"
#include "ui/UiFonts.h"
#include <QGraphicsTextItem>
#include <QPainter>
#include <QTextCursor>
#include <QTextLayout>
#include <QTextBlock>
#include <QTextLine>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

MarkdownItem::MarkdownItem(QGraphicsItem* parent) : AnnotationItem(parent) {
    m_preview.setDocumentMargin(6.0);
    m_editor.setDocumentMargin(6.0);
    m_textItem = new QGraphicsTextItem();
    m_textItem->setDocument(&m_preview);
    m_textItem->setTextInteractionFlags(Qt::NoTextInteraction);
    m_textItem->setAcceptedMouseButtons(Qt::NoButton);
    m_textItem->setFlag(QGraphicsItem::ItemIsSelectable, false);
    m_textItem->setZValue(-1);
    m_textItem->setParentItem(this);
    m_textItem->setTextWidth(320.0);
    m_sourceItem = new QGraphicsTextItem();
    m_sourceItem->setDocument(&m_editor);
    m_sourceItem->setTextInteractionFlags(Qt::NoTextInteraction);
    m_sourceItem->setAcceptedMouseButtons(Qt::NoButton);
    m_sourceItem->setFlag(QGraphicsItem::ItemIsSelectable, false);
    m_sourceItem->setZValue(-1);
    m_sourceItem->setParentItem(this);
    m_sourceItem->setTextWidth(320.0);
    m_sourceItem->hide();
    m_cursor = QTextCursor(&m_editor);
    setMarkdown("# 标题\n\n正文段落。\n\n- 列表项 A\n- 列表项 B");
}

void MarkdownItem::rebuildPreview() {
    m_preview.setMarkdown(m_source, QTextDocument::MarkdownDialectGitHub);
    m_preview.setDefaultStyleSheet(buildStyleSheet());
    if (m_textItem) {
        m_textItem->setTextWidth(320.0);
        m_textItem->adjustSize();
    }
    relayoutPreview();
    update();
}

QString MarkdownItem::buildStyleSheet() const {
    return QString(
        "h1 { font-size: 19px; font-weight: 700; color: #1f2733; margin-top: 4px; margin-bottom: 6px; }"
        "h2 { font-size: 16px; font-weight: 700; color: #1f2733; margin-top: 8px; margin-bottom: 4px; }"
        "h3 { font-size: 14px; font-weight: 700; color: #2b3547; margin-top: 6px; margin-bottom: 3px; }"
        "p  { font-size: 13px; color: #33405a; }"
        "li { font-size: 13px; color: #33405a; }"
        "code { font-family: 'Consolas'; font-size: 12px; background-color: #eef2f9; color: #b8465f; }"
        "pre { font-family: 'Consolas'; font-size: 12px; background-color: #f4f7fc; color: #26344d; margin-top: 4px; margin-bottom: 4px; }"
        "blockquote { font-size: 12px; color: #5a6b85; margin-top: 4px; margin-bottom: 4px; }"
        "table { font-size: 12px; color: #33405a; }"
        "b, strong { font-weight: 700; color: #1f2733; }"
        "em, i { font-style: italic; color: #45536b; }"
        "hr { color: #d8e0ec; height: 1px; }"
    );
}

void MarkdownItem::setMarkdown(const QString& md) {
    m_source = md;
    m_editor.setPlainText(md);
    m_cursor = QTextCursor(&m_editor);
    m_cursor.movePosition(QTextCursor::End);
    rebuildPreview();
}

QString MarkdownItem::editorSource() const {
    return m_editor.toPlainText();
}

void MarkdownItem::beginEdit() {
    if (m_editing) return;
    m_editing = true;
    m_editor.setPlainText(m_source);
    m_cursor = QTextCursor(&m_editor);
    m_cursor.movePosition(QTextCursor::End);
    m_sourceItem->setPlainText(m_source);
    m_sourceItem->setTextWidth(320.0);
    m_sourceItem->adjustSize();
    m_sourceItem->show();
    syncCursorToItem();
    relayoutPreview();
    update();
}

void MarkdownItem::relayoutPreview() {
    if (!m_sourceItem || !m_textItem) return;
    const qreal sourceH = m_sourceItem->boundingRect().height();
    if (m_editing) {
        m_textItem->setPos(0, sourceH + 10.0);
        m_textItem->show();
    } else {
        m_textItem->setPos(0, 0);
    }
    prepareGeometryChange();
}

bool MarkdownItem::beginInlineEdit() {
    beginEdit();
    return true;
}

void MarkdownItem::syncCursorToItem() {
    if (!m_sourceItem) return;
    m_sourceItem->setTextInteractionFlags(Qt::NoTextInteraction);
    m_sourceItem->setTextCursor(m_cursor);
}

void MarkdownItem::commitInlineEdit() {
    commitEdit();
}

void MarkdownItem::cancelInlineEdit() {
    cancelEdit();
}

void MarkdownItem::commitEdit() {
    if (!m_editing) return;
    m_editing = false;
    m_source = m_editor.toPlainText();
    if (m_sourceItem) {
        m_sourceItem->hide();
        m_sourceItem->setTextInteractionFlags(Qt::NoTextInteraction);
    }
    rebuildPreview();
    relayoutPreview();
    notifyContentChanged();
    update();
}

void MarkdownItem::cancelEdit() {
    if (!m_editing) return;
    m_editing = false;
    m_source = m_editor.toPlainText();
    if (m_sourceItem) {
        m_sourceItem->hide();
        m_sourceItem->setTextInteractionFlags(Qt::NoTextInteraction);
    }
    rebuildPreview();
    relayoutPreview();
    notifyContentChanged();
    update();
}

bool MarkdownItem::handleEditKey(QKeyEvent* event) {
    if (!m_editing) return false;
    const InlineTextEdit::Action action = InlineTextEdit::handleKey(m_cursor, event);
    if (action == InlineTextEdit::Commit) {
        commitEdit();
        return true;
    }
    if (action == InlineTextEdit::Cancel) {
        commitEdit();
        return true;
    }
    if (action == InlineTextEdit::Handled) return true;

    const int key = event->key();
    const bool shift = event->modifiers().testFlag(Qt::ShiftModifier);

    if (key == Qt::Key_Backspace) {
        m_cursor.deletePreviousChar();
    } else if (key == Qt::Key_Delete) {
        m_cursor.deleteChar();
    } else if (key == Qt::Key_Return || key == Qt::Key_Enter) {
        m_cursor.insertText(QString("\n"));
    } else if (key == Qt::Key_Left) {
        if (shift) m_cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
        else m_cursor.movePosition(QTextCursor::Left);
    } else if (key == Qt::Key_Right) {
        if (shift) m_cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
        else m_cursor.movePosition(QTextCursor::Right);
    } else if (key == Qt::Key_Up) {
        if (shift) m_cursor.movePosition(QTextCursor::Up, QTextCursor::KeepAnchor);
        else m_cursor.movePosition(QTextCursor::Up);
    } else if (key == Qt::Key_Down) {
        if (shift) m_cursor.movePosition(QTextCursor::Down, QTextCursor::KeepAnchor);
        else m_cursor.movePosition(QTextCursor::Down);
    } else if (key == Qt::Key_Home) {
        m_cursor.movePosition(QTextCursor::StartOfLine, shift ? QTextCursor::KeepAnchor : QTextCursor::MoveAnchor);
    } else if (key == Qt::Key_End) {
        m_cursor.movePosition(QTextCursor::EndOfLine, shift ? QTextCursor::KeepAnchor : QTextCursor::MoveAnchor);
    } else if (!event->text().isEmpty() && event->text().at(0).isPrint()) {
        m_cursor.insertText(event->text());
    } else {
        return false;
    }

    m_source = m_editor.toPlainText();
    if (m_sourceItem) {
        m_sourceItem->setTextCursor(m_cursor);
        m_sourceItem->setTextWidth(320.0);
        m_sourceItem->adjustSize();
    }
    rebuildPreview();
    relayoutPreview();
    notifyContentChanged();
    update();
    return true;
}

void MarkdownItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) {
    if (!m_locked) {
        beginEdit();
        event->accept();
        return;
    }
    QGraphicsObject::mouseDoubleClickEvent(event);
}

QRectF MarkdownItem::contentBounds() const {
    if (!m_textItem) return QRectF();
    if (m_editing && m_sourceItem && m_sourceItem->isVisible()) {
        const QRectF src = m_sourceItem->boundingRect();
        const QRectF prev = m_textItem->boundingRect().translated(m_textItem->pos());
        return src.united(prev);    }
    return m_textItem->boundingRect();
}

QRectF MarkdownItem::boundingRect() const {
    return contentBounds().adjusted(-4, -4, 4, 4);
}

void MarkdownItem::applyStyle() {
    setAcceptedMouseButtons(Qt::NoButton);
}

void MarkdownItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    if (!m_editing) {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        QPen pen(QColor(150, 162, 180), 1.0, Qt::DashLine);
        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(contentBounds().adjusted(-2, -2, 2, 2));
        painter->restore();
    }
    drawCaret(painter);
    drawSelectionFrame(painter);
}

void MarkdownItem::drawCaret(QPainter* painter) const {
    if (!m_editing) return;
    const QTextBlock block = m_cursor.block();
    const QTextLayout* layout = block.layout();
    if (!layout) return;
    const int offset = m_cursor.position() - block.position();
    const QTextLine line = layout->lineForTextPosition(offset);
    if (!line.isValid()) return;
    const qreal x = line.x() + line.cursorToX(offset) + kCaretGap;
    const QRectF lineRect = line.rect();
    painter->save();
    painter->setPen(QPen(QColor(31, 39, 51), 1.4));
    painter->drawLine(QPointF(x, lineRect.top()), QPointF(x, lineRect.bottom()));
    painter->restore();
}

bool MarkdownItem::prefersCaretPlacement(const QPointF& scenePos) const {
    const QPointF local = mapFromScene(scenePos);
    if (!m_sourceItem || !m_sourceItem->isVisible()) return false;
    const QRectF src = m_sourceItem->boundingRect();
    return src.contains(local);
}

bool MarkdownItem::placeCaretAt(const QPointF& scenePos) {
    if (!m_editing || !m_sourceItem) return false;
    const QPointF local = mapFromScene(scenePos);
    const QPointF inner = m_sourceItem->mapFromParent(local);
    const int position = InlineTextEdit::positionByLineScan(m_sourceItem, inner);
    if (position < 0) return false;
    m_cursor.setPosition(position);
    if (m_sourceItem) {
        m_sourceItem->setTextCursor(m_cursor);
        m_sourceItem->setTextWidth(320.0);
        m_sourceItem->adjustSize();
    }
    m_source = m_editor.toPlainText();
    rebuildPreview();
    relayoutPreview();
    notifyContentChanged();
    update();
    return true;
}

AnnotationItem* MarkdownItem::cloneItem() const {
    auto* copy = new MarkdownItem();
    copy->setMarkdown(m_source);
    copy->setPenColor(m_penColor);
    copy->setPenWidth(m_penWidth);
    copy->setFillOpacity(m_fillOpacity);
    copy->setLocked(m_locked);
    return copy;
}

QJsonObject MarkdownItem::toJson() const {
    QJsonObject obj = AnnotationItem::toJson();
    obj["markdown"] = m_source;
    return obj;
}

void MarkdownItem::fromJson(const QJsonObject& obj) {
    AnnotationItem::fromJson(obj);
    setMarkdown(obj.value("markdown").toString());
}
