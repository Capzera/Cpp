#include <bits/stdc++.h> //头文件 —— 为程序内需要调用的函数做铺垫
using namespace std;
int main() {
    int year;
    cin >> year;
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
        cout << "Lunar";
    }
    else {
        cout << "Solar";
    }
    return 0;
}
/*
 键盘输入3个数 a, b, c，请输出最大的那个数
 5 9 1

 9
 键盘输入一个年份（整数），判断它是否是闰年，如果是闰年，输出Lunar，如果是平年输出Solar
 2024 Lunar
 2022 Solar
 2021 Solar
 1900 Solar
 2000 Lunar
最基本的执行单位：语句
语句体 分号
函数(Function) 功能
1 + 1  = 2


数据类型 (Data Class) {
    byte  比特型 0-63
    short 短型  -128 ~ 127
    *int*   整型（Integer）  -2147483648 ~ 2147483647
    long long 超长整形    -9223372036854775808 ~ 9223372036854775807

    float  单浮点型    深入小数点后6位
    double 双浮点型    深入小数点后20位

    char   (Character) 字符型
    string 字符串

    bool(boolean) true(1) false(0)
    3.141592
变量定义(Variable)
数据类型 [空格] 变量名称;

赋值
变量名称 = 值;
初值 (first value)
3 5

变量名
只允许包含：数字、字母、下划线
首位：必须是字母

int account;

从键盘输入两个整数，交换两个整数的值输出；

缩进级
每4个空格形成一个缩进级，空格越多代表缩进级越高

条件语句
语句a;
语句b;
if (条件成立) {
    语句1;
    语句2;
    语句3;
}

否则语法
if (条件成立) {
    语句1;
    语句2;
}
else {
    语句3;
    语句4;
}

语句c;

逻辑判断符号
> 大于号
< 小于号
>= 大于等于号
<= 小于等于号
== 等于号
&& 且
|| 或
!  非

运算符号
+ 加号
- 减号
* 乘号
/ 整除号
= 赋值号
% 取模号
+= 自加
-= 自减
*= 自乘
/= 自除
%= 自取模
}
*/
