#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define MAIN_VERSION 1
#define SECOND_VERSION 1
#define LAST_VERSION 0


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
#include <QListWidget>
#include <QListWidgetItem>
#include "mtextedit.h"

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

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots :
    void ranTriggle();                  //随机按钮触发槽函数
    void optTriggle();                  //设置按钮触发槽函数
    void quiTriggle();                  //退出按钮触发槽函数
    void addTriggle();                  //添加按钮触发槽函数
    void delTriggle();                  //删除按钮触发槽函数
    void backTriggle();                 //返回按钮触发槽函数
public:
    explicit MainWindow(QWidget *parent = 0);
    void Init();                        // 初始化控件
    void MainView();                    // 主界面视图
    void OptionView();                  // 设置视图
    void SetButtonOn(MyButton*);        // 设置按钮可见
    void SetButtonOff(MyButton*);       // 设置按钮不可见
    void SetTextOn(QPlainTextEdit*);    // 设置文本框可见
    void SetTextOff(QPlainTextEdit*);   // 设置文本框不可见
    void SetTextOn(MTextEdit*);    // 设置文本框可见
    void SetTextOff(MTextEdit*);   // 设置文本框不可见
    void SetLabelOn(QLabel*);           // 设置标签可见
    void SetLabelOff(QLabel*);          // 设置标签不可见
    void SetTableOn(QListWidget*);     // 设置表格可见
    void SetTableOff(QListWidget*);    // 设置表格不可见
    void reFresh();                     // 刷新表格
    void Reset();                       // 菜品文件清空/建立
    void Read();                        // 菜品文件读入
    void Save();                        // 菜品文件保存
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QListWidget *tv;
    MyButton *ran, *opt, *qui, *back, *add, *del, *qui2;
    QPlainTextEdit *type;
    MTextEdit *ans;
    QLabel *title;
    QVector<QString> menu;
    int MAX_MENU, lst1, lst2;
    double dpi;
    QString VersionNumber = "V" + QString::number(MAIN_VERSION) + "." + QString::number(SECOND_VERSION) + "." + QString::number(LAST_VERSION);
};

#endif // MAINWINDOW_H
