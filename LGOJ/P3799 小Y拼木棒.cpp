#include <bits/stdc++.h>
using namespace std;
long long n, x, ans, mod = 1e9 + 7;
unordered_map<int, long long> mp;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> x;
		mp[x]++;
	}
	for (int i = 2; i <= 5000; i++) {
		if (mp[i] < 2) continue;
		for (int j = 1; j <= i >> 1; j++) {
			if (!mp[j] || !mp[i - j]) continue;
			if (j + j == i) ans = (ans + mp[i] * (mp[i] - 1) / 2 * mp[j] * (mp[j] - 1) / 2) % mod;
			else ans = (ans + mp[i] * (mp[i] - 1) / 2 * mp[j] * mp[i - j]) % mod;
		}
	}
	cout << ans;
	return 0;
}
