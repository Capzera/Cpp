#include <bits/stdc++.h>
using namespace std;
int a[22][22];
int x, y, n;
int main() {
	cin >> n;
	for (int i = 1, x = 0, y = n >> 1; i <= n * n; i++) {
		a[x][y] = i;
		int nx = (x - 1 + n) % n, ny = (y + 1) % n;
		if (a[nx][ny]) nx = (x + 1) % n, ny = y;
		x = nx, y = ny;
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) cout << a[i][j] << " ";
		puts("");
	}
	return 0;
}
