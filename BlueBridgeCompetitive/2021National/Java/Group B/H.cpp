#include <bits/stdc++.h>
using namespace std;
long long solve() {
    int n, x;
    cin >> x >> n;
    vector<vector<int>> nums(n, vector<int>(3));
    for (int i = 0; i < n; i++)
        cin >> nums[i][0] >> nums[i][1] >> nums[i][2];
    auto cmp = [](vector<int>& a, vector<int>& b) -> bool {
        return a[0] > b[0];
    };
    priority_queue<vector<int>, vector<vector<int>>, decltype(cmp)> pq;
    sort(nums.begin(), nums.end(), [&](auto& a, auto& b) { return a[1] > b[1]; });
    long long ans = 0;
    for (int t = x, i = 0; t >= 1; --t) {
        while (i < n && nums[i][1] >= t) pq.push(nums[i++]);
        if (pq.empty()) return -1;
        auto select = pq.top(); pq.pop();
        ans += select[0], select[2]--;
        if (select[2]) pq.push(select);
    }
    return ans;
}

int main() {
    cout << solve() << endl;
    system("pause");
    return 0;
}
/*
小蓝很喜欢吃巧克力，他每天都要吃一块巧克力。
一天小蓝到超市想买一些巧克力。超市的货架上有很多种巧克力，
每种巧克力有自己的价格、数量和剩余的保质期天数，小蓝只吃没过保质期的巧克力，
请问小蓝最少花多少钱能买到让自己吃 x 天的巧克力。
输入描述
输入的第一行包含两个整数 x, n，分别表示需要吃巧克力的天数和巧克力的种类数。
接下来 n 行描述货架上的巧克力，其中第 i 行包含三个整数 ai, bi, ci，
表示第 i 种巧克力的单价为 ai，保质期还剩 bi天（从现在开始的 bi天可以吃），数量为 ci。
输出描述
输出一个整数表示小蓝的最小花费。如果不存在让小蓝吃 x 天的购买方案，输出 ?1。
输入输出样例
示例
输入
10 3
1 6 5
2 7 3
3 10 10
输出
18
样例说明
一种最佳的方案是第 1 种买 5 块，第 2 种买 2 块，第 3 种买 3 块。前 5 天吃第 1 种，第 6、7 天吃第 2 种，第 8 至 10 天吃第 3 种。
*/
