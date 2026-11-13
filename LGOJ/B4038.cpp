#include <bits/stdc++.h>
using namespace std;
int t, n, a[10001];
bool solve() {
	cin >> n;
	long long tot = 0, sum = 0;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		tot += a[i];
	}
	for (int i = 1; i <= n; i++) {
		sum += a[i];
		if (sum == tot - a[i]) return 1;
		tot -= a[i];
	}
	return 0;
}
int main() {
	cin >> t;
	while (t--) {
		cout << (solve() ? "Yes\n" : "No\n");
	}
	return 0;
}
