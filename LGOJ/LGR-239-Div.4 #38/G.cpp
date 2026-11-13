#include <bits/stdc++.h>
using namespace std;
const int N = 110;
long long n, q, x, y, a[N][N], b[N][N];
int main() {
	cin >> n >> q;
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= n; j++)
			cin >> a[i][j];
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= n; j++)
			cin >> b[i][j];
	while (q--) {
		cin >> x >> y;
		long long c1 = LLONG_MAX, c2 = LLONG_MAX;
		if (a[x][y] != -1) c1 = a[x][y];
		for (int i = 1; i <= n; i++) {
			if (i == x || i == y || b[x][i] == -1 || b[i][y] == -1) continue; // 中转点不能等于起终点
			c2 = min(c2, b[x][i] + b[i][y]);
		}
		long long ans = min(c1, c2);
		cout << (ans == LLONG_MAX ? -1 : ans) << '\n';
	}
	return 0;
}
