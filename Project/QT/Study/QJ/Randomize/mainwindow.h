#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QString>
#include <QVBoxLayout>

class MainWindow : public QMainWindow{
    Q_OBJECT
public slots:
    void btnclick() {
        srand(time(nullptr));
        QString str = edit1->text();
        int maxsize = str.toInt(), a = maxsize;
        while (a == maxsize) {
            a = rand() % maxsize;
        }
        str = QString::number(a);
        edit2->setText(str);
    }
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QPushButton *btn;
    QLineEdit *edit1, *edit2;
};

#endif // MAINWINDOW_H
