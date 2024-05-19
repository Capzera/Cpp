#include <bits/stdc++.h>
using namespace std;
long long ans, n, k;
int main() {
	cin >> n >> k;
	ans = n * k;
	for (int l = 1, r = 1; l <= k && l <= n; l = r + 1) {
		r = min(k / (k / l), n);
		ans -= (k / l) * (r - l + 1) * (l + r) / 2;
	}
	cout << ans;
	return 0;
}
