#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setWindowTitle("Clock");
    this->setFixedSize(1024, 768);
    timer = startTimer(1000);
    QTime time = QTime::currentTime();
    hh = time.hour();
    mm = time.minute();
    ss = time.second();
}


MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPen pen(Qt::black, 3);
    int x = 40 , y = 100;
    QPoint center(this->width() / 2, this->height() / 2);
    QPainter pt(this);

    pt.setPen(pen);
    pt.translate(x, y);
    QGraphicsScene *qg = new QGraphicsScene();
    MyItem *it = new MyItem();
    it->setPos(0, 0);
    qg.addItem(it);
    ui->graphicsView->setSceneRect(-100,-100,100,100);
    ui->graphicsView->setScene(qg);
    update();
}

void MainWindow::timerEvent(QTimerEvent *event) {
    ss++;
    if (ss == 60) {
        ss = 0, mm++;
    }
    if (mm == 60) {
        mm = 0, hh++;
    }
    if (hh == 24) hh = 0;
}
