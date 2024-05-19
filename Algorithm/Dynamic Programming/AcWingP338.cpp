#include <bits/stdc++.h>
using namespace std;
vector<int> solve(int n) {
    vector<int> nums, last(10), ans(10), base(10);
    if (!n) return ans;
    while (n) nums.emplace_back(n % 10), n /= 10;
    int m = nums.size();
    vector<vector<int>> f(m + 1, vector<int>(10)), g(m + 1, vector<int>(10));
    base[0] = 1;
    for (int i = 1; i < 10; i++) base[i] = base[i - 1] * 10;
    for (int i = 0; i < 10; i++) f[1][i] = 1;
    for (int i = 2; i <= m; i++)
        for (int j = 0; j < 10; j++)
            f[i][j] = f[i - 1][j] * 10 + base[i - 1];
    for (int i = 1; i < 10; i++) g[1][i] = 1;
    for (int i = 2; i <= m; i++) {
        g[i][0] = g[i - 1][0] + 9 * f[i - 1][0];
        for (int j = 1; j < 10; j++)
            g[i][j] = g[i - 1][j] + 9 * f[i - 1][j] + base[i - 1];
    }
    for (int i = 0; i < 10; i++) ans[i] = g[m - 1][i];

    for (int i = m - 1; i >= 0; --i) {
        int x = nums[i];
        for (int j = (i == m - 1); j < x; j++) {
            ans[j] += base[i];
            for (int k = 0; k < 10; k++) {
                ans[k] += base[i] * last[k];
                ans[k] += f[i][k];
            }
        }
        last[x]++;
    }
    for (int k = 0; k < 10; k++) ans[k] += last[k];
    return ans;
}
int main() {
    int a, b;
    while (cin >> a >> b, a | b) {
        if (a > b) swap(a, b);
        vector<int> x = solve(b), y = solve(a - 1);
        for (int i = 0; i < 10; i++) cout << x[i] - y[i] << " ";
        cout << endl;
    }
    system("pause");
    return 0;
}