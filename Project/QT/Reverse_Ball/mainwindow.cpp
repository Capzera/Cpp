#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    dpi  = QGuiApplication::primaryScreen()->logicalDotsPerInch() / 96;
    qDebug() << dpi;
    ui->setupUi(this);
    init();
    Menu();
    this->setFixedSize(1000, 800);
    this->setWindowTitle(QString("Cross Bubble  By:Capzera       ") + VersionNumber);
}

void MainWindow::init() {
    FileRead();
    LEVEL = {
        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24},//0关
        {0, 1, 3, 4, 5, 6, 8, 9, 15, 16, 18, 19, 20, 21, 23, 24},//level 1   12
        {1, 2, 3, 4, 6, 7, 8, 9, 16, 17, 18, 19, 21, 22, 23, 24},//level 2   10
        {4, 6, 7, 8, 11, 12, 13, 16, 17, 18, 20}, //level 3   0 24
        {1, 2, 3, 6, 7, 8, 10, 11, 12, 13, 14, 16, 17, 18, 21, 22, 23}, //level 4   10 14
        {1, 3, 6, 8, 16, 18, 21, 23}, //level 5   10 12 14
        {1, 2, 3, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23}, //level 6   0 4 20 24
        {0, 1, 2, 3, 4, 5, 7, 9, 10, 11, 12, 13, 14, 15, 17, 19, 20, 21, 22, 23, 24}, //level 7   6 8 16 18
        {0, 2, 4, 6, 7, 8, 10, 11, 12, 13, 14, 16, 17, 18, 20, 22, 24}, //level 8  2 10 14 22
        {0, 1, 2, 3, 4, 6, 7, 8, 10, 11, 12, 13, 14, 16, 17, 18, 20, 21, 22, 23, 24}, //level 9   5 9 15 19
        {0, 4, 7, 8, 11, 13, 16, 17, 20, 24}, //level 10   6 12 18
        {0, 4, 6, 8, 10, 11, 13, 14, 16, 18, 21, 23}, //level 11    2 10 14
        {2, 4, 6, 8, 10, 14, 16, 18, 20, 22}, //level 12  0 12 24
        {1, 3, 5, 9, 10, 14, 15, 16, 18, 19, 20, 24}, //level 13  2 16 18
        {1, 3, 5, 9, 10, 11, 13, 14, 16, 18, 21, 23}, //level 14   7 10 14
        {2, 4, 6, 8, 10, 11, 15, 18, 22, 24}, //level 15   5 13 16
        {4, 5, 7, 9, 12, 15, 17, 19, 20, 21, 23}, //level 16  2 5 14 15
        {0, 1, 4, 6, 8, 9, 12, 15, 16, 18, 20, 23, 24}, //level 17   3 5 19 21
        {2, 5, 7, 9, 11, 12, 13, 17, 22}, //level 18  5 9 11 13
        {4, 6, 7, 8, 10, 12, 13, 15, 16, 18, 20, 21, 22}, //level 19  5 11 17 23
        {5, 9, 16, 18, 20, 21, 23, 24}, //level 20   5 9 16 18 22
        {5, 8, 15, 18}, //level 21   3 5 13 15 23
        {0, 1, 2, 5, 12, 13, 14, 15, 18, 19, 22, 23, 24}, //level 22   2 6 15 17
        {0, 1, 3, 6, 8, 9, 11, 12, 13, 17, 22}, //level 23   4 5 11 13
        {1, 2, 5, 6, 7, 8, 13, 14, 15, 18, 23, 24}, //level 24   0 8 16 17 18
        {0, 1, 3, 4, 5, 6, 7, 9, 10, 11, 12, 13, 16, 17, 18, 19, 20, 22, 23, 24}, //level 25  2 8 14 15 21
        {2, 6, 7, 8, 10, 11, 12, 13, 14, 16, 17, 18, 22}, //level 26  2 5 9 15 19 22
        {1, 4, 5, 8, 12, 13, 15, 17, 21, 24}, //level 27  7 10 18
        {4, 6, 7, 8, 10, 11, 12, 16, 17, 19, 24}, //level 28   5 13 16 17
        {0, 3, 4, 9, 10, 11, 13, 19, 20, 22, 23}, //level 29   0 3 12 21
        {2, 5, 7, 9, 10, 11, 12, 13, 14, 16, 17, 18, 20, 21, 22, 23, 24}, //level 30   5 9 12 16 18 22
    };

    BestStep = QVector<int> {
        0, 1, 1, 2, 2, 3, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 4, 4, 5, 5, 6, 3, 4, 4, 6, 0,
    };

    QFont ft("等线", 18 / dpi);
    QFont ft2("等线", 14 / dpi);
    QFont ftB("Comic Sans MS", 54 / dpi);
    version = new QLabel(VersionNumber, this);
    version->setFont(ft2);
    version->move(20, 770);
    title = new QLabel("Cross Bubble", this);
    title->setFont(ftB);
    title->setGeometry(285, 150, 500, 200);
    curLevel = new QLabel("第 1 关", this);
    curLevel->setFont(ft);
    curLevel->setGeometry(825, 100, 200, 50);
    curStep = new QLabel("当前步数:", this);
    curStep->setFont(ft);
    curStep->setGeometry(770, 175, 200, 50);
    minStep = new QLabel("最少步数:", this);
    minStep->setFont(ft);
    minStep->setGeometry(770, 250, 200, 50);
    MybestStep = new QLabel("历史最佳步数:", this);
    MybestStep->setFont(ft);
    MybestStep->setGeometry(770, 325, 200, 50);
    helper = new QLabel(this);
    helper->setFont(QFont("楷体", 20));
    helper->setWordWrap(true);
    helper->setText("欢迎您游玩《Cross Bubble》\n这是一款单机解谜游戏，游戏开始时，玩家会看到一个5 x 5的泡泡棋盘，泡泡有黑色和绿色。把泡泡全部变成绿色即为过关。\n玩家点击一个泡泡会使一个十字形范围的泡泡全变成相反的颜色：\n黑色泡泡变成绿色泡泡，绿色泡泡变成黑色泡泡。\n目前实装的内容有30关，祝您游玩愉快！\n感谢您的体验！\n\n如果您遇到游戏内Bug，可以发送邮件给我：1786126188@qq.com\n\n\n制作人：Capzera");
    helper->setGeometry(0, 0, 820, 800);
    Log = new QPlainTextEdit(this);
    Log->setFont(QFont("Consolas", 16));
    Log->setPlainText(UpdateLog);
    Log->setReadOnly(true);
    Log->setGeometry(100, 80, 800, 550);
    start = new QPushButton("开始游戏", this);
    start->setFont(ft);
    start->setGeometry(400, 380, 200, 80);
    newg = new QPushButton("存档清除", this);
    newg->setFont(ft);
    newg->setGeometry(400, 480, 200, 80);
    option = new QPushButton("帮助", this);
    option->setFont(ft);
    option->setGeometry(400, 580, 200, 80);
    end = new QPushButton("退出", this);
    end->setFont(ft);
    end->setGeometry(400, 680, 200, 80);

    lastPage = new MyButton("上一页", this);
    lastPage->setFont(ft);
    lastPage->setGeometry(800, 500, 150, 100);
    nextPage = new MyButton("下一页", this);
    nextPage->setFont(ft);
    nextPage->setGeometry(800, 625, 150, 100);


    buttons.resize(25);
    for (int j = 0; j < 5; j++) {
        for (int i = 0; i < 5; i++) {
            int k = j * 5 + i;
            buttons[k] = new MyButton(this, k, 0);
            buttons[k]->setText(QString::number(k));
            buttons[k]->setGeometry(50 + i * 145, 50 + j * 145, 120, 120);
            buttons[k]->changeToBlackCircle();
            connect(buttons[k], SIGNAL(clicked(bool)), this, SLOT(pushed()));
        }
    }
    levels.resize(MAX_LEVEL);
    for (int i = 0; i < MAX_LEVEL; i++) {
        int x = i % 25 / 5, y = i % 25 % 5;
        levels[i] = new MyButton(this, i, 0);
        levels[i]->setText(QString::number(i + 1));
        levels[i]->setGeometry(50 + y * 145, 50 + x * 145, 120, 120);
        levels[i]->changeToGreyRect();
        levels[i]->setFont(ft);
        if (MyStep[i] != -1) levels[i]->changeToGreenRect();
        connect(levels[i], SIGNAL(clicked(bool)), this, SLOT(GameStart()));
    }

    UpDoc = new QPushButton("版本", this);
    UpDoc->setFont(ft);
    UpDoc->setGeometry(825, 610, 100, 100);
    btm = new MyButton("返回", this);  //返回菜单
    btm->changeToWhiteRect();
    btm->setGeometry(825, 100, 100, 300);
    bth = new MyButton("返回", this);  //返回帮助
    bth->changeToWhiteRect();
    bth->setGeometry(250, 650, 500, 80);
    wd = new QPushButton("上一步", this);
    wd->setFont(ft);
    wd->setGeometry(800, 420, 150, 60);
    retry = new QPushButton("重新挑战", this);
    retry->setFont(ft);
    retry->setGeometry(800, 500, 150, 60);
    bts = new QPushButton("关卡选择", this);
    bts->setFont(ft);
    bts->setGeometry(800, 620, 150, 60);
    connect(btm, SIGNAL(clicked(bool)), this, SLOT(Menu()));
    connect(bth, SIGNAL(clicked(bool)), this, SLOT(Help()));
    connect(UpDoc, SIGNAL(clicked(bool)), this, SLOT(LookUpdate()));
    connect(bts, SIGNAL(clicked(bool)),this, SLOT(SelectLevel()));
    connect(retry, SIGNAL(clicked(bool)), this, SLOT(Retry()));
    connect(wd, SIGNAL(clicked(bool)), this, SLOT(Withdraw()));
    connect(start, SIGNAL(clicked(bool)), this, SLOT(SelectLevel()));
    connect(newg, SIGNAL(clicked(bool)), this, SLOT(NewGame()));
    connect(option, SIGNAL(clicked(bool)), this, SLOT(Help()));
    connect(end, SIGNAL(clicked(bool)), this, SLOT(QuitGame()));
    connect(lastPage, SIGNAL(clicked(bool)), this, SLOT(PageMinus()));
    connect(nextPage, SIGNAL(clicked(bool)), this, SLOT(PagePlus()));
}

