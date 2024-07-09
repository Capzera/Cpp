#include <bits/stdc++.h>
using namespace std;
long long a[2005][2005];
int main() {
	for (int i = 0; i < 2005; i++) {
		memset(a[i], 0, sizeof(a[i]));
	}
	long long n, m, q, x, r1, r2, c1, c2;
	cin >> n >> m >> q;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			cin >> x;
			a[i][j] = a[i - 1][j] + a[i][j - 1] - a[i - 1][j - 1] + x;
		}
	}
	while (q--) {
		cin >> r1 >> c1 >> r2 >> c2;
		cout << a[r2][c2] - a[r2][c1 - 1] - a[r1 - 1][c2] + a[r1 - 1][c1 - 1] << endl;
	}
	return 0;
}
