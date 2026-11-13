#include <QCoreApplication>
#include "MyServer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv); // 控制台应用的核心循环，不带GUI

    MyServer server;
    if (!server.startServer(8888)) {
        return -1;
    }

    return a.exec(); // 进入QT事件循环
}
