#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void mstart();
    void mquit();
    void mback();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QPushButton *btn1, *btn2, *btn3;
};
#endif // MAINWINDOW_H