void MainWindow::FileRead() {
    QFile file("./save.dat");
    if (!file.exists()) {
        FileReset();
    }
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    MyStep.resize(MAX_LEVEL);
    QTextStream ts(&file);
    for (int i = 0; i < MAX_LEVEL; i++) {
        QString line = ts.readLine();
        MyStep[i] = line.toInt();
    }
    file.close();
}

void MainWindow::FileWrite() {
    QFile file("./save.dat");
    file.open(QFile::WriteOnly | QFile::Text);
    for (int i = 0; i < MAX_LEVEL; i++) {
        file.write(QString::number(MyStep[i]).toLocal8Bit());
        file.write("\n");
    }
    file.close();
}

void MainWindow::FileReset() {
    QFile file("./save.dat");
    file.open(QIODevice::WriteOnly);
    for (int i = 0; i < MAX_LEVEL; i++) {
        file.write(QString::number(-1).toLocal8Bit());
        file.write("\n");
    }
    file.close();
    for (int i = 0; i < MAX_LEVEL; i++) {
        levels[i]->changeToGreyRect();
    }
}

void MainWindow::Menu() {
    openButton(start);
    openButton(newg);
    openButton(option);
    openButton(end);
    openLabel(version);
    openLabel(title);
    closeLabel(curLevel);
    closeLabel(curStep);
    closeLabel(minStep);
    closeLabel(MybestStep);
    closeLabel(helper);
    closeButton(bth);
    closeButton(btm);
    closeButton(UpDoc);
    closeButton(bts);
    closeButton(retry);
    closeButton(wd);
    closeButton(lastPage);
    closeButton(nextPage);
    for (int i = 0; i < 25; i++) closeButton(buttons[i]);
    for (int i = 0; i < MAX_LEVEL; i++) closeButton(levels[i]);
    Log->setVisible(false);
}

