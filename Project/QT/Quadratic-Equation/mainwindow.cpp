#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    dpi  = QGuiApplication::primaryScreen()->logicalDotsPerInch() / 96;
    qDebug() << dpi;
    ui->setupUi(this);
    Init();
}


void MainWindow::Init() {
    this->setFixedSize(400 / dpi, 300 / dpi);
    this->setWindowTitle("二元一次方程求解器");
    CreateItem();
    SetFont();
    SetGeometry();
    Connection();
}

void MainWindow::CreateItem() {
    cal = new QPushButton("计算", this);
    qui = new QPushButton("退出", this);
    eq1 = new QLabel("方程1：", this);
    eq2 = new QLabel("方程2：", this);
    xx1 = new QLabel("x + ", this);
    xx2 = new QLabel("x + ", this);
    yy1 = new QLabel("y = ", this);
    yy2 = new QLabel("y = ", this);
    x1 = new QLineEdit(this);
    x2 = new QLineEdit(this);
    y1 = new QLineEdit(this);
    y2 = new QLineEdit(this);
    c1 = new QLineEdit(this);
    c2 = new QLineEdit(this);
    ans = new QLineEdit(this);
}

void MainWindow::SetFont() {
    QFont ft("等线", 16 / dpi);
    QFont ft2("等线", 28 / dpi);
    cal->setFont(ft);
    qui->setFont(ft);
    eq1->setFont(ft);
    eq2->setFont(ft);
    xx1->setFont(ft);
    xx2->setFont(ft);
    yy1->setFont(ft);
    yy2->setFont(ft);
    x1->setFont(ft);
    x2->setFont(ft);
    y1->setFont(ft);
    y2->setFont(ft);
    c1->setFont(ft);
    c2->setFont(ft);
    ans->setFont(ft);
}

void MainWindow::SetGeometry() {
    cal->setGeometry(50, 250, 100, 40);
    qui->setGeometry(250, 250, 100, 40);
    eq1->setGeometry(25, 65, 80, 25);
    eq2->setGeometry(25, 120, 80, 25);
    xx1->setGeometry(165, 65, 60, 25);
    xx2->setGeometry(165, 120, 60, 25);
    yy1->setGeometry(265, 65, 60, 25);
    yy2->setGeometry(265, 120, 60, 25);
    x1->setGeometry(105, 68, 55, 25);
    x2->setGeometry(105, 123, 55, 25);
    y1->setGeometry(205, 68, 55, 25);
    y2->setGeometry(205, 123, 55, 25);
    c1->setGeometry(300, 68, 90, 25);
    c2->setGeometry(300, 123, 90, 25);
    ans->setGeometry(25, 160, 350, 80);
    ans->setAlignment(Qt::AlignCenter);
}

void MainWindow::Connection() {
    connect(cal, SIGNAL(clicked(bool)), this, SLOT(Calculator()));
    connect(qui, SIGNAL(clicked(bool)), this, SLOT(Quit()));
}

void MainWindow::Calculator() {
    int a1 = x1->text().toInt();
    int a2 = x2->text().toInt();
    int b1 = y1->text().toInt();
    int b2 = y2->text().toInt();
    double v1 = c1->text().toDouble();
    double v2 = c2->text().toDouble();
    int lcma = a1 * a2 / __gcd(a1, a2);
    b1 *= lcma / a1;
    v1 *= lcma / a1;
    b2 *= lcma / a2;
    v2 *= lcma / a2;
    double y = 1.0 * (v1 - v2) / (b1 - b2);
    double x = (v1 - b1 * y) / lcma;
    QString ansText = "X = " + QString::number(x) + ", Y = " + QString::number(y);
    ans->setText(ansText);
    qDebug() << lcma;
}

void MainWindow::Quit() {
    this->close();
}

MainWindow::~MainWindow() {
    delete ui;
}
