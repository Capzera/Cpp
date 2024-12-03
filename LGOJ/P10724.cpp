#include <bits/stdc++.h>
using namespace std;
long long n, ans;
int p[] = {0, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
unordered_map<int, int> mp;
int main() {
	cin >> n;
	vector<int> a(n + 1);
	mp[0] = 1;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		int res = 0;
		for (int j = 1; j <= 10; j++) {
			if (p[j] > a[i]) break;
			while (a[i] % p[j] == 0) {
				res ^= 1 << j;
				a[i] /= p[j];
			}
		}
		a[i] = a[i - 1] ^ res;
		ans += mp[a[i]];
		mp[a[i]]++;
	}
	cout << ans;
	return 0;
}
