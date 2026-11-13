#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/resources/coc.ico"));
    MainWindow w;
    w.setWindowIcon(QIcon(":/resources/coc.ico"));
    w.show();

    return a.exec();
}