void MainWindow::SelectLevel() {
    Page = 0;
    closeButton(start);
    closeButton(newg);
    closeButton(option);
    closeButton(end);
    closeLabel(version);
    closeLabel(title);
    closeLabel(curLevel);
    closeLabel(curStep);
    closeLabel(minStep);
    closeLabel(MybestStep);
    closeButton(bts);
    closeButton(retry);
    closeButton(wd);
    openButton(btm);
    openButton(lastPage);
    openButton(nextPage);
    if (!Page) lastPage->changeToGreyRect();
    else lastPage->changeToWhiteRect();
    if (Page < MAX_LEVEL / 25 - 1) nextPage->changeToWhiteRect();
    else nextPage->changeToGreyRect();
    for (int i = 0; i < 25; i++) openButton(levels[i]);
}

void MainWindow::Help() {
    closeButton(start);
    closeButton(option);
    closeButton(newg);
    closeButton(end);
    closeButton(bth);
    closeLabel(version);
    closeLabel(title);
    openButton(btm);
    openButton(UpDoc);
    openLabel(helper);
    Log->setVisible(false);
}

void MainWindow::GameStart() {
    closeButton(btm);
    closeButton(lastPage);
    closeButton(nextPage);
    openButton(bts);
    openButton(retry);
    openButton(wd);
    openLabel(curLevel);
    openLabel(curStep);
    openLabel(minStep);
    openLabel(MybestStep);
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    int level = btn->text().toInt();
    for (int i = 0; i < 25; i++) openButton(buttons[i]);
    for (int i = 0; i < MAX_LEVEL; i++) closeButton(levels[i]);
    GameInit(level);
}

