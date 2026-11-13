#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    dpi  = QGuiApplication::primaryScreen()->logicalDotsPerInch() / 96;
    qDebug() << dpi;
    ui->setupUi(this);
    this->setFixedSize(1024 / dpi, 768 / dpi);
    this->setWindowTitle(QString("FudianPractice    By:Capzera       ") + VersionNumber);
    Init();
}

void MainWindow::startTriggle() {
    lbOn(lbstick);
    lbOn(lbban);
    lbOn(lbfu);
    teOn(tefu);
    teOn(teban);
    btnOn(btnnxt);
    btnOn(btnans);
    btnOff(btnhjm);
    nxtTriggle();
}

void MainWindow::Init() {
    QFont ft("微软雅黑", 28 / dpi);
    QFont ft2("微软雅黑", 36 / dpi);
    int c1 = 200 / dpi;
    int c2 = 450 / dpi;
    int c3 = 610 / dpi;
    int w1 = 100 / dpi;
    int w2 = 100 / dpi;
    btnhjm = new MyButton("开始", this);
    btnhjm->setFont(ft2);
    btnhjm->setGeometry((this->width() - 400 / dpi) / 2, (this->height() - 300 / dpi) / 2, 400 / dpi, 300 / dpi);
    lbply[0] = new QLabel("亲家", this);
    lbply[0]->setFont(ft2);
    lbply[0]->setGeometry(130 / dpi, c1, w1, w2);
    lbOff(lbply[0]);
    lbply[1] = new QLabel("子家", this);
    lbply[1]->setFont(ft2);
    lbply[1]->setGeometry(130 / dpi, c1, w1, w2);
    lbOff(lbply[1]);
    lbpay[0] = new QLabel("子家    均付", this);
    lbpay[0]->setFont(ft2);
    lbpay[0]->setGeometry(130 / dpi, c2, 350 / dpi, w2);
    lbOff(lbpay[0]);
    lbpay[1] = new QLabel("子亲    支付", this);
    lbpay[1]->setFont(ft2);
    lbpay[1]->setGeometry(130 / dpi, c2, 350 / dpi, w2);
    lbOff(lbpay[1]);
    lbpay[2] = new QLabel("铳家    支付", this);
    lbpay[2]->setFont(ft2);
    lbpay[2]->setGeometry(130 / dpi, c2, 350 / dpi, w2);
    lbOff(lbpay[2]);
    lbstick = new QLabel("点棒", this);
    lbstick->setFont(ft2);
    lbstick->setGeometry(814 / dpi, c2, w1, w2);
    lbOff(lbstick);
    lbtm[0] = new QLabel("自摸", this);
    lbtm[0]->setFont(ft2);
    lbtm[0]->setGeometry(280 / dpi, c1, w1, w2);
    lbOff(lbtm[0]);
    lbtm[1] = new QLabel("荣和", this);
    lbtm[1]->setFont(ft2);
    lbtm[1]->setGeometry(280 / dpi, c1, w1, w2);
    lbOff(lbtm[1]);
    lbban = new QLabel("番", this);
    lbban->setFont(ft2);
    lbban->setGeometry(580 / dpi, c1, w1, w2);
    lbOff(lbban);
    lbfu = new QLabel("符", this);
    lbfu->setFont(ft2);
    lbfu->setGeometry(860 / dpi, c1, w1, w2);
    lbOff(lbfu);
    lbpt = new QLabel("·", this);
    lbpt->setFont(ft2);
    lbpt->setGeometry(600 / dpi, c2, 30 / dpi, w2);
    lbOff(lbpt);
    teban = new MTextEdit(this);
    teban->setFont(ft2);
    teban->setGeometry(420 / dpi, c1, 140 / dpi, w2);
    teban->setReadOnly(1);
    teOff(teban);
    tefu = new MTextEdit(this);
    tefu->setFont(ft2);
    tefu->setGeometry(650 / dpi, c1, 170 / dpi, w2);
    tefu->setReadOnly(1);
    teOff(tefu);
    tepay1 = new MTextEdit(this);
    tepay1->setFont(ft2);
    tepay1->setGeometry(420 / dpi, c2, 170 / dpi, w2);
    teOff(tepay1);
    tepay2 = new MTextEdit(this);
    tepay2->setFont(ft2);
    tepay2->setGeometry(620 / dpi, c2, 170 / dpi, w2);
    teOff(tepay2);
    tepay3 = new MTextEdit(this);
    tepay3->setFont(ft2);
    tepay3->setGeometry(450 / dpi, c2, 300 / dpi, w2);
    teOff(tepay3);
    btnans = new MyButton("做答", this);
    btnans->setFont(ft2);
    btnans->setGeometry(240 / dpi, c3, 320 / dpi, w2);
    btnans->changeToGreyRect();
    btnOff(btnans);
    btnnxt = new MyButton("下一题", this);
    btnnxt->setFont(ft);
    btnnxt->setGeometry(600 / dpi, c3, 180 / dpi, w2);
    btnnxt->changeToGreyRect();
    btnOff(btnnxt);
    connect(btnhjm, SIGNAL(clicked(bool)), this, SLOT(startTriggle()));
    connect(btnans, SIGNAL(clicked(bool)), this, SLOT(ansTriggle()));
    connect(btnnxt, SIGNAL(clicked(bool)), this, SLOT(nxtTriggle()));
    srand(time(NULL));
}

