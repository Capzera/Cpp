#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, ans = 0, last = -1;
	cin >> n;
	vector<pair<int, int> > a(n);
	for (int i = 0; i < n; i++) {
		cin >> a[i].first >> a[i].second;
	}
	sort(a.begin(), a.end(), [&](auto x, auto y) {
		return x.second < y.second;
	});
	for (int i = 0; i < n; i++) {
		if (a[i].first >= last) {
			ans++;
			last = a[i].second;
		}
	}
	cout << ans << endl;
	return 0;
}
