#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(1000, 800);
    this->setWindowTitle("测试菜单");
    buttons.resize(25);
    init();
    Menu();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::init() {
    start = new QPushButton("开始游戏", this);
    start->setFont(QFont("等线", 18));
    start->setGeometry(400, 380, 200, 80);
    option = new QPushButton("选项", this);
    option->setFont(QFont("等线", 18));
    option->setGeometry(400, 480, 200, 80);
    end = new QPushButton("退出", this);
    end->setFont(QFont("等线", 18));
    end->setGeometry(400, 580, 200, 80);
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            int k = i * 5 + j;
            buttons[k] = new QPushButton(this);
            buttons[k]->setText(QString::number(k));
            buttons[k]->setFont(QFont("等线", 18));
            buttons[k]->setGeometry(50 + j * 145, 50 + i * 145, 120, 120);
            connect(buttons[k], SIGNAL(clicked(bool)), this, SLOT(close()));
        }
    }
    btm = new QPushButton("返回", this);
    btm->setFont(QFont("等线", 18));
    btm->setGeometry(850, 100, 100, 300);
    connect(btm, SIGNAL(clicked(bool)), this, SLOT(Menu()));
    connect(start, SIGNAL(clicked(bool)), this, SLOT(SelectLevel()));
    connect(end, SIGNAL(clicked(bool)), this, SLOT(QuitGame()));
}

void MainWindow::Menu() {
    openButton(start);
    openButton(option);
    openButton(end);
    closeButton(btm);
    for (int i = 0; i < 25; i++) closeButton(buttons[i]);
}

void MainWindow::SelectLevel() {
    closeButton(start);
    closeButton(option);
    closeButton(end);
    openButton(btm);
    for (int i = 0; i < 25; i++) openButton(buttons[i]);
}

void MainWindow::closeButton(QPushButton *btn) {
    btn->setVisible(false);
    btn->setEnabled(false);
}

void MainWindow::openButton(QPushButton *btn) {
    btn->setVisible(true);
    btn->setEnabled(true);
}

void MainWindow::QuitGame() {

    this->close();
}
