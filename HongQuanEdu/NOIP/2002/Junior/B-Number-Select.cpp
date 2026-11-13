/*
已知 n 个整数 x1, x2, ...,xn
，以及 1 个整数 k（k < n）。从 n 个整数中任选 k 个整数相加，可分别得到一系列的和。例如当 n = 4，k = 3，
4 个整数分别为 3, 7, 12, 19 时，可得全部的组合与它们的和为：

3 + 7 + 12 = 22
3 + 7 + 19 = 29
7 + 12 + 19 = 38
3 + 12 + 19 = 34
现在，要求你计算出和为素数共有多少种。
例如上例，只有一种的和为素数：3 + 7 + 19 = 29。
*/
#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, k, ans = 0, sum;
    cin >> n >> k;
    vector<int> nums(n);
    for (auto& x : nums) cin >> x;
    sort(nums.begin(), nums.end());
    auto check = [&] (int x) -> int {
        if (x == 2 || x == 3 || x == 5) return 1;
        if (x % 6 != 1 && x % 6 != 5) return 0;
        for (int i = 5; i * i < x; i += 6) {
            if (x % i == 0 || x % (i + 2) == 0) return 0;
        }
        return 1;
    };
    function<void(int, int, int)> dfs = [&](int cnt, int sum, int last) {
        if (cnt == k) {
            ans += check(sum);
            return;
        }
        for (int i = last; i < n; i++) {
            dfs(cnt + 1, sum + nums[i], i + 1);
        }
    };
    dfs(0, 0, 0);
    cout << ans << endl;
    system("pause");
    return 0;
}