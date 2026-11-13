#ifndef COMMONTYPES_H
#define COMMONTYPES_H

#include <QString>
#include <QVector>
#include <QStringList>
#include <QVariant>

// 学生底层核心数据结构
struct StudentInfo {
    QString account;
    QString username;
    QString className;
    QString regTime;
    QString password;
    QString realName;
    QString lastLoginTime;   // 最近一次登录时间
    QString lastSubmitTime;  // 最近一次交卷时间
    QString phone;           // 手机号 (找回密码备用)
    QString email;           // 邮箱 (发送成绩单备用)
    QString idCard;          // 身份证号 (竞赛报名备用)
    QString motto;           // 个人签名/座右铭
    QString gender;          // 性别
};

// 题库底层核心数据结构 (支持多题型)
struct ProblemInfo {
    QString id;
    QString title;
    QString type;
    QString tag;
    QString difficulty;
    QString description;
    QStringList options;
    QVariant standardAnswer;
    QString inputSample;
    QString outputSample;
};

#endif // COMMONTYPES_H
