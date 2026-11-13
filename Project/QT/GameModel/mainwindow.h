#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define MAX_LEVEL 75
#define MAIN_VERSION 1
#define SECOND_VERSION 0
#define LAST_VERSION 0

#include <QMainWindow>
#include <QDebug>
#include <QPushButton>
#include <QVector>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QEventLoop>
#include <QTimer>
#include <QScreen>
#include <QStack>
#include <QPlainTextEdit>
#include <QPalette>

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
        this->setStyleSheet("QPushButton {background-color: green;font: bold 28px;border-width: 2px;color: white;}");
        state = 1;
    }
    void changeToBlueRect() {  //设置蓝色矩形按钮
        this->setStyleSheet("QPushButton {background-color: blue;font: bold 28px;border-width: 2px;color: white;}");
        state = 1;
    }
    void changeToOrangeRect() {//设置橙色矩形按钮
        this->setStyleSheet("QPushButton {background-color: orange;font: bold 28px;border-width: 2px;color: white;}");
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
    void QuitGame();        //退出游戏
    void SelectLevel();     //关卡选择
    void Menu();            //菜单
    void GameStart();       //游戏开始准备阶段
    void PageMinus();       //向上翻页
    void PagePlus();        //向下翻页
    void Retry();           //重新挑战
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
    QString GameName = "UnTitled";
    double dpi; //获取屏幕的缩放比例
    QString VersionNumber = "V" + QString::number(MAIN_VERSION) + "." + QString::number(SECOND_VERSION) + "." + QString::number(LAST_VERSION);
    Ui::MainWindow *ui;
    int NowLevel;       //记录当前的关卡号
    int Page;           //记录当前的页号
    int Step;           //记录当前的步数
    int CurMaxLevel = 0;    //记录当前通过的最大关卡
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
    QString UpdateLog = QString("* 1.0.0  完成了游戏的主体逻辑\n");
};

#endif // MAINWINDOW_H