void MainWindow::LookUpdate() {
    closeButton(btm);
    closeButton(UpDoc);
    closeLabel(helper);
    openButton(bth);
    Log->setVisible(true);
}

void MainWindow::NewGame() {
    QMessageBox *mb = new QMessageBox;
    mb->setWindowTitle("清除存档");
    mb->setText("是否需要清除存档记录\n注意！该操作不可逆！");
    mb->addButton("确定", QMessageBox::AcceptRole);
    mb->addButton("取消", QMessageBox::RejectRole);
    int ret = mb->exec();
    if (ret) return;
    FileReset();
    FileRead();
    QMessageBox *ok = new QMessageBox;
    ok->setWindowTitle("已完成");
    ok->setText("您的存档已重置");
    ok->addButton("确定", QMessageBox::AcceptRole);
    ok->exec();
}
void MainWindow::Retry() {
    GameInit(NowLevel);
}

void MainWindow::GameInit(int level) {
    NowLevel = level;
    Step = 0;
    if (level >= LEVEL.size()) return;
    stepStack.clear();
    curLevel->setText("第 " + QString::number(level) + " 关");
    curStep->setText("当前步数: " + QString::number(Step));
    minStep->setText("最佳步数: " + QString::number(BestStep[level]));
    QString tmp;
    if (MyStep[level - 1] == -1) tmp = "历史最佳步数: N/A";
    else tmp = "历史最佳步数: " + QString::number(MyStep[level - 1]);
    MybestStep->setText(tmp);
    for (int i = 0; i < 25; i++) buttons[i]->changeToBlackCircle();
    for (int i = 0; i < LEVEL[level].size(); i++) {
        buttons[LEVEL[level][i]]->changeToGreenCircle();
    }
}

void MainWindow::Withdraw() {
    if (stepStack.empty()) return;
    int op = stepStack.top();
    stepStack.pop();
    operat(op);
    curStep->setText("当前步数: " + QString::number(--Step));
}

