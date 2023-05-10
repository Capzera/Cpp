#include <bits/stdc++.h>
using namespace std;
int main() {
    int x, y, k, b;
    cin >> x >> y >> k >> b;
    int  n = log(y) / log(b);
    vector<vector<int>> c(n + 1, vector<int>(n + 1));
    for (int i = 0; i <= n; i++) {
        c[i][0] = 1;
        for (int j = 1; j <= i; j++)
            c[i][j] = c[i - 1][j] + c[i - 1][j - 1];
    }
    auto solve = [&](int num) -> int {
        if (!num) return 0;
        int last = 0, ans = 0;
        vector<int> nums;
        while (num) {
            nums.emplace(num % b);
            num /= b;
        }
        for (int i = nums.size() - 1; i >= 0; i--) {
            if (nums[i]) {
                ans += c[i][k - last];
                if (nums[i] > 1) {
                    if (k - last - 1 >= 0) ans += c[i][k - last - 1];
                    break;
                }
                else if (++last > k) break;
            }
            if (!i && k == last) ans++;
        }
        return ans;
    };
    cout << solve(y) - solve(x - 1) << endl;
    system("pause");
    return 0;
}