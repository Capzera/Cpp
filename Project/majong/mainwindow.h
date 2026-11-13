#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QtGlobal>
#include <bits/stdc++.h>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QVector>
#include <QSet>
#include <QString>
#include <QMap>

namespace Ui {
    class MainWindow;
}

using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void mousePressEvent();
public:
    explicit MainWindow(QWidget *parent = 0);
    void Init();//手牌初始化函数
    void Solve();//解答听牌函数
    QString Decode(int);//解码，把牌数字转化成字符牌
    QVector<int> haveStraight(QVector<int>);
    QVector<int> haveTriple(QVector<int>);
    QVector<int> havePair(QVector<int>);
    void removeStraight(QVector<int>&, int);
    void removeTriple(QVector<int>&, int);
    void removePair(QVector<int>&, int);
    bool CheckHu(QVector<int>);//检查当前牌组是否已经和了
    bool dfs(QVector<int>, int);//dfs进行查牌
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    bool nom, nop, nos, noz;
    QString shoupai;
    QVector<int> pai;
    QMap<int, int> mp;
    QMap<QString, int> tenpai;
    QPushButton *btn1;
    QLabel *lb1, *lb2;
    QLineEdit *le1;
    QPlainTextEdit *pte1;
};
#endif // MAINWINDOW_H
