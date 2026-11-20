#include "TextBoxItem.h"
#include "InlineTextEdit.h"
#include <QGraphicsTextItem>
#include <QPainter>
#include <QTextCursor>
#include <QTextBlock>
#include <QTextLayout>
#include <QTextLine>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QApplication>
#include <QClipboard>

TextBoxItem::TextBoxItem(QGraphicsItem* parent) : AnnotationItem(parent) {
    m_document.setDocumentMargin(4.0);
    m_textItem = new QGraphicsTextItem();
    m_textItem->setDocument(&m_document);
    m_textItem->setTextInteractionFlags(Qt::NoTextInteraction);
    m_textItem->setAcceptedMouseButtons(Qt::NoButton);
    m_textItem->setFlag(QGraphicsItem::ItemIsSelectable, false);
    m_textItem->setZValue(-1);
    m_textItem->setParentItem(this);
    m_cursor = QTextCursor(&m_document);
    rebuildDocument();
}

void TextBoxItem::rebuildDocument() {
    QTextCursor cursor(&m_document);
    cursor.select(QTextCursor::Document);
    QTextCharFormat fmt;
    fmt.setFontFamilies({m_family});
    fmt.setFontPointSize(m_fontSize);
    fmt.setFontWeight(m_bold ? QFont::Bold : QFont::Normal);
    fmt.setFontItalic(m_italic);
    fmt.setFontUnderline(m_underline);
    fmt.setForeground(m_textColor);
    cursor.mergeCharFormat(fmt);
    if (m_textItem) {
        m_textItem->setTextWidth(280.0);
        m_textItem->adjustSize();
    }
    prepareGeometryChange();
    update();
}

void TextBoxItem::setPlainText(const QString& text) {
    m_document.setPlainText(text);
    m_cursor = QTextCursor(&m_document);
    m_cursor.movePosition(QTextCursor::End);
    rebuildDocument();
}

QString TextBoxItem::plainText() const {
    return m_document.toPlainText();
}

void TextBoxItem::setFontFamily(const QString& family) {
    if (m_family == family) return;
    m_family = family;
    rebuildDocument();
}

void TextBoxItem::setFontSize(int pointSize) {
    if (m_fontSize == pointSize) return;
    m_fontSize = qBound(7, pointSize, 72);
    rebuildDocument();
}

void TextBoxItem::setBold(bool on) {
    if (m_bold == on) return;
    m_bold = on;
    rebuildDocument();
}

void TextBoxItem::setItalic(bool on) {
    if (m_italic == on) return;
    m_italic = on;
    rebuildDocument();
}

void TextBoxItem::setUnderline(bool on) {
    if (m_underline == on) return;
    m_underline = on;
    rebuildDocument();
}

void TextBoxItem::setTextColor(const QColor& c) {
    if (m_textColor == c) return;
    m_textColor = c;
    rebuildDocument();
}

void TextBoxItem::beginEdit() {
    if (m_editing) return;
    m_editing = true;
    syncCursorToItem();
    update();
}

bool TextBoxItem::beginInlineEdit() {
    beginEdit();
    return m_editing;
}

void TextBoxItem::commitInlineEdit() {
    commitEdit();
}

void TextBoxItem::cancelInlineEdit() {
    cancelEdit();
}

void TextBoxItem::syncCursorToItem() {
    if (!m_textItem) return;
    m_textItem->setTextInteractionFlags(Qt::NoTextInteraction);
    m_textItem->setTextCursor(m_cursor);
}

void TextBoxItem::commitEdit() {
    if (!m_editing) return;
    m_editing = false;
    if (m_textItem) m_textItem->setTextInteractionFlags(Qt::NoTextInteraction);
    update();
}

void TextBoxItem::cancelEdit() {
    if (!m_editing) return;
    m_editing = false;
    if (m_textItem) m_textItem->setTextInteractionFlags(Qt::NoTextInteraction);
    update();
}

bool TextBoxItem::handleEditKey(QKeyEvent* event) {
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
        if (!m_cursor.hasSelection() && m_cursor.position() > 0) {
            QTextCursor probe = m_cursor;
            probe.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
            const QString block = probe.selectedText();
            if (block.size() == 1 && block.at(0) == QChar(0x2029)) {
                m_cursor.deletePreviousChar();
            } else {
                m_cursor.deletePreviousChar();
            }
        } else {
            m_cursor.deletePreviousChar();
        }
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

    if (m_textItem) m_textItem->setTextCursor(m_cursor);
    rebuildDocument();
    notifyContentChanged();
    update();
    return true;
}

void TextBoxItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) {
    if (!m_locked) {
        beginEdit();
        event->accept();
        return;
    }
    QGraphicsObject::mouseDoubleClickEvent(event);
}

QRectF TextBoxItem::contentBounds() const {
    if (!m_textItem) return QRectF();
    return m_textItem->boundingRect();
}

QRectF TextBoxItem::boundingRect() const {
    return contentBounds().adjusted(-4, -4, 4, 4);
}

void TextBoxItem::applyStyle() {
    setAcceptedMouseButtons(Qt::NoButton);
}

void TextBoxItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
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

void TextBoxItem::drawCaret(QPainter* painter) const {
    if (!m_editing || !m_textItem) return;
    const QRectF caret = caretRect();
    if (caret.isEmpty()) return;
    painter->save();
    painter->setPen(QPen(m_textColor, 1.4));
    painter->drawLine(caret.topLeft(), caret.bottomLeft());
    painter->restore();
}

QRectF TextBoxItem::caretRect() const {
    if (!m_textItem) return QRectF();
    const QTextBlock block = m_cursor.block();
    const QTextLayout* layout = block.layout();
    if (!layout) return QRectF();
    const int offset = m_cursor.position() - block.position();
    const QTextLine line = layout->lineForTextPosition(offset);
    if (!line.isValid()) return QRectF();
    const qreal x = line.x() + line.cursorToX(offset) + kCaretGap;
    const QRectF lineRect = line.rect();
    return QRectF(x, lineRect.top(), 1.4, lineRect.height());
}

bool TextBoxItem::prefersCaretPlacement(const QPointF& scenePos) const {
    const QPointF local = mapFromScene(scenePos);
    return contentBounds().contains(local);
}

bool TextBoxItem::placeCaretAt(const QPointF& scenePos) {
    if (!m_editing || !m_textItem) return false;
    const QPointF local = mapFromScene(scenePos);
    const QPointF inner = m_textItem->mapFromParent(local);
    const int position = InlineTextEdit::positionByLineScan(m_textItem, inner);
    if (position < 0) return false;
    m_cursor.setPosition(position);
    m_textItem->setTextCursor(m_cursor);
    rebuildDocument();
    update();
    return true;
}

AnnotationItem* TextBoxItem::cloneItem() const {
    auto* copy = new TextBoxItem();
    copy->setPlainText(m_document.toPlainText());
    copy->setFontFamily(m_family);
    copy->setFontSize(m_fontSize);
    copy->setBold(m_bold);
    copy->setItalic(m_italic);
    copy->setUnderline(m_underline);
    copy->setTextColor(m_textColor);
    copy->setPenColor(m_penColor);
    copy->setPenWidth(m_penWidth);
    copy->setFillOpacity(m_fillOpacity);
    copy->setLocked(m_locked);
    return copy;
}

QJsonObject TextBoxItem::toJson() const {
    QJsonObject obj = AnnotationItem::toJson();
    obj["text"] = m_document.toPlainText();
    obj["family"] = m_family;
    obj["fontSize"] = m_fontSize;
    obj["bold"] = m_bold;
    obj["italic"] = m_italic;
    obj["underline"] = m_underline;
    obj["textColor"] = colorToJson(m_textColor);
    return obj;
}

void TextBoxItem::fromJson(const QJsonObject& obj) {
    AnnotationItem::fromJson(obj);
    m_family = obj.value("family").toString(m_family);
    m_fontSize = obj.value("fontSize").toInt(m_fontSize);
    m_bold = obj.value("bold").toBool(m_bold);
    m_italic = obj.value("italic").toBool(m_italic);
    m_underline = obj.value("underline").toBool(m_underline);
    m_textColor = colorFromJson(obj.value("textColor"), m_textColor);
    m_document.setPlainText(obj.value("text").toString());
    m_cursor = QTextCursor(&m_document);
    m_cursor.movePosition(QTextCursor::End);
    rebuildDocument();
}
