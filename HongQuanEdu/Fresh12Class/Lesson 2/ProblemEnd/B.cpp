/*
    *入门第2讲
    *100元平均分给7个同学，每位同学分到多少钱
*/

#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    
    double sumMoney = 100;
    double studentCount = 7;
    double resMoney = 0;

    resMoney = sumMoney / studentCount;
    
    cout << "Each student get " << fixed << setprecision(2) << resMoney << " yuan" << endl;
    system("pause");
    return 0;
}