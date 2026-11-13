#include <bits/stdc++.h>
using namespace std;
const int N = 210, M = 21;
long long n, m, x, y, base[N][6], a[M][N], f[M][N];
int main() {
	cin >> n >> m;
	memset(f, 0x3f, sizeof(f));
	f[0][0] = 0;
	for (int i = 1; i <= n; i++) {
		base[i][0] = 1;
		for (int j = 1; j <= 5; j++) {
			base[i][j] = base[i][j - 1] * i;
		}
	}
	for (int i = 1; i <= m; i++) {
		cin >> x >> y;
		f[i][0] = 0;
		for (int j = 0; j <= n; j++) {
			a[i][j] = x * base[j][y];
			for (int k = 0; k <= j; k++) {
				f[i][j] = min(f[i][j], f[i - 1][k] + a[i][j - k]);
			}
		}
	}
	cout << f[m][n];
	return 0;
}

