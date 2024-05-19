#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, m, cnt;
	cin >> n >> m; cnt = m = m * 3 / 2;
	vector<vector<int> > a(n + 1, vector<int>(2));
	for (int i = 1; i <= n; i++) cin >> a[i][0] >> a[i][1];
	sort(a.begin() + 1, a.end(),[&](auto& x, auto& y) {
		if (x[1] != y[1]) return x[1] > y[1];
		return x[0] < y[0];
	});
	for (int i = m + 1; i <= n; i++) {
		cout << i << " " << a[i][1] << endl;
		if (a[i][1] != a[i][m]) break;
		cnt++;
	}
	cout << a[m][1] << " " << cnt << endl;
	for (int i = 1; i <= cnt; i++) {
		cout << a[i][0] << " " << a[i][1] << endl;
	}
	return 0;
}
