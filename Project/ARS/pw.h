#ifndef _PW_H_
#define _PW_H_

#include "common.h"
#include "passenger.h"
#include "flight.h"

extern vector<passenger> pass;
extern vector<administrator> admin;
extern vector<flight> F;

void init();

void flinfoin();//航班信息读入

int verification(int);//密码验证

string passwordinput();//密码输入

void paregist();//用户注册

void painfoin();//用户信息录入

int idlogin(int&);//身份证登陆

int tellogin(int&);//电话号码登陆

void adlogin(int&);//管理员登陆

bool checkid(string);//检验身份证号

bool pwdcheck(string);//检验密码

bool pwdcheck(string,string);//检验确认密码
#endif //pw.h
