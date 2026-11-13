#include <bits/stdc++.h>
using namespace std;
const int N = 2e4 + 10, p1 = 131, m1 = 1e9 + 7;

int n, k, x;
long long h[N], p[N];
long long get(int l, int r) {
	return (h[r] - h[l - 1] * p[r - l + 1] % m1 + m1) % m1; 
}
int check(int len) {
	unordered_map<long long, int> mp;
	for (int l = 1; l + len - 1 <= n; l++) {
		long long r = l + len - 1, x = get(l, r);
		if (++mp[x] >= k) return 1;
	}
	return 0;
}
int main() {
	cin >> n >> k;
	p[0] = 1;
	for (int i = 1; i <= n; i++) {
		cin >> x;
		p[i] = p[i - 1] * p1 % m1;
		h[i] = (h[i - 1] * p1 + x) % m1;
	}
	int l = 0, r = n + 1, ans = 0;
	while (l < r) {
		int mid = l + (r - l >> 1);
		if (check(mid)) {
			ans = mid;
			l = mid + 1;
		} else r = mid;
	}
	cout << ans << endl;
	return 0;
}
