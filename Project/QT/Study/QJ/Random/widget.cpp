#include "widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent) {
    QString str("OK");
    btn = new QPushButton("出题");
    edit1 = new QLineEdit;
    edit2 = new QLineEdit;
    QLabel *lb1 = new QLabel("最大题库数:");
    QLabel *lb2 = new QLabel("结果:");
    QGridLayout *qgl = new QGridLayout;
    qgl->addWidget(lb1, 0, 0, 1, 1);
    qgl->addWidget(edit1, 0, 1, 1, 3);
    qgl->addWidget(lb2, 1, 0, 1, 1);
    qgl->addWidget(edit2, 1, 1, 1, 3);
    qgl->addWidget(btn, 4, 1);
    this->setLayout(qgl);
    connect(btn, SIGNAL(clicked(bool)), this, SLOT(btnclick()));
}

Widget::~Widget()
{

}
