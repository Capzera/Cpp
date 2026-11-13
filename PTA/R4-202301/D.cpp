#include <bits/stdc++.h>
using namespace std;
int n, m, x, y, r, z;
vector<vector<int> > a(101, vector<int>(101)), b;
void rotate(int x, int y, int r, int z) {
	for (int i = x - r; i <= x + r; i++) {
		for (int j = y - r; j <= y + r; j++) {
			a[i][j] = z ? b[x - y + j][x + y - i] : b[x - j + y][i + y - x];
		}
	}
	b = a;
}
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n * n; i++) {
		a[(i - 1) / n + 1][(i - 1) % n + 1] = i;
	}
	b = a;
	while (m--) {
		cin >> x >> y >> r >> z;
		rotate(x, y, r, z);
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) cout << a[i][j] << " ";
		puts("");
	}
	return 0;
}
