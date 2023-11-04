/*
    *入门第2讲
    *算数运算符
*/

#include <iostream>

using namespace std;

int main() {
    
    int number = 10;
    int res = 0;
    //等号左边是变量的名字，等号右边是变量的值
    res = res + number;
    number++;

    cout << "number = " << number << ", res = " << res << endl;

    system("pause");
    return 0;
}