#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    dpi = QGuiApplication::primaryScreen()->logicalDotsPerInch() / 96;
    this->setMouseTracking(true);
    this->setAcceptDrops(true);
    this->setFixedSize(160 / dpi, 48 / dpi);
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    lb1 = new QLabel(this);
    lb1->setFont(QFont("等线", (17 / dpi)));
    lb1->setGeometry(0, 0, 160 / dpi, 48 / dpi);
    QDesktopWidget *wi = QApplication::desktop();
    move(wi->width() - this->width() - (30 / dpi), wi->height() - this->height() - (48 / dpi));
    bool ret = GlobalMouseEvent::installMouseEvent();
    connect(GlobalMouseEvent::getInstance(), SIGNAL(mouseEvent(QMouseEvent*)), this, SLOT(on_mouseEvent(QMouseEvent*)));
}

void MainWindow::on_mouseEvent(QMouseEvent* event) {
    int x = event->x() / dpi, y = event->y() / dpi;
    QString str = " X:" + QString::number(x) + " Y:" + QString::number(y);
    lb1->setText(str);
    delete event;
}

MainWindow::~MainWindow() {
    delete ui;
}
