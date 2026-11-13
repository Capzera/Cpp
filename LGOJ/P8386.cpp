#include <bits/stdc++.h>
using namespace std;
int n, m, mod = 1e9 + 7;
long long ans, f[3001][2];
int main() {
	cin >> n >> m;
	f[1][0] = m;
	for (int i = 2; i <= n; i++) {
		for (int j = i; j > 0; j--) {
			f[j + 1][0] = (f[j + 1][0] + f[j][1] * (m - j)) % mod;
			f[j][1] = ((f[j][1] + f[j][0]) * j) % mod;
			f[j][0] = (f[j][0] * (m - j)) % mod;
		} 
	}
	for (int i = 1; i <= n; i++) ans = (ans + f[i][1]) % mod;
	cout << ans;
	return 0;
}
