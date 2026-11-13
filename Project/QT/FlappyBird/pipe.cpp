#include "pipe.h"
#include <QCoreApplication>
//winHeight 窗口高度 h1 底下管道高度 x 位置横坐标 groundHeight 地面高度 parent
PipeChannel::PipeChannel(int winHeight,int h1, int x, int groundHeight, QObject *parent) : QObject(parent) {
    this->h1 = h1;
    this->x = x;
    this->groundHeight = groundHeight;
    this->winHeight = winHeight;
    initPipe();
}
//初始化管道
void PipeChannel::initPipe() {
    pipeImage =QImage(":/myimage/icon/pipe.png");
    imageMirror = pipeImage.mirrored(false, true);

    pipe1 = QPixmap::fromImage(pipeImage);
    pipe2 = QPixmap::fromImage(imageMirror);
    int h2 = getPiPe2Height();
    pipe1 = pipe1.copy(0, 0, pipe1.width(), h1);
    pipe1 = pipe1.scaled(pipeWidth, h1);

    pipe2 = pipe2.copy(0, pipe2.height()-h2, pipe2.width(), h2);
    pipe2 = pipe2.scaled(pipeWidth, h2);
    isScore = false;
}
//绘制管道
void PipeChannel::draw(QPainter &painter){
    painter.drawPixmap(x, getY1(), pipe1);//管道1（下面）
    painter.drawPixmap(x, 0, pipe2);//管道2
}
//设置位置 X坐标
void PipeChannel::setX(int x) {
      this->x = x;
}
//getX 获取位置
int PipeChannel::getX() {
    return this->x;
}
//getY1 获取管道1（底部管道）的y坐标
int PipeChannel::getY1() {
    return winHeight - h1 - groundHeight;
}
//获取管道2的高度
int PipeChannel::getPiPe2Height() {
    return winHeight - channelWidth -h1;
}
//获取管道宽度
int PipeChannel::getPipeWidth() {
    return this->pipeWidth;
}
//设置高度
void PipeChannel::setH1(int h1) {
    this->h1 = h1;
}
