#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QMenu>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
public slots:
    void Build();
    void Close();
    void Quit();
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    QPushButton *bt;
    QWidget *w1;
    int cnt;
private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
