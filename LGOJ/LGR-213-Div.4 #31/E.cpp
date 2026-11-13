#include <bits/stdc++.h>
using namespace std;
int n, m, k;
int main() {
	cin >> n >> m >> k;
	vector<vector<char> > ans(n, vector<char>(m, '.'));
	for (int j = m - 1, now = 1, f = 0; j >= 0; j--) {
		int st = max(n / 2 - k / 2, 0), ed = min(n / 2 + k / 2, n - 1);
		if (!f) {
			st = max(n / 2 - now / 2, 0), ed = min(n / 2 + now / 2, n - 1);
		}
		now += 2;
		if (now == n + 2) f = 1;
		for (int i = st; i <= ed; i++) ans[i][j] = '#';
	}
	for (auto& i : ans) {
		for (auto& x : i) cout << x;
		cout << endl;
	}
	return 0;
}
