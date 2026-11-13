#include <bits/stdc++.h>
using namespace std;
const int dx[] = {0, 1, 0, -1}, dy[] = {1, 0, -1, 0};
int n, a[10][10];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= n; j++)
			a[i][j] = -1;
	for (int i = 1, x = 1, y = 1, d = 0; i <= n * n; i++) {
		a[x][y] = i;
		int mx = x + dx[d], my = y + dy[d];
		if (mx > n || mx < 1 || my > n || my < 1 || a[mx][my] != -1) d = (d + 1) % 4;
		x += dx[d], y += dy[d];
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++)
			cout << fixed << setw(3) << a[i][j];
		cout << endl;
	}
	return 0;
}
