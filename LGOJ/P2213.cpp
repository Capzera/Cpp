#include <bits/stdc++.h>
using namespace std;
long long n, m, x, ans, a[801][801], s[801][801];
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cin >> a[i + j - 1][n - i + j];
		}
	}
	for (int i = 1; i <= 2 * n; i++) {
		for (int j = 1; j <= 2 * n; j++) {
			s[i][j] = s[i - 1][j] + s[i][j - 1] - s[i - 1][j - 1] + a[i][j];
		}
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			long long x2 = i + j - 1, y2 = n - i + j;
			long long x1 = max(x2 - m, 1ll), y1 = max(y2 - m, 1ll);
			x2 = min(x2 + m, n * 2 - 1), y2 = min(y2 + m, n * 2 - 1);
			long long sum = s[x2][y2] - s[x2][y1 - 1] - s[x1 - 1][y2] + s[x1 - 1][y1 - 1];
			ans = max(ans, sum);
		}
	}
	cout << ans;
	return 0;
}
/*
5 3
50 5 25 6 17
14 3 2 7 21
99 10 1 2 80
8 7 5 23 11
10 0 78 1 9

*/
