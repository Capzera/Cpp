#include <bits/stdc++.h>
using namespace std;
long long n, m, x, y, ans1, ans2, a[1001][1001];
int main() {
	cin >> n >> m >> x >> y;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			cin >> a[i][j];
		}
	}
	for (int i = x + 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			ans1 += a[i][j];
		}
	}
	for (int j = 1; j <= m; j++) {
		ans2 += a[x][j];
	}
	cout << ans1 + 1 << " " << ans1 + ans2 << endl;
	return 0;
}
