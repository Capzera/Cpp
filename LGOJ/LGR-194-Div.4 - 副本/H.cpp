#include <bits/stdc++.h>
#define int long long
using namespace std;
int n, m, ans;
int a[1005][1005];
signed main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cin >> a[i][j];
		}
	}
	for (int i = 0; i < n / 3; i++) {
		for (int j = 0; j < m / 3; j++) {
			ans += a[i * 3 + 1][j * 3 + 1];
		}
	}
	cout << ans;
	return 0;
}
