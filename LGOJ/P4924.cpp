#include <bits/stdc++.h>
using namespace std;
long long n, m, x, y, r, c;
int a[501][501], b[501][501];
void Rotate(int x, int y, int r) {
	for (int i = x - r; i <= x + r; i++)
		for (int j = y - r; j <= y + r; j++)
			b[r + i - x + 1][r + j - y + 1] = a[i][j];
	for (int i = x - r; i <= x + r; i++)
		for (int j = y - r; j <= y + r; j++)
			a[i][j] = b[r - j + y + 1][r + i - x + 1];
}
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= n; j++)
			a[i][j] = i * n + j - n;
	while (m--) {
		cin >> x >> y >> r >> c;
		Rotate(x, y, r);
		if (c) Rotate(x, y, r), Rotate(x, y, r);
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) cout << a[i][j] << " ";
		cout << endl;
	}
	return 0;
}
