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
    ~MainWindow();
private:
    int hh = 0, mm = 0, ss = 0;
    int timer;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
