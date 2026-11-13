#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
long long t, n, a[N];
int solve() {
	long long sum = 0, mn = LLONG_MAX;
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		sum += a[i];
		mn = min(mn, a[i]);
	}
	if (sum % 5) return 0;
	for (int i = 1; i <= n; i++) {
		if (a[i] * 5 > sum) return 0;
	}
	return 1;
}
int main() {
	cin >> t;
	while (t--) cout << (solve() ? "T\n" : "F\n");
	return 0;
}


