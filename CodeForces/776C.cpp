#include <bits/stdc++.h>
using namespace std;
long long n, k, ans, mx, a[100010], sum[100010];
map<long long, long long> mp;
unordered_set<long long> se;
int main() {
	cin >> n >> k;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		sum[i] = sum[i - 1] + a[i];
		mx += abs(a[i]);
	}
	for (long long p = 1; abs(p) <= mx; p *= k) {
		if (se.count(p)) break;
		se.insert(p);
		mp.clear();
		mp[0] = 1;
		for (int i = 1; i <= n; i++) {
			ans += mp[sum[i] - p];
			mp[sum[i]]++;
		}
	}
	cout << ans;
	return 0;
}
