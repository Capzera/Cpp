/*
    *入门第2讲
    *定义10个苹果并吃掉6个半个
*/

#include <iostream>

using namespace std;

int main() {
    int sumApple = 10;
    double eatApple = 6.5;
    double resApple = 0;

    resApple = sumApple - eatApple;

    cout << "I have " << resApple << " Apple(s)" << endl;
    system("pause");
    return 0;
}