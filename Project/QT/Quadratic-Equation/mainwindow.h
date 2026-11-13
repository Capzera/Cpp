#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QPushButton>
#include <QVector>
#include <QLineEdit>
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

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public slots:
    void Calculator();
    void Quit();
public:
    explicit MainWindow(QWidget *parent = 0);
    void Init();
    void CreateItem();
    void SetFont();
    void SetGeometry();
    void Connection();
    ~MainWindow();

private:
    double dpi;
    QLabel *eq1, *eq2, *xx1, *xx2, *yy1, *yy2;
    QLineEdit *x1, *x2, *y1, *y2, *c1, *c2, *ans;
    QPushButton *cal;
    QPushButton *qui;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
