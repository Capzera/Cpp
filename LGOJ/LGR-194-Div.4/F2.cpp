#include <bits/stdc++.h>
#define int long long
using namespace std;
int n, m, x, a[105][105];
signed main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> m;
		while (m--) {
			cin >> x;
			a[i][x] = 1; // 邻接表
		}
	}
	for (int i = 1; i <= n; i++) { // 访问起始点1
		vector<int> vis(n + 1);
		for (int j = 1; j <= n; j++) { // 枚举终点
			if (i == j || !a[i][j]) continue;
			vis[j] = 1;
			for (int k = 1; k <= n; k++) {
				if (k == j || k == i || !a[j][k]) continue;
				vis[k] = 1;
			}
		}
		int ans = 0;
		for (int j = 1; j <= n; j++) {
			if (j == i ) continue;
			ans += vis[j];
		}
		cout << ans << " ";
	}
	return 0;
}
/*
a[i][j] = 1
表示i 到 j 有一条有向边
*/
