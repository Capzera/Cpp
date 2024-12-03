#include <bits/stdc++.h>
#define int long long
using namespace std;
int n;
signed main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n;
	vector<vector<int> > a(n, vector<int>(3));
	for (int i = 0; i < n; i++) {
		cin >> a[i][0] >> a[i][1];
		a[i][2] = i + 1;
	}
	sort(a.begin(), a.end(), [&](auto& x, auto& y) {
		if (x[1] * y[0] != y[1] * x[0]) return x[1] * y[0] > y[1] * x[0];
		return x[0] < y[0];
	});
	cout << a[0][2];
	return 0;
}
/*
x[1] * y[0]  > y[1] * x[0];
*/
