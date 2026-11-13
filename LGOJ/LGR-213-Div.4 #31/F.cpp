#include <bits/stdc++.h>
using namespace std;
long long n, m, a[51][51], col[51], mx[51], mv, j, ans;
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			cin >> a[i][j];
			col[j] += a[i][j];
			mx[j] = max(mx[j], a[i][j]);
		}
	}
	for (int i = 1; i <= m; i++) {
		if (col[i] >= mv) {
			mv = col[i];
			j = i;
		}
	}
	for (int i = 1; i <= n; i++) ans += a[i][j] == mx[j];
	cout << mx[j] << " " << ans;
	return 0;
}
