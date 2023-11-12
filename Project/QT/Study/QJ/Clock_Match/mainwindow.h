#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPen>
#include <QPainter>
#include <QTime>
#include <QString>
#include <QDebug>

using namespace std;

namespace Ui {class MainWindow;}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);
    void drawHexH(QPainter&, int, int, int, int);
    void drawHexV(QPainter&, int, int, int, int);
    ~MainWindow();
private:
    int hh = 0, mm = 0, ss = 0;
    int timer;
    int BC = 40, BN = 4, a = BC * BN, b = BC;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
