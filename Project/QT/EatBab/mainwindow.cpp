#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    dpi  = QGuiApplication::primaryScreen()->logicalDotsPerInch() / 96;
    qDebug() << dpi;
    ui->setupUi(this);
    this->setFixedSize(600, 450);
    this->setWindowTitle(QString("EatBab    By:Capzera       ") + VersionNumber);
    Init();
    MainView();
}
void MainWindow::Init() {
    lst1 = lst2 = -1;
    QFont ft("等线", 16 / dpi);
    QFont ft2("等线", 12 / dpi);
    title = new QLabel("Eat Bab", this);
    title->setFont(QFont("Comic Sans MS", 54 / dpi));
    title->setGeometry(170, 0, 400, 150);
    ran = new MyButton("随机", this);
    ran->setFont(ft);
    ran->setGeometry(100, 330, 180, 90);
    opt = new MyButton("设置", this);
    opt->setFont(ft);
    opt->setGeometry(300, 330, 90, 90);
    qui = new MyButton("退出", this);
    qui->setFont(ft);
    qui->setGeometry(410, 330, 90, 90);
    add = new MyButton("添加", this);
    add->setFont(ft);
    add->setGeometry(200, 75, 100, 50);
    del = new MyButton("删除", this);
    del->setFont(ft);
    del->setGeometry(200, 180, 100, 50);
    back = new MyButton("返回", this);
    back->setFont(ft);
    back->setGeometry(25, 355, 150, 70);
    qui2 = new MyButton("退出", this);
    qui2->setFont(ft);
    qui2->setGeometry(425, 355, 150, 70);
    ans = new MTextEdit(this);
    ans->setFont(QFont("等线", 28 / dpi));
    ans->setGeometry(100, 150, 400, 150);
    ans->setReadOnly(1);
    type = new QPlainTextEdit(this);
    type->setGeometry(25, 25, 150, 250);
    type->setFont(QFont("等线", 20 / dpi));
    tv = new QListWidget(this);
    tv->setFont(ft);
    tv->setGeometry(325, 25, 250, 300);
    connect(ran, SIGNAL(clicked(bool)), this, SLOT(ranTriggle()));
    connect(opt, SIGNAL(clicked(bool)), this, SLOT(optTriggle()));
    connect(qui, SIGNAL(clicked(bool)), this, SLOT(quiTriggle()));
    connect(add, SIGNAL(clicked(bool)), this, SLOT(addTriggle()));
    connect(del, SIGNAL(clicked(bool)), this, SLOT(delTriggle()));
    connect(back, SIGNAL(clicked(bool)), this, SLOT(backTriggle()));
    connect(qui2, SIGNAL(clicked(bool)), this, SLOT(quiTriggle()));
    Read();
}

void MainWindow::Reset() {
    QFile file("./save.dat");
    file.open(QIODevice::WriteOnly);
    file.write(QString::number(0).toLocal8Bit());
    file.write("\n");
    file.write(QString::number(-1).toLocal8Bit());
    file.write("\n");
    file.write(QString::number(-1).toLocal8Bit());
    file.write("\n");
    file.close();
}

void MainWindow::Read() {
    QFile file("./save.dat");
    if (!file.exists()) {
        Reset();
    }
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream ts(&file);
    MAX_MENU = ts.readLine().toInt();
    menu.resize(MAX_MENU);
    for (int i = 0; i < MAX_MENU; i++) {
        QString line = ts.readLine();
        menu[i] = line;
    }
    lst1 = ts.readLine().toInt();
    lst2 = ts.readLine().toInt();
    reFresh();
    file.close();
}


void MainWindow::Save() {
    QFile file("./save.dat");
    file.open(QFile::WriteOnly | QFile::Text);
    file.write(QString::number(MAX_MENU).toLocal8Bit());
    file.write("\n");
    for (int i = 0; i < MAX_MENU; i++) {
        file.write(menu[i].toLocal8Bit());
        file.write("\n");
    }
    file.write(QString::number(lst1).toLocal8Bit());
    file.write("\n");
    file.write(QString::number(lst2).toLocal8Bit());
    file.write("\n");
    reFresh();
    file.close();
}

