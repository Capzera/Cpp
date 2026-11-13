#include <bits/stdc++.h>
using namespace std;
int n, m, k, ans = INT_MAX, sum[101][101];
string s;
int main() {
	cin >> n >> m >> k;
	for (int i = 1; i <= n; i++) {
		cin >> s;
		for (int j = 1; j <= m; j++) {
			sum[i][j] = sum[i - 1][j] + sum[i][j - 1] - sum[i - 1][j - 1] + s[j - 1] - '0';
		}
	}
	for (int x1 = 1; x1 <= n; x1++) {
		for (int y1 = 1; y1 <= m; y1++) {
			for (int x2 = x1; x2 <= n; x2++) {
				for (int y2 = y1; y2 <= m; y2++) {
					long long cnt = sum[x2][y2] - sum[x2][y1 - 1] - sum[x1 - 1][y2] + sum[x1 - 1][y1 - 1];
					if (cnt >= k) {
						ans = min(ans, (x2 - x1 + 1) * (y2 - y1 + 1));
					}
				}
			}
		}
	}
	if (ans == INT_MAX) ans = 0;
	cout << ans;
	return 0;
}
