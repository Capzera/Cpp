#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setFixedSize(1024, 728);
    this->setWindowTitle(QString("QT6按钮功能测试"));
    btn1 = new QPushButton("开始", this);
    btn2 = new QPushButton("退出", this);
    btn3 = new QPushButton("返回", this);
    btn1->setGeometry(150, 500, 300, 100);
    btn2->setGeometry(550, 500, 300, 100);
    btn3->setGeometry(350, 500, 300, 100);
    btn3->setVisible(0);
    connect(btn1, SIGNAL(clicked(bool)), this, SLOT(mstart()));
    connect(btn2, SIGNAL(clicked(bool)), this, SLOT(mquit()));
    connect(btn3, SIGNAL(clicked(bool)), this, SLOT(mback()));

}

void MainWindow::mstart() {
    btn1->setVisible(0);
    btn2->setVisible(0);
    btn3->setVisible(1);
}

void MainWindow::mback() {
    btn1->setVisible(1);
    btn2->setVisible(1);
    btn3->setVisible(0);
}

void MainWindow::mquit() {
    this->close();
}

MainWindow::~MainWindow()
{
    delete ui;
}
