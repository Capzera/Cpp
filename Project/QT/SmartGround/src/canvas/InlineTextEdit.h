#ifndef INLINETEXTEDIT_H
#define INLINETEXTEDIT_H
#include <QTextCursor>
#include <QTextBlock>
#include <QTextLayout>
#include <QTextLine>
#include <QKeyEvent>
#include <QPointF>

class QGraphicsTextItem;

namespace InlineTextEdit {

enum Action { NotHandled, Handled, Commit, Cancel };

Action handleKey(QTextCursor& cursor, QKeyEvent* event);

QTextBlock blockAtPoint(QGraphicsTextItem* item, const QPointF& localPos);
int cursorOffsetAtPoint(QGraphicsTextItem* item, const QPointF& localPos, const QTextBlock& block);
int positionAtPoint(QGraphicsTextItem* item, const QPointF& localPos);
int positionAtPointRaw(const QTextDocument& doc, const QPointF& localPos);
int positionByLineScan(QGraphicsTextItem* item, const QPointF& localPos);

}  // namespace InlineTextEdit

#endif
