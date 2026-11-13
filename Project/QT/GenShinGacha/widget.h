#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLineEdit> //单行文本框
#include <QPushButton> //按钮
#include <QGridLayout> //控件矩阵布局工具
#include <QVector> //数组
#include <QString> //字符串
#include <QDebug>  //调试
#include <QLabel>  //标签
#include <QComboBox> //下拉选项框
#include <QCheckBox> //复选框
#include <QPlainTextEdit> //多行文本框
#include <QMap> //哈希表
#include <QDateTime> //日期时间
#include <QTime> //时刻
#include <QRandomGenerator> //随机种子
#include <QFile> //文件类
#include <QFileInfo> //文件夹类
#include <QDir> //文件夹类

class Widget : public QWidget {
    Q_OBJECT
public slots:
    void Init();//初始化
    void Solve();//计算
    void PresetChange(int);//预设
public:
    Widget(QWidget *parent = 0);
    QVector<QString> role5, role4, weapon4, weapon3, preset_name;
    QVector<QVector<int>> preset;
    QVector<int> fate_count;
    int pad4, pad5, total, curIn1 = 12, curIn2 = 23, curIn3 = 24, curIn4 = 25, times, ga;
    int total3, total4, total5;
    bool guarantee4, guarantee5, initok;
    QPushButton *btn1, *btn2;
    QLineEdit *le1, *le2, *le3;
    QLabel *lb1, *lb2, *lb3, *lb4, *lb5, *lb6, *lb7, *lb8;
    QComboBox *cb1, *cb2, *cb3, *cb4, *cb5;
    QCheckBox *check1, *check2;
    QPlainTextEdit *result;
    QGridLayout *gl1;
    QMap<QString, int> um;
    QRandomGenerator *rng;
    QString timeStr, fileName;
    void init_data();
    void SSR(QVector<QString>&);
    void SR(QVector<QString>&);
    void R(QVector<QString>&);
    void make_file();
    void Record(int, int, bool, bool);
    QString to_string(int);
    QVector<QString> Gacha();
    ~Widget();
};

#endif // WIDGET_H
