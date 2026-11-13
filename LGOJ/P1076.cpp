#include <bits/stdc++.h>
using namespace std;
long long n, m, ans, mod = 20123, a[10001][101], f[10001][101], len[10001];
int s;
int main() {
	cin >> n >> m;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cin >> f[i][j] >> a[i][j];
			if (f[i][j]) len[i]++;
		}
	}
	cin >> s;
	for (int i = 0; i < n; i++) {
		int x = a[i][s], p = x % len[i] + len[i];
		ans = (ans + x) % mod;
		s--;
		while (p) {
			s = (s + 1) % m;
			if (f[i][s]) p--;
		}
	}
	cout << ans;
	return 0;
}
