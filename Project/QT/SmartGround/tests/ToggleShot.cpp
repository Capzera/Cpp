#include <QApplication>
#include <QImage>
#include <QPainter>
#include <cstdio>
#include "ui/widgets/MyToggleSwitch.h"
#include "ui/widgets/MyTextEdit.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    MyToggleSwitch off;
    off.setOnText("无向图");
    off.setOffText("有向图");
    off.setChecked(false);
    off.setFixedSize(off.sizeHint());
    off.show();
    QApplication::processEvents();

    MyToggleSwitch on;
    on.setOnText("无向图");
    on.setOffText("有向图");
    on.setChecked(true);
    on.setFixedSize(on.sizeHint());
    on.show();
    QApplication::processEvents();

    QImage img(320, 60, QImage::Format_ARGB32);
    img.fill(QColor(251, 252, 254));
    QPainter painter(&img);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(10, 15);
    off.render(&painter);
    painter.resetTransform();
    painter.translate(170, 15);
    on.render(&painter);
    img.save("toggle_states.png");

    MyTextEdit edit;
    edit.setFixedSize(300, 120);
    edit.setPlaceholderText("第一行输入两个正整数，表示顶点数和边数\n接下来 m 行，每行两个整数 u, v, 表示一条边");
    edit.show();
    QApplication::processEvents();
    QImage editImg(300, 120, QImage::Format_ARGB32);
    editImg.fill(QColor(251, 252, 254));
    QPainter p2(&editImg);
    edit.render(&p2);
    editImg.save("textedit_placeholder.png");

    printf("saved\n");
    return 0;
}
