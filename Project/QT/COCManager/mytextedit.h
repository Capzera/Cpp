#ifndef MTEXTEDIT_H
#define MTEXTEDIT_H

#include <QTextEdit>
#include <QLabel>

class MTextEdit : public QTextEdit {
    Q_OBJECT
public:
    explicit MTextEdit(QWidget *parent = 0);
    void setPlainText(const QString &text);
private:
    QLabel sh;
};

#endif // MTEXTEDIT_H
