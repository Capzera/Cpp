#ifndef TEXTBOXITEM_H
#define TEXTBOXITEM_H
#include "AnnotationItem.h"
#include <QTextDocument>
#include <QTextCharFormat>
#include <QTextCursor>
#include "InlineTextEdit.h"

class TextBoxItem : public AnnotationItem {
    Q_OBJECT
public:
    enum { Type = UserType + 23 };
    int type() const override { return Type; }

    explicit TextBoxItem(QGraphicsItem* parent = nullptr);

    void setPlainText(const QString& text);
    QString plainText() const;

    void setFontFamily(const QString& family);
    QString fontFamily() const { return m_family; }
    void setFontSize(int pointSize);
    int fontSize() const { return m_fontSize; }
    void setBold(bool on);
    bool isBold() const { return m_bold; }
    void setItalic(bool on);
    bool isItalic() const { return m_italic; }
    void setUnderline(bool on);
    bool isUnderline() const { return m_underline; }
    void setTextColor(const QColor& c);
    QColor textColor() const { return m_textColor; }

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
    bool placeCaretAt(const QPointF& scenePos) override;
    bool prefersCaretPlacement(const QPointF& scenePos) const override;
    int cursorPositionForTest() const { return m_cursor.position(); }

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
    static constexpr qreal kCaretGap = 4.0;

    void rebuildDocument();
    void syncCursorToItem();
    QRectF caretRect() const;
    void drawCaret(QPainter* painter) const;

    QTextDocument m_document;
    QGraphicsTextItem* m_textItem = nullptr;
    QTextCursor m_cursor;
    QString m_family = "Microsoft YaHei UI";
    int m_fontSize = 12;
    bool m_bold = false;
    bool m_italic = false;
    bool m_underline = false;
    bool m_editing = false;
    QColor m_textColor = QColor(31, 39, 51);
};

#endif
