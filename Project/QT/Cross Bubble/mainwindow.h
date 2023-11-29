#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define MAX_LEVEL 75
#define MAIN_VERSION 1
#define SECOND_VERSION 10
#define LAST_VERSION 1

#include <QMainWindow>
#include <QDebug>
#include <QPushButton>
#include <QVector>
#include <QLabel>
#include <bits/stdc++.h>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QEventLoop>
#include <QTimer>
#include <QScreen>
#include <QStack>
#include <QPlainTextEdit>

using namespace std;
namespace Ui {class MainWindow;}

class MyButton : public QPushButton {
public:
    MyButton();
    MyButton(QWidget *parent, int num, int st) :QPushButton(parent){
        state = st;
        number = num;
    }
    MyButton(const QString& str, QWidget *parent):QPushButton(str, parent) {

    }
    MyButton(QWidget *parent) : QPushButton(parent) {}
    void change() {state ^= 1;}  //切换按钮的内核状态
    void changeToGreenCircle() { //设置绿色圆型按钮
        this->setStyleSheet("QPushButton {background-color: green;font: bold 20px;border-width: 2px;border-radius: 60px;color: green;}");
        state = 1;
    }
    void changeToBlackCircle() { //设置黑色圆型按钮
        this->setStyleSheet("QPushButton {background-color: black;font: bold 20px;border-width: 2px;border-radius: 60px;color: black;}");
        state = 0;
    }

    void changeToBlackRect() {  //设置黑色矩形按钮
        this->setStyleSheet("QPushButton {background-color: black;font: bold 20px;border-width: 2px;color: white;}");
        state = 0;
    }
    void changeToWhiteRect() {  //设置白色矩形按钮
        this->setStyleSheet("QPushButton {background-color: white;font: bold 28px;border-width: 2px;color: black;}");
    }
    void changeToGreyRect() {   //设置灰色矩形按钮
        this->setStyleSheet("QPushButton {background-color: grey;font: bold 28px;border-width: 2px;color: white;}");
        state = 0;
    }
    void changeToGreenRect() {  //设置绿色矩形按钮
        this->setStyleSheet("QPushButton {background-color: Green;font: bold 28px;border-width: 2px;color: white;}");
        state = 1;
    }
    int getNum() {return number;}   //获取按钮编号
    int getState() {return state;}  //获取按钮状态
private:
    int state;
    int number;
};

