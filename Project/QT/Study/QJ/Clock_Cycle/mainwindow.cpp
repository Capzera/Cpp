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


void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter pt1(this);
    QPen pen(Qt::black, 3);
    QPoint center(this->width() / 2, this->height() / 2), cen(0, 0);

    pt1.setRenderHint(QPainter::Antialiasing, true);
    pt1.setPen(pen);
    pt1.translate(center);
    for (int i = 0; i < 12; i++) {
        pt1.rotate(30);
        pt1.drawLine(0, -180, 0, -200);
    }

    for (int i = 0; i < 60; i++) {
        pt1.rotate(6);
        pt1.drawLine(0, -190, 0, -200);
    }
    pt1.end();

    QPainter pt2(this);
    pt2.setRenderHint(QPainter::Antialiasing, true);
    pen.setWidth(2);
    pen.setColor(Qt::black);
    pt2.setPen(pen);
    pt2.translate(center);
    pt2.rotate(ss * 6);
    pt2.drawLine(0, 0, 0, -180);
    pt2.end();

    QPainter pt3(this);
    pt3.setRenderHint(QPainter::Antialiasing, true);
    pen.setWidth(6);
    pen.setColor(Qt::red);
    pt3.setPen(pen);
    pt3.translate(center);
    pt3.rotate(hh * 30 + mm * 0.5);
    pt3.drawLine(0, 0, 0, -120);
    pt3.end();


    QPainter pt4(this);
    pt4.setRenderHint(QPainter::Antialiasing, true);
    pen.setWidth(4);
    pen.setColor(Qt::blue);
    pt4.setPen(pen);
    pt4.translate(center);
    pt4.rotate(mm * 6 + ss * 0.1);
    pt4.drawLine(0, 0, 0, -155);
    pt4.end();

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

MainWindow::~MainWindow() {
    delete ui;
}
