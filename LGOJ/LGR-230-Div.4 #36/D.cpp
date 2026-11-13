#include <bits/stdc++.h>
using namespace std;
long long n, k, s, t, x, ans, a[100010], b[100010], c[100010];
int main() {
	cin >> n;
	for (int i = 1; i < n; i++) cin >> a[i];
	cin >> k;
	for (int i = 1; i <= k; i++) cin >> b[i];
	for (int i = 1; i <= k; i++) cin >> c[b[i]];
	cin >> s >> t;
	for (int i = s; i < t; i++) {
		ans += a[i];
		if (i != s) ans += c[i];
	}
	cout << ans;
	return 0;
}
