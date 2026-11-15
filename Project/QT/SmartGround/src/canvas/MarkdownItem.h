#ifndef MARKDOWNITEM_H
#define MARKDOWNITEM_H
#include "AnnotationItem.h"
#include <QTextDocument>
#include <QTextCursor>

class MarkdownItem : public AnnotationItem {
    Q_OBJECT
public:
    enum { Type = UserType + 24 };
    int type() const override { return Type; }

    explicit MarkdownItem(QGraphicsItem* parent = nullptr);

    void setMarkdown(const QString& md);
    QString markdown() const { return m_editing ? m_document.toPlainText() : m_source; }

    void beginEdit();
    bool isEditing() const { return m_editing; }
    void commitEdit();
    void cancelEdit();

    bool supportsInlineEdit() const override { return true; }
    bool beginInlineEdit() override;
    bool isInlineEditing() const override { return m_editing; }
    void commitInlineEdit() override;
    void cancelInlineEdit() override;
    bool handleEditKey(QKeyEvent* event) override;

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;
    void applyStyle() override;
    QRectF contentBounds() const override;
    AnnotationItem* cloneItem() const override;
    QJsonObject toJson() const override;
    void fromJson(const QJsonObject& obj) override;

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

private:
    void rebuildDocument();
    void syncCursorToItem();
    void drawCaret(QPainter* painter) const;
    QString buildStyleSheet() const;

    QTextDocument m_document;
    QGraphicsTextItem* m_textItem = nullptr;
    QTextCursor m_cursor;
    QString m_source;
    bool m_editing = false;
};

#endif
