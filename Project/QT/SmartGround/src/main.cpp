#include <QApplication>
#include "ui/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setStyle("Fusion");
    QCoreApplication::setOrganizationName("SmartGround");
    QCoreApplication::setApplicationName("SmartGround");

    MainWindow w;
    w.show();

    return a.exec();
}