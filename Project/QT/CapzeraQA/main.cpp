#include "mainwindow.h"
#include <QApplication>
#include <QCoreApplication>
#include <QWebEngineProfile>
#include <QFontDatabase>

void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    if (msg.contains("has no notify signal") || msg.contains("value updates in HTML will be broken") || msg.contains("Deprecated Setup Option") || msg.contains("Unknown property box-shadow") || msg.contains("qtwebengine") || msg.contains("Md2")) {
        return;
    }
    QByteArray localMsg = msg.toLocal8Bit();
    fprintf(stderr, "%s\n", localMsg.constData());
}

int main(int argc, char *argv[]) {
    qInstallMessageHandler(customMessageHandler);
    //qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--disable-gpu");
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    QApplication a(argc, argv);
    int fontId65 = QFontDatabase::addApplicationFont(":/fonts/HYWenHei65W.ttf");
    int fontId85 = QFontDatabase::addApplicationFont(":/fonts/HYWenHei85W.ttf");

    if (fontId65 != -1) {
        // 获取字体真实的 Family Name，并设置为全软件默认字体
        QString fontFamily = QFontDatabase::applicationFontFamilies(fontId65).at(0);
        QFont globalFont(fontFamily, 10);
        a.setFont(globalFont);
    }

    a.setStyleSheet("QWidget:focus, QPushButton:focus { outline: none; }");
    QString globalQss =
        "QWidget:focus, QPushButton:focus, QRadioButton:focus, QCheckBox:focus { outline: none; }\n"
        "QScrollBar:vertical {"
        "    border: none; background: transparent; width: 10px; margin: 0px;"
        "}\n"
        "QScrollBar::handle:vertical {"
        "    background: #CBD5E1; min-height: 40px; border-radius: 5px; margin: 0px;"
        "}\n"
        "QScrollBar::handle:vertical:hover { background: #94A3B8; }\n"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }\n"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }\n"
        "QScrollBar:horizontal {"
        "    border: none; background: transparent; height: 10px; margin: 0px;"
        "}\n"
        "QScrollBar::handle:horizontal {"
        "    background: #CBD5E1; min-width: 40px; border-radius: 5px; margin: 0px;"
        "}\n"
        "QScrollBar::handle:horizontal:hover { background: #94A3B8; }\n"
        "QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal { width: 0px; }\n"
        "QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal { background: none; }";

    a.setStyleSheet(globalQss);

    MainWindow w;
    w.show();
    return a.exec();
}