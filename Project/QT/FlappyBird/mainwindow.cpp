#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QDebug>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    init();
    initGame();
}

MainWindow::~MainWindow() {
    delete pipeChannels;
}

void MainWindow::paintEvent(QPaintEvent*) {//画面更新的画图预设
    DrawBack();
    DrawPipe();
    DrawBird();
    if (gameStatus == GameStatus::STOPING) DrawTitle();
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
     //QSound::play(WINGPATH);//鸟上升时扑打翅膀的声音
    if (event->button() == Qt::LeftButton)
        birdStatus = BirdStatus::UP;
    initSpeed();
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_W) birdStatus = BirdStatus::UP;
    initSpeed();
}
//初始化
void MainWindow::init()
{
    //QSound::play(BGMPATH);
    this->setFixedSize(480, 640);
    this->setWindowIcon(QIcon(":/myimage/icon/bird1.png"));
    this->setWindowTitle("Flappy Bird");

    ground = QPixmap(":/myimage/icon/land.png");
    ground = ground.scaled(20,80); // 缩放
    int groundHeight = ground.height();
    background = QPixmap(":/myimage/icon/backgroundday.png");
    background = background.scaled(this->width(), this->height() - groundHeight); //缩放
    groundSize = this->width()/ground.width() + 1; //防止除不尽有余数，所以加1
    v = new QVector<int>(groundSize);
    title = QPixmap(":/myimage/icon/title.png");
    title = title.scaled(178, 48);

    for(int i = 0; i < groundSize; i++){
         int x = i * ground.width();
         (*v)[i] = x;
    }
    bird1 = QPixmap(":/myimage/icon/bird1.png");
    bird1 = bird1.scaled(60, 42);
    bird2 = QPixmap(":/myimage/icon/bird2.png");
    bird2 = bird2.scaled(60, 42);
    bird3 = QPixmap(":/myimage/icon/bird3.png");
    bird3 = bird3.scaled(60, 42);

    scoreLabel = new QLabel(this);
    scoreLabel->setGeometry(width()/2, 0, 50, 50);

    QFont font ( "Microsoft YaHei", 20, 75); //第一个属性是字体（微软雅黑），第二个是大小，第三个是加粗（权重是75）
    scoreLabel->setFont(font);
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::white);
    scoreLabel->setPalette(pa);
    this->score = 0;
    scoreLabel->setText(QString::number(this->score));

    //最高分数设置
    scoreLabel1 = new QLabel(this);
    scoreLabel1->setGeometry(0, this->height() - 50, 250, 50);

    scoreLabel1->setFont(font);
    scoreLabel1->setPalette(pa);
    scoreLabel1->setText("best score " + QString::number(best.score));

    startButton = new QPushButton(this);
    font = QFont( "Microsoft YaHei", 25, 75); //第一个属性是字体（微软雅黑），第二个是大小，第三个是加粗（权重是75）
    startButton->setFont(font);
    startButton->setPalette(pa);
    QLabel* label = new QLabel();
    label->setFixedSize(100, 60);
    label->setStyleSheet("QLabel{border-image:url(:/myimage/icon/play.png)}");
    QHBoxLayout* myLayout = new QHBoxLayout();
    myLayout->addWidget(label);
    myLayout->setContentsMargins(0, 0, 0, 0);
    startButton->setLayout(myLayout);
    startButton->setFlat(true);
    startButton->setGeometry(width()/2 - 50, height()/2 - 30, 100, 60);
    timer = new QTimer(this);
    //事件
    connect(startButton,SIGNAL(clicked(bool)),this,SLOT(Start()));
    connect(timer,SIGNAL(timeout()),this,SLOT(LoopPaint()));

}
//初始化游戏相关
void MainWindow::initGame()
{
    best.read();
    birdX = this->width() / 3;
    birdY = this->height() / 2 - bird1.height();
    this->score = 0;
    scoreLabel->setText(QString::number(this->score));
    scoreLabel1->setText("best score " + QString::number(best.score));
    uint seed_x = static_cast<uint>(clock());
    int l = Globalutils::getRandom(seed_x, 200); //获取随机数
    if(pipeChannels == nullptr) {
        pipeChannels = new QVector<PipeChannel*>(2); //创建两个通道
    }
    else {
        delete (*pipeChannels)[0];
        delete (*pipeChannels)[1];
    }
    (*pipeChannels)[0] = new PipeChannel(height(), 100+l, width()+100, ground.height(), this);//窗口高度 底下管道高度 x位置横坐标 地面高度 objet parent
    (*pipeChannels)[1] = new PipeChannel(height(), 150 +l, 2 * width() + l, ground.height(), this);
}
//初始化速度
void MainWindow::initSpeed() {
     birdDownSpeed = 2.5;
     birdUpSpeed = 5.0;
}
//开始游戏
void MainWindow::StartGame() {
    //QSound::play(POINTPATH);
    startButton->hide();
    initGame();
    initSpeed();
    gameStatus = GameStatus::RUNNING;
    birdStatus = BirdStatus::DOWN;
    timer->start(10);
}
//停止游戏
void MainWindow::StopGame() {
    timer->stop();
    gameStatus = GameStatus::STOPING;
    startButton->show();
    if(score > best.score) {
        best.score = score;
        best.save();
    }
}
//绘制标题
void MainWindow::DrawTitle() {
    QPainter painter(this);
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::white);
    QFont Font("Microsoft YaHei", 10, 40);
    int width = title.width();
    painter.drawPixmap(this->width()/2 - width/2, 200, title);
    painter.setFont(Font);
    painter.setPen(QColor(255, 255, 255));
    painter.drawText(this->width()/2 - 35, 275, "By:机长大大");
}

