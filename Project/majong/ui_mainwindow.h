/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLineEdit *le1;
    QLabel *lb1;
    QPushButton *btn1;
    QPlainTextEdit *pte1;
    QLabel *lb2;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(566, 502);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        le1 = new QLineEdit(centralWidget);
        le1->setObjectName(QStringLiteral("le1"));
        le1->setGeometry(QRect(140, 120, 291, 51));
        QFont font;
        font.setFamily(QStringLiteral("Consolas"));
        font.setPointSize(14);
        le1->setFont(font);
        lb1 = new QLabel(centralWidget);
        lb1->setObjectName(QStringLiteral("lb1"));
        lb1->setGeometry(QRect(10, 130, 101, 31));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\347\255\211\347\272\277"));
        font1.setPointSize(14);
        lb1->setFont(font1);
        btn1 = new QPushButton(centralWidget);
        btn1->setObjectName(QStringLiteral("btn1"));
        btn1->setGeometry(QRect(240, 220, 91, 41));
        QFont font2;
        font2.setFamily(QString::fromUtf8("\347\255\211\347\272\277"));
        font2.setPointSize(18);
        btn1->setFont(font2);
        pte1 = new QPlainTextEdit(centralWidget);
        pte1->setObjectName(QStringLiteral("pte1"));
        pte1->setGeometry(QRect(140, 320, 291, 41));
        pte1->setFont(font);
        lb2 = new QLabel(centralWidget);
        lb2->setObjectName(QStringLiteral("lb2"));
        lb2->setGeometry(QRect(19, 324, 90, 31));
        lb2->setFont(font1);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 566, 23));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        lb1->setText(QApplication::translate("MainWindow", "\350\257\267\350\276\223\345\205\245\346\211\213\347\211\214", nullptr));
        btn1->setText(QApplication::translate("MainWindow", "\350\256\241\347\256\227", nullptr));
        lb2->setText(QApplication::translate("MainWindow", "\345\220\254\347\211\214\347\273\223\346\236\234", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
