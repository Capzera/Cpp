#include <QFile>
#include <QString>
#include <QCoreApplication>
#include <QDebug>
#include "bestscore.h"

BestScore::BestScore(){
    QFile file("score.txt");
    file.open(QFile::ReadOnly);
    QByteArray ba = file.readAll();
    file.close();
    score = ba.toInt();
}

void BestScore::save() {
    QFile file("score.txt");
    file.open(QFile::WriteOnly | QFile::Text);
    file.write(QString::number(score).toLocal8Bit());
    file.close();
}

void BestScore::read() {
    QFile file("score.txt");
    file.open(QFile::ReadOnly);//文件以只读形式打开
    QByteArray ba = file.readAll();//将文件中的码流以二进制数组读入
    file.close();
    score = ba.toInt();//将文件码流转换为int型的score
}
