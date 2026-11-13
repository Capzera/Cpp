#include <bits/stdc++.h>
using namespace std;
int main() {
    int a, b;
    cin >> a >> b;
    auto solve = [&](int n) -> int {
        if (!n) return 0;
        vector<int> nums;
        while (n) {nums.emplace_back(n % 10); n /= 10;}
        n = nums.size();
        vector<vector<int>> f(n + 1, vector<int>(10));
        for (int i = 1; i < 10; i++) f[1][i] = 1;//第一位(个位)填任何数结果都是1个
        for (int i = 2; i <= n; i++)
            for (int j = 0; j < 10; j++)
                for (int k = j; k < 10; k++)
                    f[i][j] += f[i - 1][k];
        int ans = 0;
        for (int i = n - 1, last = 0; i >= 0; --i) {
            int x = nums[i];
            for (int j = last; j < x; j++) 
                ans += f[i + 1][j];
            if (x < last) break;
            last = x;
            if (!i) ans++;
        }
        return ans;
    };

    cout << solve(b) - solve(a - 1) << endl;
    system("pause");
    return 0;
}