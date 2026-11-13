#ifndef WIDGET_H
#define WIDGET_H

#include <Qwidget>
#include <QPushButton>
#include <QLineEdit>
#include <QString>
#include <QVBoxLayout>
#include <QHboxLayout>
#include <QGridLayout>
#include <QTime>
#include <QLabel>

class Widget : public QWidget
{
    Q_OBJECT
public slots:
void btnclick() {
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    QString str = edit1->text();
    int maxsize = str.toInt(), a = maxsize;
    while (a == maxsize) {
        a = qrand() % maxsize;
    }
    str = QString::number(a);
    edit2->setText(str);
}
public:
    Widget(QWidget *parent = 0);
    ~Widget();
private:
    QPushButton *btn;
    QLineEdit *edit1, *edit2;
};

#endif // WIDGET_H
