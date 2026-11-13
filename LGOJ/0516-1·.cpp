#include <bits/stdc++.h>
using namespace std;
const int dx[] = {0, 1, 0, -1}, dy[] = {1, 0, -1, 0};
int n, a[21][21];
int main() {
	cin >> n;
	for (int i = 1, x = 1, y = 1, d = 0; i <= n * n; i++) {
		a[x][y] = i; // 填数
		int nx = x + dx[d], ny = y + dy[d];
		if (nx < 1 || nx > n || ny < 1 || ny > n || a[nx][ny]) {
			d = (d + 1) % 4;
			nx = x + dx[d], ny = y + dy[d];
		}
		x = nx, y = ny;
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			printf("%3d", a[i][j]);
		}
		puts("");
	}
	return 0;
}
