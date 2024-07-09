#include <bits/stdc++.h>
using namespace std;
long long ans = LLONG_MIN;
long long n, k, x, a[200010], sum[200010];
int main() {
	memset(a, 0, sizeof(a));
	memset(sum, 0, sizeof(sum));
	cin >> n >> k >> x;
	for (int i = 0; i < n; i++) {
		cin >> a[i];
	}
	sort(a, a + n);
	for (int i = 0; i < n; i++) {
		sum[i + 1] = sum[i] + a[i];
	}
	for (int i = 0; i <= min(k, n); i++) {
		if (i + x > n) ans = max(ans, -1ll * sum[n - i]);
		else ans = max(ans, 2ll * sum[n - i - x] - sum[n - i]);
	}
	cout << ans << endl;
	return 0;
}
