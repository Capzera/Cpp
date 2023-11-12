/*
已知 Sn = 1 + 1/2 + 1/3 + ... + 1/n。显然对于任意一个整数 k，当 n 足够大的时候，Sn > k，
现给出一个整数 k，要求计算出一个最小的 n，使得 Sn > kS
*/
#include <bits/stdc++.h>
using namespace std;
int main() {
    int k, n = 0;
    cin >> k;
    for (double sum = 0; sum <= k; n++, sum += 1.0 / n);
    cout << n << endl;
    system("pause");
    return 0;
}