class MainWindow : public QMainWindow {
    Q_OBJECT
public slots:
    void pushed();          //游戏内点击
    void QuitGame();        //退出游戏
    void SelectLevel();     //关卡选择
    void Menu();            //菜单
    void GameStart();       //游戏开始准备阶段
    void PageMinus();       //向上翻页
    void PagePlus();        //向下翻页
    void Retry();           //重新挑战
    void Withdraw();        //撤回
    void Help();            //帮助文档
    void LookUpdate();      //查看更新记录
    void NewGame();         //存档清空
public:
    explicit MainWindow(QWidget *parent = 0);
    void operat(int number);            //对number号按钮进行操作
    void change(MyButton*);             //更改按钮状态
    void closeButton(QPushButton*);     //删除按钮
    void closeButton(MyButton*);
    void openButton(QPushButton*);      //启用按钮
    void openButton(MyButton*);
    void openLabel(QLabel*);
    void closeLabel(QLabel*);
    void GameInit(int);                 //游戏关卡初始化
    void GameWin(int);                  //游戏胜利
    void init();                        //初始化控件
    void FileRead();                    //存档读入
    void FileWrite();                   //存档写入
    void FileReset();                   //存档重置
    ~MainWindow();

private:
    double dpi; //获取屏幕的缩放比例
    QString VersionNumber = "V" + QString::number(MAIN_VERSION) + "." + QString::number(SECOND_VERSION) + "." + QString::number(LAST_VERSION);
    Ui::MainWindow *ui;
    int NowLevel;  //记录当前的关卡号
    int Page;      //记录当前的页号
    int Step;      //记录当前的步数
    QVector<QVector<int> > LEVEL;   //记录每关的内容
    QVector<int> BestStep; //记录每关的最佳步数
    QVector<int> MyStep;   //记录玩家每关的最佳步数，联系文件读取
    QVector<MyButton*> buttons; //核心按钮，游戏内点击按钮
    QVector<MyButton*> levels;  //关卡选择按钮
    QPushButton *start; //开始游戏按钮
    QPushButton *option;//帮助文档按钮
    QPushButton *end;   //退出游戏按钮
    QPushButton *newg;  //清除存档按钮
    QPushButton *retry; //Retry重新挑战按钮
    QPushButton *wd;    //WithDraw撤回按钮
    QPushButton *UpDoc; //更新文档按钮
    QPushButton *bts;   //Back to Select 返回关卡选择按钮
    MyButton *btm;      //Back to Menu 返回菜单按钮
    MyButton *bth;      //Back to Help 返回帮助按钮
    MyButton *lastPage; //需变色按钮，上一页翻页按钮
    MyButton *nextPage; //需变色按钮，下一页翻页按钮
    QLabel *title;      //游戏标题——Cross Bubble
    QLabel *version;    //菜单中做下表版本号
    QLabel *curStep;    //显示当前步数
    QLabel *minStep;    //显示最优步数
    QLabel *MybestStep; //显示玩家最佳步数
    QLabel *curLevel;   //显示当前关卡号
    QLabel *helper;     //帮助文档具体内容
    QPlainTextEdit *Log;    //更新文档具体内容
    QStack<int> stepStack;  //保存玩家的操作，以便于撤回
    QString UpdateLog = QString("* 1.0.0  完成了游戏的主体逻辑\n") +
                        QString("* 1.1.0  增加游戏的主界面\n") +
                        QString("* 1.2.0  增加关卡选择模块\n") +
                        QString("* 1.3.0  增加了游戏胜利时弹出对话框的选项\n") +
                        QString("* 1.3.1  修复了一个错误，可能会导致玩家在胜利后点击下一关时游戏闪退\n") +
                        QString("* 1.4.0  增加关卡翻页系统，关卡选择界面新增“上一页”、“下一页”按钮\n") +
                        QString("* 1.4.1  关卡上限增加到50关\n") +
                        QString("* 1.4.2  优化了“上一页”，“下一页”的颜色，新增白色按钮\n") +
                        QString("* 1.4.3  增加了主界面版本号，游戏标题\n") +
                        QString("* 1.4.4  修改了窗口标题\n") +
                        QString("* 1.5.0  增加了游戏内当前关卡，当前步数，最佳步数，历史最佳步数\n") +
                        QString("* 1.5.1  修复了进入下一关时，不会更新关卡号的问题\n") +
                        QString("* 1.5.2  修复了选择关卡时，关卡最佳步数导致游戏闪退的问题\n") +
                        QString("* 1.6.0  增加存档系统，支持本地文件读写，记录玩家信息\n") +
                        QString("* 1.6.1  增加了游戏内退出\n") +
                        QString("* 1.7.0  新增游戏内重来，新增撤回上一步，因为bug，暂时无法实装\n") +
                        QString("* 1.8.0  增加了操作动画，在点击泡泡时渐进操作\n") +
                        QString("* 1.8.1  清除了泡泡中心的测试用数字\n") +
                        QString("* 1.8.2  优化了关卡数字的字体\n") +
                        QString("* 1.8.4  修复了点击31-50关闪退的问题\n") +
                        QString("* 1.8.5  修复了一个问题导致缩放比例为125%时的显示错误\n") +
                        QString("* 1.8.6  修复了一个按钮显示错误的问题\n") +
                        QString("* 1.9.0  实装了上一步功能\n") +
                        QString("* 1.9.1  更新了帮助文档\n") +
                        QString("* 1.9.2  为底层代码增加了注释，调整游戏内按钮的布局\n") +
                        QString("* 1.9.3  新增更新文档按钮和具体更新文档内容\n") +
                        QString("* 1.9.4  更改了选择关卡的字体，关卡上限提高到75关\n") +
                        QString("* 1.9.5  修改了返回按钮的样式，使之与其他按钮看起来更不违和\n") +
                        QString("* 1.10.0 新增清除存档功能，玩家可以从头体验游戏\n") +
                        QString("* 1.10.1 修复了一个Bug，可能导致异常的撤回上一步");
};

#endif // MAINWINDOW_H
