#include <bits/stdc++.h>
using namespace std;
int n, m, q, op, x, y;
int r[1001], c[1001], a[1001][1001];
char ch;
int main() {
	cin >> n >> m >> q;
	for (int i = 1; i <= n; i++) {
		r[i] = i;
		for (int j = 1; j <= m; j++) {
			cin >> ch;
			a[i][j] = ch - '0';
		}
	}
	for (int j = 1; j <= m; j++) c[j] = j;
	while (q--) {
		cin >> op >> x >> y;
		if (op == 1) swap(r[x], r[y]);
		else if (op == 2) swap(c[x], c[y]);
		else cout << a[r[x]][c[y]] << endl;
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			cout << a[r[i]][c[j]];
		}
		cout << endl;
	}
	return 0;
}
