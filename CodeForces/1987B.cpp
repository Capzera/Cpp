#include<bits/stdc++.h>
using namespace std;
void solve() {
	long long n, ans = 0;
	cin >> n;
	vector<int> a(n), cnt = {0};
	for (int i = 0; i < n; i++) {
		cin >> a[i];
		if (i && a[i] < a[i - 1]) {
			cnt.push_back(a[i - 1] - a[i]);
			a[i] = a[i - 1];
		}
	}
	sort(cnt.begin(), cnt.end());
	int m = cnt.size();
	for (int i = 1; i < m; i++) {
		ans += 1ll * (cnt[i] - cnt[i - 1]) * (m + 1 - i);
	}
	cout << ans << endl;
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
9
344 12 37 60 311 613 365 328 675



b[1] = 3
b[2] = 1
b[3] = 1
b[4] = 0
1 2 3
3


*/