//绘制背景
void MainWindow::DrawBack() {
     QPainter painter(this);
     int height = ground.height();
     int pos;
     // 绘制背景图
     painter.drawPixmap(0,0, background);
     // 绘制地面
     for(int i = 0; i < groundSize; i++) {
         pos = (*v)[i];
         painter.drawPixmap(pos,this->height() - height, ground);//绘制一个地面图像
         // 改变坐标 移动起来
         pos -= moveSpeed;
         if(pos <= -ground.width())
             pos = (groundSize - 1) *  ground.width();
         (*v)[i] = pos;
     }
}
//绘制bird
void MainWindow::DrawBird() {
     QPainter painter(this);
     QPixmap bird = bird1;
     if(gameStatus == GameStatus::RUNNING) {
         //绘制哪一个 (动画效果)
         if(birdNo < birdNoMax/3) bird = bird1;
         else if(birdNo < birdNoMax/3 * 2) bird = bird2;
         else bird = bird3;
         birdNo++;
         if(birdNo > birdNoMax) birdNo = 1;
     }
     // 小鸟下降
     if(birdStatus == BirdStatus::DOWN) {
         birdY += birdDownSpeed;
         birdDownSpeed += 0.2;
     }
    // 判断是否碰撞
     if(isCrush()) {
        //QSound::play(DIEPATH);
        StopGame();
     }
    // 上升
     if(birdStatus == BirdStatus::UP) {
         birdY -= birdUpSpeed;
         birdUpSpeed -= 0.25;
         if(birdUpSpeed <= 0.0){
             birdStatus = BirdStatus::DOWN;
             initSpeed();
         }
     }
     painter.drawPixmap(birdX, birdY, bird);
}
//绘制管道
void MainWindow::DrawPipe() {
    QPainter painter(this);
    uint seed_x = static_cast<uint>(clock());
    int l = Globalutils::getRandom(seed_x,200); //获取随机数
    int otherX = 0; //另一个通道的位置， 只有两个管道
    for(int i = 0; i < pipeChannels->size(); i++) {
        PipeChannel * pipeChannel = (*pipeChannels)[i];
        if(i == 0) otherX = (*pipeChannels)[1]->getX();
        else otherX = (*pipeChannels)[0]->getX();
        pipeChannel->draw(painter);
        int x = pipeChannel->getX();
        if(x + pipeChannel->getPipeWidth() < birdX && !pipeChannel->isScore) {
            //QSound::play(POINTPATH);
            this->score++; //增加得分
            scoreLabel->setText(QString::number(this->score));
            pipeChannel->isScore = true;
            flag = 0;
        }
        x -= moveSpeed; // 改变坐标移动起来
        if(x <= -pipeChannel->getPipeWidth()) {// 移出窗口了
            x = otherX + width() / 2 +  2 * l; //重新设置位置
            pipeChannel->setH1(50+l); //重新设置高度
            pipeChannel->initPipe();//重新初始化
        }
        pipeChannel->setX(x);
    }
    if (!flag && score % 10 == 0) {
        int res = score / 10;
        if (res % 2 == 0) background = QPixmap(":/myimage/icon/backgroundday.png");
        else background = QPixmap(":/myimage/icon/backgroundnight.png");
        background = background.scaled(this->width(), this->height() - ground.height()); //缩放
        flag = 1;
    }
}
//碰撞检测
bool MainWindow::isCrush() {
    if (birdY >= this->height() - ground.height() - bird1.height()) { //是否碰撞地面
        return true;
    }
    if(birdY <= 0)return true;//是否碰撞天空
    //是否碰撞管道
    for(PipeChannel* c : *pipeChannels) {
        if (birdX + bird1.width() > c->getX() && birdX < c->getX() + c->getPipeWidth() && (birdY < c->getPiPe2Height() || birdY+bird1.height() > c->getY1())){
            return true;
        }
    }
    return false;
}
//循环绘制
void MainWindow::LoopPaint() {
    update();//事实画面更新
}
//循环游戏
void MainWindow:: Start() {
    StartGame();
}
