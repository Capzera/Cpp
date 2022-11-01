#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setFixedSize(960, 960);
    cnt = 0;
    bt = new QPushButton(QString::number(cnt), this);
    bt->setText("文件");
    bt->setGeometry(440, 440, 80, 80);
    QMenu *menu = new QMenu();
    QAction *ac1 = new QAction("新建");
    QAction *ac2 = new QAction("关闭");
    QAction *ac3 = new QAction("退出");
    menu->addAction(ac1);
    menu->addAction(ac2);
    menu->addAction(ac3);
    bt->setMenu(menu);
    connect(ac1, SIGNAL(triggered(bool)), this, SLOT(Build()));
    connect(ac2, SIGNAL(triggered(bool)), this, SLOT(Close()));
    connect(ac3, SIGNAL(triggered(bool)), this, SLOT(Quit()));
}

void Widget::Build() {
    w1 = new QWidget();
    w1->show();
}

void Widget::Close() {
    w1->close();
}

void Widget::Quit() {
    this->close();
}

Widget::~Widget()
{
    delete ui;
}
