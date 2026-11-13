#include <bits/stdc++.h>
using namespace std;
const int N = 2e5 + 10, K = 50;
int n, k, p, a[N], b[N], cnt[K], mp[K];
long long ans = 0, tot = 0;
int main() {
	cin >> n >> k >> p;
	for (int i = 1; i <= n; i++) {
		cin >> a[i] >> b[i];
		cnt[a[i]]++;
	}
	for (int i = 0; i < k; i++) {
		tot += 1ll * cnt[i] * (cnt[i] - 1) / 2;
	}
	for (int i = 1; i <= n; i++) {
		if (b[i] > p) {
			mp[a[i]]++;
			continue;
		}
		for (int j = 0; j < k; j++) {
			ans += 1ll * mp[j] * (mp[j] - 1) / 2;
			mp[j] = 0;
		}
	}
	for (int j = 0; j < k; j++) {
		ans += 1ll * mp[j] * (mp[j] - 1) / 2;
	}
	cout << tot - ans;
	return 0;
}
