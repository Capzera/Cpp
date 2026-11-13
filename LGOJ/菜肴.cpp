#include <bits/stdc++.h>
using namespace std;
int n, m, ans;
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m;
	ans = n;
	vector<vector<long long> > a(m, vector<long long>(n));
	vector<long long> s(m);
	for (auto& x : s) cin >> x;
	for (auto& line : a)
		for (auto& x : line) 
			cin >> x;
	for (int i = 0; i < m; i++) {
		sort(a[i].begin(), a[i].end(), greater<long long>());
		long long sum = 0;
		for (int j = 0; j < n; j++) {
			if (sum + a[i][j] > s[i]) {
				ans = min(ans, j + 1);
				break;
			}
			sum += a[i][j];
		}
	}
	cout << ans;
	return 0;
}
/*
5 3
2000000000000 2000000000000 2000000000000
1 1 1 1 1
2 2 2 2 2
3 3 3 3 3
*/
