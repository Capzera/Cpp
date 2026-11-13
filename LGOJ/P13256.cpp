#include <bits/stdc++.h>
using namespace std;
int t, n, k, a[10001];
int solve() {
	cin >> n >> k;
	for (int i = 1; i <= n; i++) cin >> a[i];
	sort(a + 1, a + n + 1);
	int l = 1, r = n, ans = 0;
	while (l <= r) {
		ans++;
		if (a[l] + a[r] <= k) l++;
		r--;
	}
	return ans;
}
int main() {
	cin >> t;
	for (int i = 1; i <= t; i++) {
		printf("Case #%d: %d\n", i, solve());
	}
	return 0;
}
/*
5 1 6
1 2 3 4 5

x = 1 背包的容积为1，商铺装走重量为1的物品，小偷偷走重量为2
x = 2 背包容积为2，商铺装走重量为1的物品，小偷偷走重量为2
x = 3 背包容积为，商铺装走重量[1, 2]的物品，偷走重量为3
x = 4 同上
x = 5 同上
x = 6 装走[1, 2, 3] 小偷只能偷4

背包容积为x时，考虑是否可以将第i个物品装进窗口
*/
