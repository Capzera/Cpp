#include "MarkdownItem.h"
#include "ui/widgets/AnnotationEditor.h"
#include <QGraphicsTextItem>
#include <QPainter>
#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>
#include <QTextBlock>
#include <QDialog>

MarkdownItem::MarkdownItem(QGraphicsItem* parent) : AnnotationItem(parent) {
    m_document.setDocumentMargin(14.0);
    m_textItem = new QGraphicsTextItem();
    m_textItem->setDocument(&m_document);
    m_textItem->setTextInteractionFlags(Qt::NoTextInteraction);
    m_textItem->setAcceptedMouseButtons(Qt::NoButton);
    m_textItem->setFlag(QGraphicsItem::ItemIsSelectable, false);
    m_textItem->setZValue(-1);
    m_textItem->setParentItem(this);
    m_textItem->setTextWidth(320.0);
    setMarkdown("# 标题\n\n正文段落。\n\n- 列表项 A\n- 列表项 B");
}

void MarkdownItem::rebuildDocument() {
    m_document.setMarkdown(m_source, QTextDocument::MarkdownDialectGitHub);
    m_document.setDefaultStyleSheet(buildStyleSheet());
    if (m_textItem) {
        m_textItem->setTextWidth(320.0);
        m_textItem->adjustSize();
    }
    prepareGeometryChange();
    update();
}

QString MarkdownItem::buildStyleSheet() const {
    return QString(
        "h1 { font-size: 19px; font-weight: 700; color: #1f2733; margin-top: 4px; margin-bottom: 6px; }"
        "h2 { font-size: 16px; font-weight: 700; color: #1f2733; margin-top: 8px; margin-bottom: 4px; }"
        "h3 { font-size: 14px; font-weight: 700; color: #2b3547; margin-top: 6px; margin-bottom: 3px; }"
        "p  { font-size: 13px; color: #33405a; }"
        "li { font-size: 13px; color: #33405a; }"
        "code { font-family: 'Cascadia Mono','Consolas'; font-size: 12px; background-color: #eef2f9; color: #b8465f; }"
        "pre { font-family: 'Cascadia Mono','Consolas'; font-size: 12px; background-color: #f4f7fc; color: #26344d; margin-top: 4px; margin-bottom: 4px; }"
        "blockquote { font-size: 12px; color: #5a6b85; margin-top: 4px; margin-bottom: 4px; }"
        "table { font-size: 12px; color: #33405a; }"
        "b, strong { font-weight: 700; color: #1f2733; }"
        "em, i { font-style: italic; color: #45536b; }"
        "hr { color: #d8e0ec; height: 1px; }"
    );
}

void MarkdownItem::setMarkdown(const QString& md) {
    m_source = md;
    rebuildDocument();
}

void MarkdownItem::beginEdit() {
    if (m_editing) return;
    m_editing = true;
    if (m_textItem) {
        m_textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
        m_textItem->setFocus();
    }
    update();
}

bool MarkdownItem::beginInlineEdit() {
    if (m_editing) return true;
    m_editing = true;
    m_document.setPlainText(m_source);
    if (m_textItem) {
        m_textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
        m_textItem->setFocus();
        QTextCursor cursor = m_textItem->textCursor();
        cursor.select(QTextCursor::Document);
        m_textItem->setTextCursor(cursor);
    }
    update();
    return true;
}

void MarkdownItem::commitInlineEdit() {
    if (!m_editing) return;
    m_editing = false;
    m_source = m_document.toPlainText();
    if (m_textItem) m_textItem->setTextInteractionFlags(Qt::NoTextInteraction);
    rebuildDocument();
    update();
}

void MarkdownItem::cancelInlineEdit() {
    if (!m_editing) return;
    m_editing = false;
    if (m_textItem) m_textItem->setTextInteractionFlags(Qt::NoTextInteraction);
    rebuildDocument();
    update();
}

void MarkdownItem::commitEdit() {
    if (!m_editing) return;
    m_editing = false;
    if (m_textItem) m_textItem->setTextInteractionFlags(Qt::NoTextInteraction);
    update();
}

void MarkdownItem::cancelEdit() {
    if (!m_editing) return;
    m_editing = false;
    if (m_textItem) m_textItem->setTextInteractionFlags(Qt::NoTextInteraction);
    update();
}

void MarkdownItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) {
    if (!m_locked) {
        AnnotationEditor editor(AnnotationEditor::MarkdownMode, m_source, nullptr);
        if (editor.exec() == QDialog::Accepted) {
            setMarkdown(editor.content());
        }
        event->accept();
        return;
    }
    QGraphicsObject::mouseDoubleClickEvent(event);
}

QRectF MarkdownItem::contentBounds() const {
    if (!m_textItem) return QRectF();
    return m_textItem->boundingRect();
}

QRectF MarkdownItem::boundingRect() const {
    return contentBounds().adjusted(-8, -8, 8, 8);
}

void MarkdownItem::applyStyle() {
    setAcceptedMouseButtons(Qt::NoButton);
}

void MarkdownItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    const QRectF r = contentBounds();
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(QColor(255, 255, 255, 244));
    painter->setPen(QPen(QColor(211, 219, 232), 1.3));
    painter->drawRoundedRect(r.adjusted(-7, -7, 7, 7), 9, 9);
    painter->restore();
    drawSelectionFrame(painter);
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
