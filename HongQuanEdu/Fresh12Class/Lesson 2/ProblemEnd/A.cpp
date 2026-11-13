/*
    *入门第2讲
    *每位同学有32323个苹果，共9323位同学，问一共有多少个苹果
*/

#include <iostream>

using namespace std;

int main() {
    
    int studentApple = 32323;
    int studentCount = 9323;
    int sumApples = 0;

    sumApples = studentApple * studentCount;
    
    cout << "sum = " << sumApples << endl;
    system("pause");
    return 0;
}