void MainWindow::QuitGame() {
    FileWrite();
    this->close();
}

void MainWindow::pushed() {
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    int number = btn->text().toInt();
    curStep->setText("当前步数: " + QString::number(++Step));
    stepStack.push(number);
    operat(number);
    int flag = 1;
    for (int i = 0; flag && i < 25; i++) {
        if (buttons[i]->getState() == 0) {
            flag = 0;
        }
    }
    if (flag == 1) GameWin(NowLevel);
}

void MainWindow::GameWin(int level) {
    levels[level - 1]->changeToGreenRect();
    if (MyStep[level - 1] == -1) MyStep[level - 1] = Step;
    else MyStep[level - 1] = min(MyStep[level - 1], Step);
    QString tmp;
    if (MyStep[level - 1] == -1) tmp = "历史最佳步数: N/A";
    else tmp = "历史最佳步数: " + QString::number(MyStep[level - 1]);
    MybestStep->setText(tmp);
    FileWrite();
    QMessageBox *mb = new QMessageBox;
    mb->setWindowTitle("挑战成功");
    mb->setText("是否进入下一关");
    mb->addButton("关卡选择", QMessageBox::AcceptRole);
    mb->addButton("下一关", QMessageBox::RejectRole);
    int ret = mb->exec();
    switch (ret) {
        case 0:
            SelectLevel();
        case 1:
            GameInit(level + 1);
    }
}

void MainWindow::operat(int number) {
    int l = number / 5 * 5, r = l + 5;
    change(buttons[number]);
    for (int m = 1; m < 5; m++) {
        if (number + m < r) change(buttons[number + m]);
        if (number - m >= l) change(buttons[number - m]);
        if (number - m * 5 >= 0) change(buttons[number - m * 5]);
        if (number + m * 5 < 25) change(buttons[number + m * 5]);
        QEventLoop loop;
        QTimer::singleShot(60, &loop, SLOT(quit()));
        loop.exec();
    }
    QString tmp;
    for (int i = 0; i < 25; i++) {
        if (buttons[i]->getState() == 0) tmp += QString::number(i) + ", ";
    }
    tmp.chop(1);tmp.chop(1);
    qDebug() << tmp;
}

void MainWindow::change(MyButton *btn) {
    if (btn->getState()) btn->changeToBlackCircle();
    else btn->changeToGreenCircle();
}

void MainWindow::PageMinus() {
    if (!Page) return;
    Page--;
    if (!Page) lastPage->changeToGreyRect();
    if (Page + 1 != MAX_LEVEL / 25) nextPage->changeToWhiteRect();
    for (int i = 0; i < MAX_LEVEL; i++) {
        closeButton(levels[i]);
    }
    for (int i = 0; i < 25; i++) {
        openButton(levels[i + 25 * Page]);
    }
}

void MainWindow::PagePlus() {
    if (Page + 1 == MAX_LEVEL / 25) return;
    Page++;
    if (Page + 1 == MAX_LEVEL / 25) nextPage->changeToGreyRect();
    if (Page) lastPage->changeToWhiteRect();
    for (int i = 0; i < MAX_LEVEL; i++) {
        closeButton(levels[i]);
    }
    for (int i = 0; i < 25; i++) {
        openButton(levels[i + 25 * Page]);
    }
}

void MainWindow::closeLabel(QLabel *lb) {
    lb->setVisible(false);
}

void MainWindow::openLabel(QLabel *lb) {
    lb->setVisible(true);
}

void MainWindow::closeButton(QPushButton *btn) {
    btn->setVisible(false);
    btn->setEnabled(false);
}

void MainWindow::closeButton(MyButton *btn) {
    btn->setVisible(false);
    btn->setEnabled(false);
}

void MainWindow::openButton(QPushButton *btn) {
    btn->setVisible(true);
    btn->setEnabled(true);
}

void MainWindow::openButton(MyButton *btn) {
    btn->setVisible(true);
    btn->setEnabled(true);
}

MainWindow::~MainWindow() {
    delete ui;
}
