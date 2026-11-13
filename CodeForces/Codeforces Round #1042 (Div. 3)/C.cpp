#include <bits/stdc++.h>
using namespace std;
int t, n, k;
int solve() {
	cin >> n >> k;
	vector<int> s(n), t(n);
	for (int& x : s) {
		cin >> x;
		x = min(x % k, k - x % k);
	}
	for (auto& x : t) {
		cin >> x;
		x = min(x % k, k - x % k);
	}
	sort(s.begin(), s.end()), sort(t.begin(), t.end());
	return s == t;
}
int main() {
	cin >> t;
	while(t--) cout << (solve() ? "YES\n" : "NO\n");
	return 0;
}
