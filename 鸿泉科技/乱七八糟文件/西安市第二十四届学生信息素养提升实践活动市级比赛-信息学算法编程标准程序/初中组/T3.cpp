/*
冰激凌促销 搜索-二分查找-思维
*/
#include <bits/stdc++.h>
using namespace std;
int n, k, a[17];
vector<long long> ans[2];
void dfs(int l, int r, long long m, int st) {
	if (l > r) {
		ans[st].emplace_back(m);
		return;
	}
	if (1e18 / a[l] >= m) {
		dfs(l, r, m * a[l], st);
	}
	dfs(l + 1, r, m, st);
}	
bool check(long long x) {
	int n = ans[0].size(), m = ans[1].size();
	long long ret = 0;
	for (int i = 0; i < m; i++) {
		while (n && x / ans[0][n - 1] < ans[1][i]) {
			n--;
		}
		ret += n;
	}
	return ret >= k;
}
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	sort(a + 1, a + n + 1);
	for (int i = 1; i <= n / 2; i++) {
		if (i & 1) {
			swap(a[i], a[i + n / 2]);
		}
	}
	cin >> k;
	dfs(1, n / 2, 1, 0);
	dfs(n / 2 + 1, n, 1, 1);
	sort(ans[0].begin(), ans[0].end());
	sort(ans[1].begin(), ans[1].end());
	long long l = 0, r = LLONG_MAX / 2, ret = -1;
	while (l < r) {
		long long x = l + (r - l) / 2;
		if (check(x)) ret = x, r = x;
		else l = x + 1;
	}
	cout << ret << endl;
	return 0;
}
