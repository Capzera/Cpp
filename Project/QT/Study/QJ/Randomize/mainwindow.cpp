#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QString str("确定");
    btn = new QPushButton(str);
    str = "请输入题库上限";
    edit1 = new QLineEdit(str);
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(edit1);
    vbox->addWidget(btn);
    vbox->addWidget(edit2);
    this->setLayout(vbox);
    connect(btn, SIGNAL(clicked(bool)), this, SLOT(btnclick()));
}

MainWindow::~MainWindow() {
}
