#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

void MainWindow::init() {
    btn = new QPushButton("关闭", this);
    btn->setFont(QFont("Microsoft Yahei", 40));
    btn->setGeometry(100, 100, 300, 200);
    le = new QLineEdit(this);
    le->setText("这是一条测试文字");
    le->setGeometry(450, 100, 400, 30);

    connect(btn, &QPushButton::clicked, this, &MainWindow::onClickBtn);
}

void MainWindow::onClickBtn() {
    qDebug() << 1111;
    this->close();
}

MainWindow::~MainWindow()
{
    delete ui;
}