void MainWindow::reFresh() {
    tv->clear();
    for (int i = 0; i < MAX_MENU; i++) {
        QListWidgetItem *num = new QListWidgetItem();
        QListWidgetItem *dish = new QListWidgetItem();
        num->setText(QString::number(i + 1));
        dish->setText(menu[i]);
        tv->addItem(dish);
    }
}

void MainWindow::SetTextOn(QPlainTextEdit *pte) {
    pte->setVisible(1);
    pte->setEnabled(1);
}

void MainWindow::SetTextOff(QPlainTextEdit *pte) {
    pte->setVisible(0);
    pte->setEnabled(0);
}

void MainWindow::SetTextOn(MTextEdit *te) {
    te->setVisible(1);
    te->setEnabled(1);
}

void MainWindow::SetTextOff(MTextEdit *te) {
    te->setVisible(0);
    te->setEnabled(0);
}

void MainWindow::SetButtonOn(MyButton *btn) {
    btn->setVisible(1);
    btn->setEnabled(1);
}

void MainWindow::SetButtonOff(MyButton *btn) {
    btn->setVisible(0);
    btn->setEnabled(0);
}

void MainWindow::SetLabelOn(QLabel *lb) {
    lb->setVisible(1);
    lb->setEnabled(1);
}

void MainWindow::SetLabelOff(QLabel *lb) {
    lb->setVisible(0);
    lb->setEnabled(0);
}

void MainWindow::SetTableOn(QListWidget *qtv) {
    qtv->setVisible(1);
    qtv->setEnabled(1);
}

void MainWindow::SetTableOff(QListWidget *qtv) {
    qtv->setVisible(0);
    qtv->setEnabled(0);
}

void MainWindow::MainView() {
    SetButtonOn(ran);
    SetButtonOn(opt);
    SetButtonOn(qui);
    SetButtonOff(back);
    SetButtonOff(add);
    SetButtonOff(del);
    SetButtonOff(qui2);
    SetTextOn(ans);
    SetTextOff(type);
    SetLabelOn(title);
    SetTableOff(tv);
}

void MainWindow::OptionView() {
    SetButtonOff(ran);
    SetButtonOff(opt);
    SetButtonOff(qui);
    SetButtonOn(back);
    SetButtonOn(add);
    SetButtonOn(del);
    SetButtonOn(qui2);
    SetTextOff(ans);
    SetTextOn(type);
    SetLabelOff(title);
    SetTableOn(tv);
}

void MainWindow::ranTriggle() {
    if (!MAX_MENU) {
        return;
    }
    int x = rand() % MAX_MENU;
    while (MAX_MENU == 2 && x == lst2) {
        x = rand() % MAX_MENU;
    }
    while (MAX_MENU > 2 && (x == lst1 || x == lst2)) {
        x = rand() % MAX_MENU;
    }
    if (MAX_MENU == 2) {
        lst2 = x;
    }
    if (MAX_MENU > 2) {
       lst1 = lst2, lst2 = x;
    }
    Save();
    ans->setPlainText(menu[x]);
    qDebug() << x;
}
void MainWindow::optTriggle() {
    OptionView();
}
void MainWindow::quiTriggle() {
    Save();
    this->close();
}
void MainWindow::addTriggle() {
    QString newDish = type->toPlainText();
    if (newDish == "") {
        return;
    }
    MAX_MENU++;
    menu.push_back(newDish);
    Save();
    type->setPlainText("");
}
void MainWindow::delTriggle() {
    int pointer = tv->currentRow();
    qDebug() << pointer;
    if (pointer >= MAX_MENU || pointer < 0) {
        return;
    }
    tv->takeItem(pointer);
    for (int i = pointer; i < MAX_MENU - 1; i++) {
        menu[i] = menu[i + 1];
    }
    menu.pop_back();
    MAX_MENU--;
    Save();
}
void MainWindow::backTriggle() {
    MainView();
}

MainWindow::~MainWindow() {
    delete ui;
}
