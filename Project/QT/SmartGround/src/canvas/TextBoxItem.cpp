#include "TextBoxItem.h"
#include "ui/widgets/AnnotationEditor.h"
#include <QGraphicsTextItem>
#include <QPainter>
#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QDialog>

TextBoxItem::TextBoxItem(QGraphicsItem* parent) : AnnotationItem(parent) {
    m_document.setDocumentMargin(8.0);
    m_textItem = new QGraphicsTextItem();
    m_textItem->setDocument(&m_document);
    m_textItem->setTextInteractionFlags(Qt::NoTextInteraction);
    m_textItem->setAcceptedMouseButtons(Qt::NoButton);
    m_textItem->setFlag(QGraphicsItem::ItemIsSelectable, false);
    m_textItem->setZValue(-1);
    m_textItem->setParentItem(this);
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
    if (m_textItem) {
        m_textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
        m_textItem->setFocus();
        QTextCursor cursor = m_textItem->textCursor();
        cursor.select(QTextCursor::Document);
        m_textItem->setTextCursor(cursor);
    }
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

void TextBoxItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) {
    if (!m_locked) {
        AnnotationEditor editor(AnnotationEditor::TextMode, m_document.toPlainText(), nullptr);
        if (editor.exec() == QDialog::Accepted) {
            setPlainText(editor.content());
        }
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
    return contentBounds().adjusted(-6, -6, 6, 6);
}

void TextBoxItem::applyStyle() {
    setAcceptedMouseButtons(Qt::NoButton);
}

void TextBoxItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    const QRectF r = contentBounds();
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    QColor fill(255, 255, 255, 235);
    painter->setBrush(fill);
    painter->setPen(QPen(QColor(205, 214, 228), 1.2));
    painter->drawRoundedRect(r.adjusted(-5, -5, 5, 5), 6, 6);
    painter->restore();
    drawSelectionFrame(painter);
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
    rebuildDocument();
}
