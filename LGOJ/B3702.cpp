#include <bits/stdc++.h>
using namespace std;
long long m, n, sx, sy;
struct node {
	int x, y;
} a[1001][1001];
int main() {
	cin >> m >> n >> sx >> sy;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++)
			cin >> a[i][j].x >> a[i][j].y;
	}
	while (a[sx][sy].x && a[sx][sy].y) {
		cout << sx << " " << sy << endl;
		int mx = a[sx][sy].x, my = a[sx][sy].y;
		sx = mx, sy = my;
	}
	cout << sx << " " << sy;
	return 0;
}
