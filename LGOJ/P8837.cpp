#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, m;
	cin >> n >> m;
	vector<int> a(n), b(m);
	for (auto& x : a) cin >> x;
	for (auto& x : b) cin >> x;
	sort(a.begin(), a.end(), greater<int>());
	sort(b.begin(), b.end(), greater<int>());
	long long ans = 0;
	for (int i = 0, j = 0; i < n && j < m; i++, j++) {
		while (j < m && a[i] < b[j]) j++;
		if (j == m) break;
		ans++;
	}
	cout << ans;
	return 0;
}
/*
1 1 3 3 4 5 5 6 6 6 7 7 8 9 10
1 4 4 5 5 6 7 8 8 8 8 9 9 9  11 12 14 18 19 20
*/
