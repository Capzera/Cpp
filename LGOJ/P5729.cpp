#include <bits/stdc++.h>
#define int long long
using namespace std;
int w, x, h, m, ans, a[21][21][21];
signed main() {
	cin >> w >> x >> h >> m;
	while (m--) {
		int x1, y1, z1, x2, y2, z2;
		cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
		for (int i = x1; i <= x2; i++) {
			for (int j = y1; j <= y2; j++) {
				for (int k = z1; k <= z2; k++) {
					a[i][j][k] = 1;
				}
			}
		}
	}
	for (int i = 1; i <= w; i++) {
		for (int j = 1; j <= x; j++) {
			for (int k = 1; k <= h; k++) {
				ans += a[i][j][k];
			}
		}
	}
	cout << w * x * h - ans;
	return 0;
}
