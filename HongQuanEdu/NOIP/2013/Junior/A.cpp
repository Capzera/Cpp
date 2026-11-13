/*
试计算在区间1 到 nn 的所有整数中，数字 x（0 ≤ x ≤ 9）共出现了多少次？
例如，在 1 到 11 中，即在 1,2,3,4,5,6,7,8,9,10,11中，数字 1 出现了 4 次。
*/
#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, k, ans = 0;
    cin >> n >> k;
    for (int i = 1; i <= n; i++) {
        int num = i;
        while (num) {
            ans += num % 10 == k;
            num /= 10;
        }
    }
    cout << ans << endl;
    system("pause");
    return 0;
}