#include <bits/stdc++.h>
using namespace std;
map<int, int> mp;
int n, x, a[200010], base[31];
long long sum, ans;
int main() {
	base[0] = 1;
	for (int i = 1; i <= 30; i++) base[i] = base[i - 1] << 1;
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		mp[a[i]]++;
	}
	for (int i = 1; i <= n; i++) {
		mp[a[i]]--;
		for (int j = 0; j <= 30; j++) {
			if (!mp.count(base[j] - a[i])) continue;
			ans += mp[base[j] - a[i]];
		}
	}
	cout << ans;
	return 0;
}
