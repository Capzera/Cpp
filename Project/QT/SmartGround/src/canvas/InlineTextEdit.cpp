#include "InlineTextEdit.h"
#include <QGraphicsTextItem>
#include <QAbstractTextDocumentLayout>

namespace InlineTextEdit {

Action handleKey(QTextCursor& cursor, QKeyEvent* event) {
    const int key = event->key();
    const bool ctrl = event->modifiers().testFlag(Qt::ControlModifier);

    if (ctrl) {
        switch (key) {
        case Qt::Key_A:
            cursor.select(QTextCursor::Document);
            return Handled;
        case Qt::Key_Z:
        case Qt::Key_Y:
            return NotHandled;
        default:
            return NotHandled;
        }
    }

    switch (key) {
    case Qt::Key_Escape:
        return Cancel;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        return Commit;
    case Qt::Key_Backspace:
    case Qt::Key_Delete:
    case Qt::Key_Left:
    case Qt::Key_Right:
    case Qt::Key_Up:
    case Qt::Key_Down:
    case Qt::Key_Home:
    case Qt::Key_End:
    case Qt::Key_PageUp:
    case Qt::Key_PageDown:
    case Qt::Key_Shift:
    case Qt::Key_Control:
    case Qt::Key_Alt:
    case Qt::Key_Meta:
        return NotHandled;
    default:
        break;
    }

    if (event->text().isEmpty()) return NotHandled;
    if (event->text().at(0).isNull()) return NotHandled;
    if (event->text().at(0).isPrint()) return NotHandled;
    return Handled;
}

QTextBlock blockAtPoint(QGraphicsTextItem* item, const QPointF& localPos) {
    if (!item) return QTextBlock();
    QTextDocument* doc = item->document();
    if (!doc) return QTextBlock();
    QTextCursor probe(doc);
    probe.setPosition(qMax(0, doc->characterCount() - 1));
    const QTextBlock block = probe.block();
    if (!block.isValid()) return QTextBlock();
    const QTextLayout* layout = block.layout();
    if (!layout) return block;
    const int lineIndex = layout->lineForTextPosition(block.position()).lineNumber();
    QTextLine line = layout->lineAt(lineIndex);
    if (!line.isValid()) return block;
    for (int i = lineIndex; i < layout->lineCount(); ++i) {
        line = layout->lineAt(i);
        if (!line.isValid()) break;
        if (localPos.y() <= line.rect().bottom()) break;
    }
    return block;
}

int cursorOffsetAtPoint(QGraphicsTextItem* item, const QPointF& localPos, const QTextBlock& block) {
    if (!item || !block.isValid()) return 0;
    const QTextLayout* layout = block.layout();
    if (!layout) return 0;
    const int lineIndex = layout->lineForTextPosition(block.position()).lineNumber();
    QTextLine line = layout->lineAt(lineIndex);
    if (!line.isValid()) return 0;
    for (int i = lineIndex; i < layout->lineCount(); ++i) {
        line = layout->lineAt(i);
        if (!line.isValid()) break;
        if (localPos.y() <= line.rect().bottom()) break;
    }
    return line.xToCursor(localPos.x(), QTextLine::CursorBetweenCharacters);
}

int positionAtPoint(QGraphicsTextItem* item, const QPointF& localPos) {
    if (!item) return -1;
    return positionAtPointRaw(*item->document(), localPos);
}

int positionAtPointRaw(const QTextDocument& doc, const QPointF& localPos) {
    QAbstractTextDocumentLayout* docLayout = doc.documentLayout();
    if (!docLayout) return -1;
    const int hit = docLayout->hitTest(localPos, Qt::FuzzyHit);
    if (hit >= 0) return hit;
    return -1;
}

int positionByLineScan(QGraphicsTextItem* item, const QPointF& localPos) {
    if (!item) return -1;
    QTextDocument* doc = item->document();
    if (!doc) return -1;
    const QTextBlock block = blockAtPoint(item, localPos);
    if (!block.isValid()) return -1;
    const QTextLayout* layout = block.layout();
    if (!layout) return -1;
    const int lineIndex = layout->lineForTextPosition(block.position()).lineNumber();
    QTextLine line = layout->lineAt(lineIndex);
    if (!line.isValid()) return -1;
    for (int i = lineIndex; i < layout->lineCount(); ++i) {
        line = layout->lineAt(i);
        if (!line.isValid()) break;
        if (localPos.y() <= line.rect().bottom()) break;
    }
    return block.position() + line.xToCursor(localPos.x(), QTextLine::CursorBetweenCharacters);
}

}  // namespace InlineTextEdit
