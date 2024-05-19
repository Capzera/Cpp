#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, mn = INT_MAX, mx = 0, m;
	cin >> n;
	m = 2 * n;
	vector<int> stones(n), sum(m + 1);
	vector<vector<int> > f(m, vector<int>(m)), g(m, vector<int>(m));
	for (int i = 0; i < n; i++) {
		cin >> stones[i];
	}
	stones.insert(stones.end(), stones.begin(), stones.end() - 1);
	for (int i = 0; i < m; i++) {
		sum[i + 1] = sum[i] + stones[i];
	}
	for (int len = 2; len <= n; len++) {
		for (int i = 0; i < m - len; i++) {
			int j = i + len - 1, val = sum[j + 1] - sum[i];
			f[i][j] = INT_MAX;
			for (int k = i; k < j; k++) {
				f[i][j] = min(f[i][j], f[i][k] + f[k + 1][j] + val);
				g[i][j] = max(g[i][j], g[i][k] + g[k + 1][j] + val);
			}
		}
	}
	for (int i = 0; i < n; i++) {
		mn = min(mn, f[i][i + n - 1]);
		mx = max(mx, g[i][i + n - 1]);
	}
	cout << mn << endl << mx << endl;
	system("pause");
	return 0;
}
