#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    dpi  = QGuiApplication::primaryScreen()->logicalDotsPerInch() / 96;
    ui->setupUi(this);
    this->setFixedSize(1000 / dpi, 700 / dpi);
    this->setWindowTitle(QString("COCManage    By:Capzera       ") + VersionNumber);
    Init();
    MainView();
}
void MainWindow::Init() {
    QFont ft("等线", 16 / dpi);
    QFont ft2("等线", 12 / dpi);
    title = new QLabel("COCManage", this);
    title->setFont(QFont("Comic Sans MS", 54 / dpi));
    title->setGeometry((this->width() - 400) / 2 / dpi, 0, 400 / dpi, 150 / dpi);
    title2 = new QLabel("设置", this);
    title2->setFont(QFont("微软雅黑", 40 / dpi));
    title2->setGeometry((this->width() - 120) / 2 / dpi, 0, 120 / dpi, 60 / dpi);
    sid = new QLabel("昵称", this);
    sid->setFont(QFont("等线", 20 / dpi));
    sid->setGeometry(250 / dpi, 20 / dpi, 350 / dpi, 160 / dpi);
    sst = new QLabel("打星", this);
    sst->setFont(QFont("等线", 20 / dpi));
    sst->setGeometry(510 / dpi, 20 / dpi, 130 / dpi, 160 / dpi);
    spc = new QLabel("摧毁率", this);
    spc->setFont(QFont("等线", 20 / dpi));
    spc->setGeometry(645 / dpi, 20 / dpi, 130 / dpi, 160 / dpi);
    sth = new QLabel("对位编号", this);
    sth->setFont(QFont("等线", 20 / dpi));
    sth->setGeometry(780 / dpi, 20 / dpi, 130 / dpi, 160 / dpi);
    ran = new MyButton("查询", this);
    ran->setFont(ft);
    ran->setGeometry(100 / dpi, 580 / dpi, 220 / dpi, 90 / dpi);
    opt = new MyButton("设置", this);
    opt->setFont(ft);
    opt->setGeometry(390 / dpi, 580 / dpi, 220 / dpi, 90 / dpi);
    qui = new MyButton("退出", this);
    qui->setFont(ft);
    qui->setGeometry(680 / dpi, 580 / dpi, 220 / dpi, 90 / dpi);
    add = new MyButton("添加", this);
    add->setFont(ft);
    add->setGeometry(310 / dpi, 540 / dpi, 170 / dpi, 90 / dpi);
    del = new MyButton("删除", this);
    del->setFont(ft);
    del->setGeometry(520 / dpi, 540 / dpi, 170 / dpi, 90 / dpi);
    back = new MyButton("返回", this);
    back->setFont(ft);
    back->setGeometry(100 / dpi, 540 / dpi, 170 / dpi, 90 / dpi);
    qui2 = new MyButton("退出", this);
    qui2->setFont(ft);
    qui2->setGeometry(730 / dpi, 540 / dpi, 170 / dpi, 90 / dpi);
    ans = new MTextEdit(this);
    ans->setFont(QFont("楷体", 18 / dpi));
    ans->setGeometry(100 / dpi, 150 / dpi, 800 / dpi, 400 / dpi);
    ans->setReadOnly(1);
    tyid = new QPlainTextEdit(this);
    tyid->setGeometry(100 / dpi, 120 / dpi, 350 / dpi, 80 / dpi);
    tyid->setFont(QFont("等线", 16 / dpi));
    tyst = new QPlainTextEdit(this);
    tyst->setGeometry(470 / dpi, 120 / dpi, 130 / dpi, 80 / dpi);
    tyst->setFont(QFont("Consolas", 16 / dpi));
    typc = new QPlainTextEdit(this);
    typc->setGeometry(620 / dpi, 120 / dpi, 130 / dpi, 80 / dpi);
    typc->setFont(QFont("Consolas", 16 / dpi));
    tyth = new QPlainTextEdit(this);
    tyth->setGeometry(770 / dpi, 120 / dpi, 130 / dpi, 80 / dpi);
    tyth->setFont(QFont("Consolas", 16 / dpi));
    tv = new QListWidget(this);
    tv->setFont(QFont("楷体", 18 / dpi));
    tv->setGeometry(100 / dpi, 220 / dpi, 800 / dpi, 300 / dpi);
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
    file.write(QString::number(0).toUtf8());
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
    star = perc = town = cnt = QVector<int>(MAX_MENU);
    for (int i = 0; i < MAX_MENU; i++) { // 读入昵称
        QString line = ts.readLine();
        menu[i] = line;
    }
    for (int i = 0; i < MAX_MENU; i++) { // 读入打星
        QString line = ts.readLine();
        star[i] = line.toInt();
    }
    for (int i = 0; i < MAX_MENU; i++) { // 读入百分比
        QString line = ts.readLine();
        perc[i] = line.toInt();
    }
    for (int i = 0; i < MAX_MENU; i++) { // 读入打本
        QString line = ts.readLine();
        town[i] = line.toInt();
    }
    for (int i = 0; i < MAX_MENU; i++) { // 读入统计量
        QString line = ts.readLine();
        cnt[i] = line.toInt();
    }
    reFresh();
    file.close();
}


