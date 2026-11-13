#include <bits/stdc++.h>
using namespace std;
int n, m, ans, a[11][11];
string s;
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		cin >> s;
		for (int j = 1; j <= m; j++) {
			int x = s[j - 1] == '1' ? 1 : -1;
			a[i][j] = a[i - 1][j] + a[i][j - 1] - a[i - 1][j - 1] + x;
		}
	}
	for (int x1 = 1; x1 <= n; x1++) {
		for (int y1 = 1; y1 <= m; y1++) {
			for (int x2 = x1; x2 <= n; x2++) {
				for (int y2 = y1; y2 <= m; y2++) {
					if (!(a[x2][y2] - a[x1 - 1][y2] - a[x2][y1 - 1] + a[x1 - 1][y1 - 1])) {
						ans = max(ans, (x2 - x1 + 1) * (y2 - y1 + 1));
					}
				}
			}
		}
	}
	cout << ans;
	return 0;
}
