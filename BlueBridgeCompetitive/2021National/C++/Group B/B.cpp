#include <bits/stdc++.h>
using namespace std;
int main() {
    int n = 20210605, ans = 0;
    vector<int> prime, st(n + 1);
    set<int> mp = {2, 3, 5, 7};
    auto check = [&](int x) -> bool {
        while (x) {
            if (!mp.count(x % 10)) return 0;
            x /= 10;
        }
        return 1;
    };
    for (int i = 2; i <= n; i++) {
        if (!st[i]) {
            prime.emplace_back(i);
            ans += check(i);
        }
        for (int j = 0; j < prime.size() && prime[j] * i <= n; j++) 
            st[prime[j] * i] = 1;
    }
    cout << ans << endl;
    system("pause");
    return 0;
}