void MainWindow::Save() {
    QFile file("./save.dat");
    file.open(QFile::WriteOnly | QFile::Text);
    file.write(QString::number(MAX_MENU).toUtf8());
    file.write("\n");
    for (int i = 0; i < MAX_MENU; i++) {
        file.write(menu[i].toUtf8());
        file.write("\n");
    }
    for (int i = 0; i < MAX_MENU; i++) {
        file.write(QString::number(star[i]).toUtf8());
        file.write("\n");
    }
    for (int i = 0; i < MAX_MENU; i++) {
        file.write(QString::number(perc[i]).toUtf8());
        file.write("\n");
    }
    for (int i = 0; i < MAX_MENU; i++) {
        file.write(QString::number(town[i]).toUtf8());
        file.write("\n");
    }
    for (int i = 0; i < MAX_MENU; i++) {
        file.write(QString::number(cnt[i]).toUtf8());
        file.write("\n");
    }
    reFresh();
    file.close();
}

void MainWindow::reFresh() {
    tv->clear();
    for (int i = 0; i < MAX_MENU; i++) {
        QListWidgetItem *num = new QListWidgetItem();
        QListWidgetItem *dish = new QListWidgetItem();
        num->setText(QString::number(i + 1));
        double c1 = star[i] * 100 / cnt[i], c2 = perc[i] * 10000 / cnt[i], c3 = town[i] * 100 / cnt[i];
        double d1 = c1 / 100.0, d2 = c2 / 10000.0, d3 = c3 / 100.0;
        QString text = SetString(menu[i], 30) + SetString(QString::number(d1), 13) + SetString(QString::number(d2) + "%", 13) + SetString(QString::number(d3), 13);
        dish->setText(text);
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
    SetTextOff(tyid);
    SetTextOff(tyst);
    SetTextOff(typc);
    SetTextOff(tyth);
    SetLabelOn(title);
    SetLabelOff(title2);
    SetLabelOff(sid);
    SetLabelOff(sst);
    SetLabelOff(spc);
    SetLabelOff(sth);
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
    SetTextOn(tyid);
    SetTextOn(tyst);
    SetTextOn(typc);
    SetTextOn(tyth);
    SetLabelOff(title);
    SetLabelOn(title2);
    SetLabelOn(sid);
    SetLabelOn(sst);
    SetLabelOn(spc);
    SetLabelOn(sth);
    SetTableOn(tv);
}

void MainWindow::ranTriggle() {
    if (!MAX_MENU) {
        return;
    }
    Save();
    QVector<int> height(MAX_MENU);
    for (int i = 0; i < MAX_MENU; i++) {
        height[i] = i;
    }

    sort(height.begin(), height.end(), [&](const int& a, const int& b) -> bool {
        if (star[a] * cnt[b] != star[b] * cnt[a]) return star[a] * cnt[b] > star[b] * cnt[a];
        if (town[a] * cnt[b] != town[b] * cnt[a]) return town[a] * cnt[b] < town[b] * cnt[a];
        return perc[a] * cnt[b] > perc[b] * cnt[a];
    });
    QString text;
    text += SetString("玩家昵称", 20) + SetString("平均打星", 10) + SetString("平均摧毁率", 12) + SetString("平对位编号", 10) + "\n";
    for (int i = 0; i < MAX_MENU; i++) {
        int j = height[i];
        double c1 = star[j] * 100 / cnt[j], c2 = perc[j] * 10000 / cnt[j], c3 = town[j] * 100 / cnt[j];
        double d1 = c1 / 100.0, d2 = c2 / 10000.0, d3 = c3 / 100.0;
        text += SetString(menu[j], 20) + SetString(QString::number(d1), 10) + SetString(QString::number(d2) + "%", 12) + SetString(QString::number(d3), 10);
        text += "\n";
    }
    ans->setPlainText(text);
}
void MainWindow::optTriggle() {
    OptionView();
}
void MainWindow::quiTriggle() {
    Save();
    this->close();
}
void MainWindow::addTriggle() {
    QString qid = tyid->toPlainText();
    QString qst = tyst->toPlainText();
    QString qpc = typc->toPlainText();
    QString qth = tyth->toPlainText();
    if (qid == "" || qst == "" || qpc == "" || qth == "") { // 如果有没有填写的内容，直接退出添加
        return;
    }
    int loc = -1;
    for (int i = 0; i < MAX_MENU; i++) {
        if (menu[i] == qid) {
            loc = i;
            break;
        }
    }
    if (loc == -1) { // 如果该用户从未出现过
        MAX_MENU++;
        menu.push_back(qid);
        star.push_back(qst.toInt());
        perc.push_back(qpc.toInt());
        town.push_back(qth.toInt());
        cnt.push_back(1);
    } else { // 这是一个现存用户
        star[loc] += qst.toInt();
        perc[loc] += qpc.toInt();
        town[loc] += qth.toInt();
        cnt[loc]++;
    }
    Save();
    tyid->setPlainText("");
    tyst->setPlainText("");
    typc->setPlainText("");
    tyth->setPlainText("");
}
void MainWindow::delTriggle() {
    int pointer = tv->currentRow();
    if (pointer >= MAX_MENU || pointer < 0) {
        return;
    }
    tv->takeItem(pointer);
    for (int i = pointer; i < MAX_MENU - 1; i++) {
        menu[i] = menu[i + 1];
        star[i] = star[i + 1];
        perc[i] = perc[i + 1];
        town[i] = town[i + 1];
        cnt[i] = cnt[i + 1];
    }
    menu.pop_back(), star.pop_back(), perc.pop_back(), town.pop_back(), cnt.pop_back();
    MAX_MENU--;
    Save();
}
void MainWindow::backTriggle() {
    MainView();
}

QString MainWindow::SetString(QString s, int x) {
    int n = s.size();
    for (auto& x : s) {
        ushort uni = x.unicode();
        if (uni >= 0x4e00 && uni <= 0x9fa5) {
            n++;
        }
    }
    if (n >= x) return s;
    QString space(x - n, ' ');
    return s + space;
}

MainWindow::~MainWindow() {
    delete ui;
}
