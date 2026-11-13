/*
输入两个正整数 x0, y0 ，求出满足下列条件的 P, Q 的个数：
1.P,Q 是正整数。
2.要求P, Q以x0为最大公约数，以y0为最小公倍数。
试求：满足条件的所有可能的P,Q的个数。
*/
#include <bits/stdc++.h>
using namespace std;
int main() {
    long long x, y, ans = 0;
    cin >> x >> y;
    if (x == y) ans--;
    y *= x;
    for (long long i = 1; i <= sqrt(y); i++) {
        if (y % i == 0 && __gcd(i, y / i) == x) ans += 2;
    }
    cout << ans << endl;
    system("pause");
    return 0;
}