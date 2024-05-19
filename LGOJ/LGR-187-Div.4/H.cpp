#include <bits/stdc++.h>
using namespace std;
int n, m, op, l, r;
int main() {
	cin >> n >> m;
	vector<vector<int> > a(n, vector<int>(n));
	vector<int> row(n), col(n);
	iota(row.begin(), row.end(), 0);
	iota(col.begin(), col.end(), 0);
	for (auto& i : a)
		for (auto& j : i)
			cin >> j;
	while (m--) {
		cin >> op >> l >> r;
		if (op) swap(row[l - 1], row[r - 1]);
		else swap(col[l - 1], col[r - 1]);
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) cout << a[row[i]][col[j]] << " ";
		cout << endl;
	}
	return 0;
}
