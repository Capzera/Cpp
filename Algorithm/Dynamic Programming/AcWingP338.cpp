#include <bits/stdc++.h>
using namespace std;
vector<int> solve(int n) {
    //last统计前置数出现的次数，ans统计每个数的答案，bese是幂次数组
    //base数组为什么开长度为10，因为数字范围不超过1e9，最多10位数，如果本题改成1e18，base也应该设置为19
    vector<int> nums, last(10), ans(10), base(10);
    if (!n) return ans;
    while (n) nums.emplace_back(n % 10), n /= 10;
    int m = nums.size();
    vector<vector<int>> f(m + 1, vector<int>(10)), g(m + 1, vector<int>(10));
    //做幂次数组，该做法取自Rabin-Karp（滚动哈希）算法
    base[0] = 1;
    for (int i = 1; i < 10; i++) base[i] = base[i - 1] * 10;

    //f[i][j] 表示 10^(i - 1) ... 999(i个9)有多少个j，当i取4时表示1000-9999有多少个j
    for (int i = 0; i < 10; i++) f[1][i] = 1;
    for (int i = 2; i <= m; i++)
        for (int j = 0; j < 10; j++)
            //对于1000-9999中。100-999的部分乘10，加"j000"开头的个数
            f[i][j] = f[i - 1][j] * 10 + base[i - 1];

    //g[i][j] 表示从0 ... 999(i个9) 有多少个j
    for (int i = 1; i < 10; i++) g[1][i] = 1;
    for (int i = 2; i <= m; i++) {
        //单独考虑0，因为首位不能填0，所以0-9999中只考虑0100-0999的情况 + （0~999的情况首位补1 - 9）
        g[i][0] = g[i - 1][0] + 9 * f[i - 1][0];
        for (int j = 1; j < 10; j++)
            //对于不是0的情况先考虑 100-999中出现j的个数乘9（首位补1-9），单独考虑首位为0，再加上首位为j时（j000）有1000个j
            g[i][j] = g[i - 1][j] + 9 * f[i - 1][j] + base[i - 1];
    }
    //对于0-9每个数字，设首位为F，所以先考虑F000-F999每个数出现的个数
    for (int i = 0; i < 10; i++) ans[i] = g[m - 1][i];

    for (int i = m - 1; i >= 0; --i) {//填第i位
        int x = nums[i];
        //这里是一个非常好用的操作，如果i == m - 1也就是首位的话，只能从1开始遍历，其他情况逻辑判断为假，从0开始遍历
        for (int j = (i == m - 1); j < x; j++) {//j表示正在填的数为j
            ans[j] += base[i];//第i个数位填写j时(j0000...99999)的个数
            for (int k = 0; k < 10; k++) {
                ans[k] += base[i] * last[k];//前置答案增加
                ans[k] += f[i][k];//后置答案增加
            }
        }
        last[x]++;
    }
    //循环中，我们考虑数字情况只考虑到n - 1，对于n这个数而言，每个数各出现几次也作为答案
    //在先前的题目中，因为只需要考虑一个数字，所以写成if (!i) ans++;
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