#include <iostream>
using namespace std;
int main() {
    double money = 1000.15;
    double costMoney = 540.5;
    double resMoney = 0;
    
    resMoney = money - costMoney;

    cout << "I have " << resMoney << " yuan" << endl;
    system("pause");
    return 0;
}