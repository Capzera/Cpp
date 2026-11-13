#include <bits/stdc++.h>
using namespace std;
int m, n, tot, x, ans = INT_MAX, c[16], sum[100010], dp[1 << 16], f[1 << 16];
// dp[i] 表示在状态i下，最多可以买多少物品
// f[i] 表示在状态i下，所需要硬币的花费
// f[i] = f[i ^ 1 << j] + c[j]
// dp[i] = max(dp[i], check(c[j], dp[i ^ 1 << j] + 1))
int check(int x, int j) {
	int l = j, r = m + 1; // 当前硬币面额为x，且之前已经购买完了j个物品的情况下，可以买到多少停止
	while (l < r) {
		int mid = l + (r - l >> 1); // 枚举一段空间的终点
		if (sum[mid] - sum[j - 1] == x) return mid; // 如果刚好这一段等于硬币面额，直接返回终点位置
		if (sum[mid] - sum[j - 1] > x) r = mid; // 如果物品价格大于硬币面额，往少了找
		else l = mid + 1;
	}
	return l - 1;
}
int main() {
	cin >> n >> m;
	for (int i = 0; i < n; i++) cin >> c[i], tot += c[i];
	for (int i = 1; i <= m; i++) cin >> x, sum[i] = sum[i - 1] + x;
	for (int i = 1; i < 1 << n; i++) {
		for (int j = 0; j < n; j++) {
			if (i >> j & 1 ^ 1) continue;
			f[i] = f[i ^ 1 << j] + c[j]; // 计算状态为i时的硬币总花费价格
			dp[i] = max(dp[i], check(c[j], dp[i ^ 1 << j] + 1)); // 计算状态为i时最多可以买到的物品个数
			if (dp[i] == m) ans = min(ans, f[i]); // 刚好能买m个物品，统计最小的硬币花费
		}
	}
	cout << (tot - ans < 0 ? -1 : tot - ans); // 如果最优情况都不能买这些东西，则无解
	return 0;
}
