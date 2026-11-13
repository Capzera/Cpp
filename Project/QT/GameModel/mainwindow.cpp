#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    dpi  = QGuiApplication::primaryScreen()->logicalDotsPerInch() / 96;
    ui->setupUi(this);
    init();
    Menu();
    this->setFixedSize(1000, 800);
    this->setWindowTitle(GameName + QString("  By:Capzera       ") + VersionNumber);
}

void MainWindow::init() {

    LEVEL = QVector<QVector<int> > (MAX_LEVEL + 1, QVector<int>(MAX_LEVEL + 1));
    levels.resize(MAX_LEVEL);
    FileRead();
    BestStep.resize(MAX_LEVEL + 1);
    QFont ft("等线", 18 / dpi);
    QFont ft2("等线", 14 / dpi);
    QFont ftB("Comic Sans MS", 54 / dpi);
    version = new QLabel(VersionNumber, this);
    version->setFont(ft2);
    version->move(20, 770);
    title = new QLabel(GameName, this);
    title->setFont(ftB);
    title->setGeometry((this->width() - title->width()) / 2, 150, 500, 200);
    curLevel = new QLabel("第 1 关", this);
    curLevel->setFont(ft);
    curLevel->setGeometry(825, 100, 200, 50);
    helper = new QLabel(this);
    helper->setWordWrap(true);
    helper->setText("NoWords");
    helper->setGeometry(0, 0, 820, 800);
    Log = new QPlainTextEdit(this);
    Log->setFont(QFont("Consolas", 16));
    Log->setPlainText(UpdateLog);
    Log->setReadOnly(true);
    Log->setGeometry(100, 80, 800, 550);
    start = new QPushButton("开始游戏", this);
    start->setFont(ft);
    start->setGeometry((this->width()) / 2, 380, 200, 80);
    newg = new QPushButton("存档清除", this);
    newg->setFont(ft);
    newg->setGeometry((this->width()) / 2, 480, 200, 80);
    option = new QPushButton("帮助", this);
    option->setFont(ft);
    option->setGeometry((this->width()) / 2, 580, 200, 80);
    end = new QPushButton("退出", this);
    end->setFont(ft);
    end->setGeometry((this->width()) / 2, 680, 200, 80);

    lastPage = new MyButton("上一页", this);
    lastPage->setFont(ft);
    lastPage->setGeometry(800, 500, 150, 100);
    nextPage = new MyButton("下一页", this);
    nextPage->setFont(ft);
    nextPage->setGeometry(800, 625, 150, 100);


    for (int i = 0; i < MAX_LEVEL; i++) {
        int x = i % 25 / 5, y = i % 25 % 5;
        levels[i] = new MyButton(this, i, 0);
        levels[i]->setText(QString::number(i + 1));
        levels[i]->setGeometry(50 + y * 145, 50 + x * 145, 120, 120);
        levels[i]->changeToGreyRect();
        levels[i]->setFont(ft);
        if (MyStep[i] != -1) {
            if (MyStep[i] == BestStep[i + 1]) levels[i]->changeToGreenRect();
            else levels[i]->changeToBlueRect();
        }
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
    /*wd = new QPushButton("上一步", this);
    wd->setFont(ft);
    wd->setGeometry(800, 420, 150, 60);
    retry = new QPushButton("重新挑战", this);
    retry->setFont(ft);
    retry->setGeometry(800, 500, 150, 60);*/
    bts = new QPushButton("关卡选择", this);
    bts->setFont(ft);
    bts->setGeometry(800, 620, 150, 60);
    connect(btm, SIGNAL(clicked(bool)), this, SLOT(Menu()));
    connect(bth, SIGNAL(clicked(bool)), this, SLOT(Help()));
    connect(UpDoc, SIGNAL(clicked(bool)), this, SLOT(LookUpdate()));
    connect(bts, SIGNAL(clicked(bool)),this, SLOT(SelectLevel()));
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
        if (line != "-1") CurMaxLevel = max(CurMaxLevel, i + 1);
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
    CurMaxLevel = 0;
}

void MainWindow::Menu() {

    openButton(start);
    openButton(newg);
    openButton(option);
    openButton(end);
    openLabel(version);
    openLabel(title);
    closeLabel(curLevel);
    closeLabel(helper);
    closeButton(bth);
    closeButton(btm);
    closeButton(UpDoc);
    closeButton(bts);
    closeButton(lastPage);
    closeButton(nextPage);
    qDebug() << 3;
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
    closeButton(bts);

    openButton(btm);
    openButton(lastPage);
    openButton(nextPage);
    if (!Page) lastPage->changeToGreyRect();
    else lastPage->changeToWhiteRect();
    if (Page < MAX_LEVEL / 25 - 1) nextPage->changeToWhiteRect();
    else nextPage->changeToGreyRect();
    levels[CurMaxLevel]->changeToOrangeRect();
    for (int i = 0; i < 25; i++) {
        openButton(levels[i]);
    }

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
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    int level = btn->text().toInt();
    if (level - 1 > CurMaxLevel) return;
    closeButton(btm);
    closeButton(lastPage);
    closeButton(nextPage);
    openButton(bts);
    openLabel(curLevel);
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
}

void MainWindow::QuitGame() {
    FileWrite();
    this->close();
}

void MainWindow::operat(int number) {

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

MainWindow::~MainWindow()
{
    delete ui;
}
