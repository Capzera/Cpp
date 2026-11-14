#ifndef MARKDOWNITEM_H
#define MARKDOWNITEM_H
#include "AnnotationItem.h"
#include <QTextDocument>

class MarkdownItem : public AnnotationItem {
    Q_OBJECT
public:
    enum { Type = UserType + 24 };
    int type() const override { return Type; }

    explicit MarkdownItem(QGraphicsItem* parent = nullptr);

    void setMarkdown(const QString& md);
    QString markdown() const { return m_source; }

    void beginEdit();
    bool isEditing() const { return m_editing; }
    void commitEdit();
    void cancelEdit();

    bool supportsInlineEdit() const override { return true; }
    bool beginInlineEdit() override;
    bool isInlineEditing() const override { return m_editing; }
    void commitInlineEdit() override;
    void cancelInlineEdit() override;

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
    QString buildStyleSheet() const;

    QTextDocument m_document;
    QGraphicsTextItem* m_textItem = nullptr;
    QString m_source;
    bool m_editing = false;
};

#endif
