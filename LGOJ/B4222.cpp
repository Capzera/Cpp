#include <bits/stdc++.h>
using namespace std;
long long a[1001][1001], sum[1001][1001], n, m, tot, ans = -1;
bool check(int x) {
	memset(sum, 0, sizeof(sum));
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++) 
			sum[i][j] = sum[i - 1][j] + sum[i][j - 1] - sum[i - 1][j - 1] + (a[i][j] >= x);
	for (int x1 = 1; x1 <= n - x + 1; x1++) {
		for (int y1 = 1; y1 <= m - x + 1; y1++) {
			int x2 = x1 + x - 1, y2 = y1 + x - 1;
			if (sum[x2][y2] - sum[x2][y1 - 1] - sum[x1 - 1][y2] + sum[x1 - 1][y1 - 1] == x * x) {
				return 1;
			}
		}
	}
	return 0;
}
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			cin >> a[i][j];
			tot += a[i][j];
		}
	}
	int l = 1, r = min(n, m) + 1;
	while (l < r) {
		int mid = l + (r - l >> 1);
		if (check(mid)) {
			ans = mid;
			l = mid + 1;
		} else r = mid;
	}
	cout << tot - ans * ans * ans;
	return 0;
}
