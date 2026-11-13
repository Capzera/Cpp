#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, m, op;
    long long k, plus = 0;
    cin >> n >> m;
    vector<long long> nums(n), sum(n + 1);
    for (int i = 0; i < n; i++) {
        cin >> nums[i];
    }
    sort(nums.begin(), nums.end());
    for (int i = 0; i < n; i++) {
        sum[i + 1] = sum[i] + nums[i];
    }
    while (m--) {
        cin >> op;
        if (op == 1) {
            cin >> k;
            plus += k;
        }
        else {
            int i = lower_bound(nums.begin(), nums.end(), -plus) - nums.begin();
            cout << sum[n] - sum[i] + 1ll * plus * (n - i) << endl;
        }
    }
    system("pause");
    return 0;
}
/*
-7 -5 2 8 12
0 -7 -12 -10 -2 10
*/