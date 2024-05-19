/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QGridLayout *gridLayout;
    QTextEdit *textEdit;
    QHBoxLayout *horizontalLayout;
    QPushButton *but_mouseI;
    QPushButton *but_mouser;
    QSpacerItem *horizontalSpacer;
    QPushButton *but_keyI;
    QPushButton *but_KeyR;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(742, 530);
        QFont font;
        font.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font.setPointSize(12);
        Widget->setFont(font);
        gridLayout = new QGridLayout(Widget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(1, 1, 1, 3);
        textEdit = new QTextEdit(Widget);
        textEdit->setObjectName(QStringLiteral("textEdit"));

        gridLayout->addWidget(textEdit, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        but_mouseI = new QPushButton(Widget);
        but_mouseI->setObjectName(QStringLiteral("but_mouseI"));

        horizontalLayout->addWidget(but_mouseI);

        but_mouser = new QPushButton(Widget);
        but_mouser->setObjectName(QStringLiteral("but_mouser"));

        horizontalLayout->addWidget(but_mouser);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        but_keyI = new QPushButton(Widget);
        but_keyI->setObjectName(QStringLiteral("but_keyI"));

        horizontalLayout->addWidget(but_keyI);

        but_KeyR = new QPushButton(Widget);
        but_KeyR->setObjectName(QStringLiteral("but_KeyR"));

        horizontalLayout->addWidget(but_KeyR);


        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", nullptr));
        but_mouseI->setText(QApplication::translate("Widget", "\345\256\211\350\243\205\345\205\250\345\261\200\351\274\240\346\240\207\347\233\221\345\220\254\345\231\250", nullptr));
        but_mouser->setText(QApplication::translate("Widget", "\345\215\270\350\275\275\345\205\250\345\261\200\351\274\240\346\240\207\347\233\221\345\220\254\345\231\250", nullptr));
        but_keyI->setText(QApplication::translate("Widget", "\345\256\211\350\243\205\345\205\250\345\261\200\351\224\256\347\233\230\347\233\221\345\220\254\345\231\250", nullptr));
        but_KeyR->setText(QApplication::translate("Widget", "\345\215\270\350\275\275\345\205\250\345\261\200\351\224\256\347\233\230\347\233\221\345\220\254\345\231\250", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
