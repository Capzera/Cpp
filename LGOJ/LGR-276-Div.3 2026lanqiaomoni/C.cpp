#include <bits/stdc++.h>
using namespace std;
long long t, n, x, a[100001];
int solve() {
	scanf("%lld%lld", &n, &x);
	for (int i = 1; i <= n; i++) scanf("%lld", &a[i]);
	for (int i = 2; i <= n; i++) {
		if (a[i] - a[i - 1] > 1) return 0;
		if (a[i - 1] - a[i] > x) return 0;
	}
	return 1;
}
int main() {
	cin >> t;
	while (t--) cout << (solve() ? "Win\n" : "Lose\n");
	return 0;
}
