#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QDebug>
#include <QPushButton>
#include <QPen>
#include <QVector>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <bits/stdc++.h>
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void QuitGame();
    void SelectLevel();
    void Menu();//菜单
public:
    explicit MainWindow(QWidget *parent = 0);
    void closeButton(QPushButton*);
    void openButton(QPushButton*);
    void init();
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QPushButton *start, *option, *end, *btm;
    QLabel *win, *select;
    QVector<QPushButton*> buttons;
};

#endif // MAINWINDOW_H
