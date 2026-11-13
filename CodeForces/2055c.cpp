#include <bits/stdc++.h>
using namespace std;
const int N = 1e3 + 5;
long long T, n, m, len, row[N], col[N], a[N][N];
string s;
void solve() {
	cin >> n >> m >> s;
	memset(row, 0, sizeof row);
	memset(col, 0, sizeof col);
	s += s.back();
	for (int i = 1; i <= n; i++) for (int j = 1; j <= m; j++) {
		cin >> a[i][j];
		row[i] += a[i][j];
		col[j] += a[i][j];
	}
	for (int x = 1, y = 1, k = 0; k < s.size(); k++) {
		if (s[k] == 'D') {
			a[x][y] = -row[x];
			col[y] += a[x++][y];
		} else {
			a[x][y] = -col[y];
			row[x] += a[x][y++];
		}
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) cout << a[i][j] << " ";
		cout << "\n";
	}
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> T;
	while (T--) solve();
	return 0;
}