void MainWindow::setQuiz() {
    ply = rand() % 2;
    tm = rand() % 2;
    if (!ply && !tm) OyaTsumo();
    if (!ply && tm) OyaRong();
    if (ply && !tm) KoTsumo();
    if (ply && tm) KoRong();
    int banId = rand() % banSet.size();
    int fuId = rand() % fuSet.size();
    ban = banSet[banId];
    while (ban == 1 && fuSet[fuId] < 30) {
        fuId = rand() % fuSet.size();
    }
    while (tm && fuSet[fuId] == 20) {
        fuId = rand() % fuSet.size();
    }
    fu = fuSet[fuId];
    teban->setPlainText(QString::number(ban));
    tefu->setPlainText(QString::number(fu));
    tepay1->setPlainText("");
    tepay2->setPlainText("");
    tepay3->setPlainText("");
    btnnxt->setEnabled(0);
}

void MainWindow::OyaRong() {
    lbOn(lbply[0]);
    lbOff(lbply[1]);
    lbOn(lbpay[2]);
    lbOff(lbpay[0]);
    lbOff(lbpay[1]);
    lbOn(lbtm[1]);
    lbOff(lbtm[0]);
    lbOff(lbpt);
    teOn(tepay3);
    teOff(tepay1);
    teOff(tepay2);
}

void MainWindow::OyaTsumo() {
    lbOn(lbply[0]);
    lbOff(lbply[1]);
    lbOn(lbpay[0]);
    lbOff(lbpay[1]);
    lbOff(lbpay[2]);
    lbOn(lbtm[0]);
    lbOff(lbtm[1]);
    lbOff(lbpt);
    teOn(tepay3);
    teOff(tepay1);
    teOff(tepay2);
}

void MainWindow::KoTsumo() {
    lbOn(lbply[1]);
    lbOff(lbply[0]);
    lbOn(lbpay[1]);
    lbOff(lbpay[0]);
    lbOff(lbpay[2]);
    lbOn(lbtm[0]);
    lbOff(lbtm[1]);
    lbOn(lbpt);
    teOff(tepay3);
    teOn(tepay1);
    teOn(tepay2);
}

void MainWindow::KoRong() {
    lbOn(lbply[1]);
    lbOff(lbply[0]);
    lbOn(lbpay[2]);
    lbOff(lbpay[0]);
    lbOff(lbpay[1]);
    lbOn(lbtm[1]);
    lbOff(lbtm[0]);
    lbOff(lbpt);
    teOn(tepay3);
    teOff(tepay1);
    teOff(tepay2);
}

int MainWindow::hyakuBai(int x) {
    if (x / 10 % 10 == 0) return x;
    return (x / 100 + 1) * 100;
}

void MainWindow::Accepted() {
    btnans->setButtonState(0, "green");
    tepay1->setReadOnly(1);
    tepay2->setReadOnly(1);
    tepay3->setReadOnly(1);
    btnans->setText("正确");
}

void MainWindow::WrongAnswer() {
    btnans->setButtonState(0, "red");
    tepay1->setReadOnly(1);
    tepay2->setReadOnly(1);
    tepay3->setReadOnly(1);
    btnans->setText("错误");
}

void MainWindow::ansTriggle() {
    QString a1 = tepay1->toPlainText(), a2 = tepay2->toPlainText(), a3 = tepay3->toPlainText();
    if (!ply && a3 == "") return;
    if (ply && !tm && (a1 == "" || a2 == "")) return;
    int a = pow(2, ban + 2) * fu;
    if ((ban == 3 && fu >= 70) || (ban == 4 && fu >= 40) || ban == 5) a = 2000;
    if (ban == 6 || ban == 7) a = 3000;
    if (ban == 8 || ban == 9 || ban == 10) a = 4000;
    if (ban == 11 || ban == 12) a = 6000;
    if (ban == 13) a = 8000;
    //a = hyakuBai(a);
    int p1 = 0, p2 = 0;
    if (!ply && tm) {
        p1 = 6 * a;
    }
    if (!ply && !tm) {
        p1 = 2 * a;
    }
    if (ply && !tm) {
        p1 = a, p2 = 2 * a;
    }
    if (ply && tm) {
        p1 = 4 * a;
    }
    p1 = hyakuBai(p1), p2 = hyakuBai(p2);
    QString s1 = QString::number(p1), s2 = QString::number(p2);
    if (ply && !tm) {
        qDebug() << a1 << s1 << a2 << s2;
        if ((a1 == s1 && a2 == s2) || (a1 == s2 && a2 == s1)) Accepted();
        else WrongAnswer();
    } else {
        qDebug() << a3 << s1;
        if (a3 == s1) Accepted();
        else WrongAnswer();
    }

    btnnxt->setEnabled(1);// ############
    //btnans->setEnabled(0);
    this->repaint();
}

void MainWindow::nxtTriggle() {
    setQuiz();
    btnans->setText("做答");
    //btnans->setEnabled(1);// ##############
    btnans->setButtonState(1, "grey");
    tepay1->setReadOnly(0);
    tepay2->setReadOnly(0);
    tepay3->setReadOnly(0);
}

void MainWindow::lbOff(QLabel *lb) {
    lb->setVisible(0);
    lb->setEnabled(0);
}

void MainWindow::lbOn(QLabel *lb) {
    lb->setVisible(1);
    lb->setEnabled(1);
}

void MainWindow::teOff(MTextEdit *te) {
    te->setVisible(0);
    te->setEnabled(0);
}

void MainWindow::teOn(MTextEdit *te) {
    te->setVisible(1);
    te->setEnabled(1);
}

void MainWindow::btnOff(MyButton *btn) {
    btn->setVisible(0);
    btn->setEnabled(0);
}

void MainWindow::btnOn(MyButton *btn) {
    btn->setVisible(1);
    btn->setEnabled(1);
}

MainWindow::~MainWindow() {
    delete ui;
}
