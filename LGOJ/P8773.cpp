#include<bits/stdc++.h>
using namespace std;
int n, m, x, l, r, a[100005], f[100005];
unordered_map<int, int> mp;
int main() {
	cin >> n >> m >> x;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		mp[a[i]] = i;
		f[i] = max(f[i - 1], mp[a[i] ^ x]);
	}
	while (m--) {
		cin >> l >> r;
		cout << (f[r] < l ? "no\n" : "yes\n");
	}
	return 0;
}

