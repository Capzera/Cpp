#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::drawHexH(QPainter& paint, int x, int y, int w, int h) {
    int k = 2;
    static const QPointF points[6] = {
        QPointF(x, y + h / 2),
        QPointF(x + h / k, y),
        QPointF(x + w - h / k, y),
        QPointF(x + w, y + h / 2),
        QPointF(x + w - h / k, y + h),
        QPointF(x + h / k, y + h)
    };
    paint.drawPolygon(points, 6);
}

void MainWindow::drawHexV(QPainter &paint, int x, int y, int w, int h) {
    int k = 2;
    static const QPointF points[6] = {
        QPointF(x + w / 2, y),
        QPointF(x + w, y + w / k),
        QPointF(x + w, y + h - w / k),
        QPointF(x + w / 2, y + h),
        QPointF(x, y + h - w / k),
        QPointF(x, y + w / k)
    };
    paint.drawPolygon(points, 6);
}

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
    QPoint center(this->width() / 2, this->height() / 2);
    QPainter pt(this);
    pt.setRenderHint( QPainter::Antialiasing, true);
    pt.setPen(pen);
    pt.translate(center);
    QPainter pt2(this);
    pt2.setRenderHint(QPainter::Antialiasing, true);
    pt2.setPen(pen);
    drawHexV(pt2, 100, 100, 100, 500);
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
