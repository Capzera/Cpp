/*
    *入门第2讲
    *定义10个苹果并吃掉6个
*/

#include <iostream>

using namespace std;

int main() {
    int sumApple = 10;
    int eatApple = 6;
    int resApple = 0;

    resApple = sumApple - eatApple;

    cout << "I have " << resApple << " Apple(s)" << endl;
    system("pause");
    return 0;
}