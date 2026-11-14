#ifndef INLINETEXTEDIT_H
#define INLINETEXTEDIT_H
#include <QTextCursor>
#include <QKeyEvent>

namespace InlineTextEdit {

enum Action { NotHandled, Handled, Commit, Cancel };

Action handleKey(QTextCursor& cursor, QKeyEvent* event);

}  // namespace InlineTextEdit

#endif
