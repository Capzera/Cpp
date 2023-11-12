#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, k, mn = 10000, cost, sum;
    long long ans = 0;
    cin >> n >> k;
    while (n--) {
        cin >> cost >> sum;
        mn = min(mn + k, cost);
        ans += 1LL * mn * sum;
    }
    cout << ans << endl;
    system("pause");
    return 0;
}