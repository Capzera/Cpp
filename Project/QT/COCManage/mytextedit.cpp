#include "mytextedit.h"

MTextEdit::MTextEdit(QWidget *parent) : QTextEdit(parent), sh(this) {
    sh.hide();
    sh.setWordWrap(1);
    sh.setFont(QFont("楷体", 20));
    sh.setAlignment(Qt::AlignLeft);
}

void MTextEdit::setPlainText(const QString &text) {
    sh.setText(text);
    sh.show();
    this->setText("");
    sh.setGeometry(0, 0, this->width(), this->height());
}
