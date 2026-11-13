#include <bits/stdc++.h>
using namespace std;
long long n, m, sum, ans, a[200001][101];
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) cin >> a[j][i];
	}
	for (int i = 1; i <= m; i++) sort(a[i] + 1, a[i] + n + 1, greater<long long>());
	for (int i = 1; i <= m; i++) {
		sum = a[i][1] + max(0ll, sum);
		ans = max(ans, sum);
	}
	cout << ans;
	return 0;
}
/*
1 2 2 2 3
f[i]表示前[i]个人最多可以划分为几个队伍
对数组进行排序
i表示第i个人，a[i]表示第i个人希望在大于等于i的队伍中
所以上一状态为i - a[i]
f[i] = max(f[1..(i-a[i])) + 1;
g[i] = max(g[i - 1], f[i]);
*/
