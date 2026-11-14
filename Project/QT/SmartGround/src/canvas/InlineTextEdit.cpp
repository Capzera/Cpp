#include "InlineTextEdit.h"

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
    case Qt::Key_ShiftLeft:
    case Qt::Key_ShiftRight:
    case Qt::Key_ControlLeft:
    case Qt::Key_ControlRight:
    case Qt::Key_AltLeft:
    case Qt::Key_AltRight:
        return NotHandled;
    default:
        break;
    }

    if (event->text().isEmpty()) return NotHandled;
    if (event->text().at(0).isNull()) return NotHandled;
    if (event->text().at(0).isPrint()) return NotHandled;
    return Handled;
}

}  // namespace InlineTextEdit
