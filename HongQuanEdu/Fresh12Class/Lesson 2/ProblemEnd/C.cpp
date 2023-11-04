/*
    *入门第2讲
    *100个苹果分给9个同学，平均分配后，还剩下几个苹果
*/

#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    
    int sumApple = 100;
    int studentCount = 9;
    int eachApple = 0;
    int resApple = 0;

    eachApple = sumApple / studentCount;
    resApple = sumApple - studentCount * eachApple;
    
    cout << "Rest " << fixed << setprecision(2) << resApple << " Apple(s)" << endl;
    system("pause");
    return 0;
}