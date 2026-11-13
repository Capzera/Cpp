#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setFixedSize(1024,768);
    this->setWindowTitle("2024-01-21");
    QPushButton *btn = new QPushButton("退出", this);
    btn->setGeometry(100, 100, 300, 300);
    connect(btn, SIGNAL(clicked(bool)), this, SLOT(guanbi()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::guanbi() {
     this->close();
}
