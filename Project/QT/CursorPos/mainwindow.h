#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QScreen>
#include <QDesktopWidget>

#include <globalmouseevent.h>
#include <QMouseEvent>
#include <QMetaEnum>

#include <QLabel>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    double dpi;//获取屏幕缩放比例
    QLabel *lb1;
    Ui::MainWindow *ui;
private slots:
    void on_mouseEvent(QMouseEvent* event);
};

#endif // MAINWINDOW_H
