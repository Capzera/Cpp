#include <bits/stdc++.h>
using namespace std;
long long n, m, q, x, y, z, a[1001][1001];
int main() {
	cin >> n >> m >> q;
	for (int i = 1; i <= n; i++) cin >> a[0][i];
	for (int i = 1; i <= m; i++) {
		for (int j = 1; j <= n; j++) {
			cin >> a[i][j];
			a[i][j] += a[i - 1][j];
		}
	}
	while (q--) {
		cin >> x >> y >> z;
		cout << a[x][y] - a[x][z] << endl;
	}
	return 0;
}
