#include <QApplication>
#include <QWebEngineView>
#include <QUrl>

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts); // 设置OpenGL
    QApplication app(argc, argv);
    QWebEngineView *p = new QWebEngineView();
    p->setUrl(QUrl("https://www.luogu.com.cn/"));
    p->resize(1920 , 1080);
    p->show();

    return app.exec();
}