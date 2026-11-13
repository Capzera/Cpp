#ifndef MYSERVER_H
#define MYSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>

class MyServer : public QObject
{
    Q_OBJECT
public:
    explicit MyServer(QObject *parent = nullptr);
    bool startServer(quint16 port);

private slots:
    void onNewConnection();
    void onClientReadyRead();
    void onClientDisconnected();

private:
    QTcpServer *tcpServer;
    QList<QTcpSocket*> clientSockets; // 记录当前连接的所有学生/教师端
};

#endif // MYSERVER_H
