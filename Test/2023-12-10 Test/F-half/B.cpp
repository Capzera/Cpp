#include <bits/stdc++.h>
using namespace std;
int main() {
    long long sum = 0, pos = 0, money, ans = n;
    int n;
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> money;
        sum += money;
        if (sum >= 0 && pos) {
            ans += (i - pos) * 2;
            pos = 0;
        }
        if (sum < 0 && !pos) pos = i;
    }
    cout << ans << endl;
    system("pause");
    return 0;
}