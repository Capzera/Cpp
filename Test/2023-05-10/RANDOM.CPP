#include<bits/stdc++.h>
using namespace std;
int main() {
    srand(time(NULL));
    vector<int> ran(8);
    for (int i = 0; i < 8; i++) {
        ran[i] = rand() % 6 + 1;
    }
    for (int i = 0; i < 8; i++) {
        cout << "第" << i + 1 << "个骰子的点数为：" << ran[i] << endl;
        system("pause");
    }
    system("pause");
    return 0;
}
/*
丢8次骰子，质数的个数是质数个：不买
质数的个数是合数个 买
*/