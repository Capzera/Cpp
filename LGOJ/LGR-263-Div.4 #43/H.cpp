#include <bits/stdc++.h>
using namespace std;
int a[16], row[4][5], col[4][5], pal[2][2][5];
int dfs(int i) {
	if (i == 16) return 1;
	if (a[i]) return dfs(i + 1);
	for (int x = 1; x <= 4; x++) {
		if (row[i / 4][x] || col[i % 4][x] || pal[i / 4 / 2][i % 4 / 2][x]) continue;
		row[i / 4][x] = col[i % 4][x] = pal[i / 4 / 2][i % 4 / 2][x] = 1;
		a[i] = x;
		if (dfs(i + 1)) return 1;
		a[i] = 0;
		row[i / 4][x] = col[i % 4][x] = pal[i / 4 / 2][i % 4 / 2][x] = 1;
	}
	return 0;
}
int main() {
	for (int i = 0; i < 16; i++) {
		cin >> a[i];
		if (!a[i]) continue;
		row[i / 4][a[i]] = col[i % 4][a[i]] = pal[i / 4 / 2][i % 4 / 2][a[i]] = 1;
	}
	dfs(0);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) cout << a[4 * i + j] << " ";
		cout << '\n';
	}
	return 0;
}
