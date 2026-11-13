#include <bits/stdc++.h>
using namespace std;
long long n, m, x, a[65][65][3], pre[65][65][3];
double ans = -1e9;
int main() {
	cin >> n >> m;
	for (int k = 0; k < 3; k++)
		for (int i = 1; i <= n; i++)
			for (int j = 1; j <= m; j++)
				cin >> a[i][j][k];
	for (int k = 0; k < 3; k++)
		for (int i = 1; i <= n; i++)
			for (int j = 1; j <= m; j++) {
				cin >> x;
				pre[i][j][k] = pre[i - 1][j][k] + pre[i][j - 1][k] - pre[i - 1][j - 1][k] + (a[i][j][k] - x) * (a[i][j][k] - x);
			}
	for (int lx = (n + 1) / 2; lx <= n; lx++) {
		for (int x1 = 1; x1 <= n - lx + 1; x1++) {
			int x2 = x1 + lx - 1;
			for (int ly = (m + 1) / 2; ly <= m; ly++) {
				for (int y1 = 1; y1 <= m - ly + 1; y1++) {
					int y2 = y1 + ly - 1;
					long long tot = 0;
					for (int k = 0; k < 3; k++) tot += pre[x2][y2][k] - pre[x1 - 1][y2][k] - pre[x2][y1 - 1][k] + pre[x1 - 1][y1 - 1][k];
					double psnr = 20.0 * log10(255.0 / sqrt(tot / (3.0 * lx * ly)));
					ans = max(ans, psnr);
				}
			}
		}
	}
	printf("%.8lf", ans);
	return 0;
}
