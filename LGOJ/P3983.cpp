#include <bits/stdc++.h>
using namespace std;
const int N = 11, M = 1e5 + 10;
int n = 10, m, c[N], cost[] = {0, 1, 3, 5, 7, 9, 10, 11, 14, 15, 17};
long long ans, f[N], g[M];
int main() {
	cin >> m;
	for (int i = 1; i <= n; i++) {
		cin >> c[i];
	}
	for (int i = 1; i <= n; i++) {
		for (int j = i; j <= n; j++) {
			f[j] = max(f[j], f[j - i] + c[i]);
		}
	}
	for (int i = 1; i <= n; i++) f[i] -= cost[i];
	for (int i = 1; i <= n; i++) {
		for (int j = i; j <= m; j++) {
			g[j] = max(g[j], g[j - i] + f[i]);
		}
	}
	for (int i = 1; i <= m; i++) ans = max(ans, g[i]);
	cout << ans;
	return 0;
}
