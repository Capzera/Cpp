#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define MAIN_VERSION 1
#define SECOND_VERSION 1
#define LAST_VERSION 1


#include <QMainWindow>
#include <QApplication>
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
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MyButton : public QPushButton {
public:
    MyButton();
    MyButton(const QString& str, QWidget *parent):QPushButton(str, parent) {

    }
    MyButton(QWidget *parent) : QPushButton(parent) {}
    void changeToGreyRect() {   //设置灰色矩形按钮
        this->setStyleSheet("QPushButton {"
                            "background-color: grey;"
                            "font: bold 28px;"
                            "border-width: 2px;"
                            "color: white;"
                            "}"
                            "QPushButton:hover {"
                            "background-color: #d3d3d3;"
                            "}"
                            "QPushButton:disabled {"
                            "background-color: grey;"
                            "color: white;"
                            "}");
        this->repaint();
    }
    void changeToGreenRect() {  //设置绿色矩形按钮
        this->setStyleSheet("QPushButton {"
                            "background-color: green;"
                            "font: bold 28px;"
                            "border-width: 2px;"
                            "color: white;"
                            "}"
                            "QPushButton:hover {"
                            "background-color: #00cc00;"
                            "}"
                            "QPushButton:disabled {"
                            "background-color: green;"
                            "color: white;"
                            "}");
        this->repaint();
    }
    void changeToRedRect() {//设置红色矩形按钮
        this->setStyleSheet("QPushButton {"
                            "background-color: red;"
                            "font: bold 28px;"
                            "border-width: 2px;"
                            "color: white;"
                            "}"
                            "QPushButton:hover {"
                            "background-color: #ff3333;"
                            "}"
                            "QPushButton:disabled {"
                            "background-color: red;"
                            "color: white;"
                            "}");
        this->repaint();
    }
    void setButtonState(bool enabled, const QString& color = "") {
        this->setEnabled(enabled);
        if (!color.isEmpty()) {
            if (color == "green") changeToGreenRect();
            else if (color == "red") changeToRedRect();
            else if (color == "grey") changeToGreyRect();
        }

        if (!enabled) {
            QEvent leaveEvent(QEvent::Leave);
            QApplication::sendEvent(this, &leaveEvent);
            QTimer::singleShot(0, this, [this]() {
                this->repaint();
            });
        }
    }
};


class MainWindow : public QMainWindow {
    Q_OBJECT
public slots:
    void ansTriggle();
    void nxtTriggle();
    void startTriggle();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void Init(); // 初始化控件
    void lbOff(QLabel *);
    void lbOn(QLabel *);
    void btnOff(MyButton *);
    void btnOn(MyButton *);
    void teOff(MTextEdit *);
    void teOn(MTextEdit *);
    void OyaRong(); // 亲家荣和
    void OyaTsumo(); // 亲家自摸
    void KoRong(); // 子家荣和
    void KoTsumo(); // 子家自摸
    void setQuiz(); // 出题
    void Accepted(); // 正确
    void WrongAnswer(); // 错误
    void Hajime(); // 开始
    int hyakuBai(int);
    int testLab = 1;
    Ui::MainWindow *ui;
    double dpi;
    QVector<int> banSet = {1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 5, 5, 6, 7, 8, 9, 10, 11, 12, 13};
    QVector<int> fuSet = {20, 25, 30, 40, 50, 60, 70, 80, 90, 100, 110};
    int ban, fu, ply, tm;
    QLabel *lbply[2], *lbpay[3], *lbtm[2], *lbpt, *lbstick, *lbban, *lbfu; // 静态 标签
    MTextEdit *teban, *tefu, *tepay1, *tepay2, *tepay3; // 文本框
    MyButton *btnans, *btnnxt, *btnhjm; // 按钮
    QString VersionNumber = "V" + QString::number(MAIN_VERSION) + "." + QString::number(SECOND_VERSION) + "." + QString::number(LAST_VERSION);
};
#endif // MAINWINDOW_H
