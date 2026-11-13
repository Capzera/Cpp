#include "MyServer.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

MyServer::MyServer(QObject *parent) : QObject(parent)
{
    tcpServer = new QTcpServer(this);
    connect(tcpServer, &QTcpServer::newConnection, this, &MyServer::onNewConnection);
}

bool MyServer::startServer(quint16 port)
{
    // 监听所有网卡的指定端口
    if (!tcpServer->listen(QHostAddress::Any, port)) {
        qDebug() << "【错误】服务器启动失败: " << tcpServer->errorString();
        return false;
    }
    qDebug() << "【成功】答题器服务端已启动，正在监听端口:" << port;
    return true;
}

void MyServer::onNewConnection()
{
    // 获取连接进来的客户端 Socket
    while (tcpServer->hasPendingConnections()) {
        QTcpSocket *clientSocket = tcpServer->nextPendingConnection();
        clientSockets.append(clientSocket);

        qDebug() << "【通知】新客户端连接成功! IP:" << clientSocket->peerAddress().toString();

        // 绑定数据读取和断开连接的信号
        connect(clientSocket, &QTcpSocket::readyRead, this, &MyServer::onClientReadyRead);
        connect(clientSocket, &QTcpSocket::disconnected, this, &MyServer::onClientDisconnected);
    }
}

void MyServer::onClientReadyRead()
{
    // 谁发的消息，sender() 就指向谁
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    while (clientSocket->canReadLine()) {
        QByteArray data = clientSocket->readLine().trimmed();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isNull() || !doc.isObject()) continue;

        QJsonObject json = doc.object();
        QString type = json["type"].toString();
        qDebug() << "【收到数据】类型:" << type << " 内容:" << data;

        // 【Ping通测试逻辑】：如果是登录请求，不管三七二十一，直接返回成功！
        if (type == "LOGIN") {
            QString username = json["username"].toString();

            QJsonObject response;
            response["type"] = "LOGIN_RESPONSE";
            response["success"] = true;
            response["message"] = QString("欢迎 %1, 本地与腾讯云服务器成功握手!").arg(username);

            QJsonDocument resDoc(response);
            clientSocket->write(resDoc.toJson(QJsonDocument::Compact) + "\n");
            qDebug() << "【发送响应】已向客户端发送握手成功信号。";
        }
    }
}

void MyServer::onClientDisconnected()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    qDebug() << "【通知】客户端断开连接: " << clientSocket->peerAddress().toString();
    clientSockets.removeAll(clientSocket);
    clientSocket->deleteLater();
}
