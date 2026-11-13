#include <bits/stdc++.h>
using namespace std;
int main() {
    const int N = 100000000;
    clock_t st, ed;
    st = clock();
    vector<int> n1;
    for (int i = 0; i < N; i++) {
        n1.emplace_back(N);
    }
    ed = clock();
    cout << "使用emplace_back的方式时间为：" <<(double) ed - st << " ms" << endl;
    st = clock();
    vector<int> n2;
    for (int i = 0; i < N; i++) {
        n2.push_back(N);
    }
    ed = clock();
    cout << "使用push_back的方式时间为：" << (double) ed - st << " ms" << endl;
    system("pause");
    return 0;
}