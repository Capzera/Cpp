#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QVector>
#include <QKeyEvent>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include "pipe.h"
#include "bestscore.h"
#include "globalutils.h"
#include <qnamespace.h>
#include <QSize>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent*);//画图事件
    void keyPressEvent(QKeyEvent*);//键盘事件监听
    void mousePressEvent(QMouseEvent*);//鼠标事件监听
    void init();//初始化
    void initGame();//游戏初始化
    void initSpeed();//初始化速度
    void StartGame();//开始游戏
    void StopGame();//游戏结束
    void DrawTitle();
    void DrawBack();//画背景
    void DrawBird();//画鸟
    void DrawPipe();//画管道
    bool isCrush();//是否碰撞
    QPixmap bird1, bird2, bird3;
    bool flag = 0;
public slots:
    void LoopPaint();//循环管道槽
    void Start();//游戏开始槽
private:
    QPixmap background, ground, title;
    int birdNo = 1;
    int birdNoMax = 15;
    int birdY = 0; // 小鸟的位置Y坐标
    int birdX = 0; // 小鸟的位置X坐标
    QMatrix matrix;
    enum GameStatus{STOPING = 1, RUNNING}; //游戏状态枚举
    enum BirdStatus{DEFAULT, UP, DOWN}; //小鸟飞行状态枚举
    GameStatus gameStatus = GameStatus::STOPING; //游戏状态 默认停止
    BirdStatus birdStatus = BirdStatus::DOWN;
    double birdDownSpeed = 0;
    double birdUpSpeed = 2.0;
    QTimer *timer = nullptr;
    QVector<int> * v = nullptr;//地面
    int groundSize = 0; //地面个数
    int moveSpeed = 3; //移动速度
    QVector<PipeChannel*> *pipeChannels = nullptr;//管道
    int score = 0;
    BestScore best;
    QLabel* scoreLabel  = nullptr; //记录分数
    QLabel* scoreLabel1  = nullptr; //记录最高分数
    QPushButton* startButton = nullptr;
};

#endif // MAINWINDOW_H
