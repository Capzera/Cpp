#include <bits/stdc++.h>
using namespace std;
long long n, m, k, a[101][101], row[101], col[101], ans = INT_MAX;
int main() {
	cin >> n >> m >> k;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			cin >> a[i][j];
			row[i] += a[i][j];
			col[j] += a[i][j];
		}
	}
	for (int ans = 0; ; ans++) {
		int cnt = 0;
		for (int i = 1; i <= n; i++)
			for (int j = 1; j <= m; j++)
				cnt += a[i][j] >= row[i] + col[j];
		if (cnt >= k) {
			cout << ans;
			return 0;
		}
		for (int i = 1; i <= n; i++)
			for (int j = 1; j <= m; j++) a[i][j]--, row[i]--, col[j] --;
	}
	return 0;
}
