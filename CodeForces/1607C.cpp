#include <bits/stdc++.h>
using namespace std;
void solve() {
	int n;
	cin >> n;
	vector<long long> a(n);
	for (auto& x : a) cin >> x;
	sort(a.begin(), a.end());
	long long mx = a[0];
	for (int i = 1; i < n; i++) {
		mx = max(mx, a[i] - a[i - 1]);
	}
	cout << mx << endl;
}
int main() {
	int t;
	cin >> t;
	while (t--) {
		solve();
	}
	return 0;
}
/*
1 2 7 10

*/
