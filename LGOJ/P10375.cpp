#include <bits/stdc++.h>
using namespace std;
int n, m, mod = 1e9 + 7;
long long f[3001][3001][2], ans;
int main() {
	cin >> n >> m;
	f[1][1][0] = m;
	for (int i = 2; i <= n; i++) {
		for (int j = 1; j <= i; j++) {
			f[i][j + 1][0] = (f[i][j + 1][0] + f[i - 1][j][1] * (m - j)) % mod;
			f[i][j][1] = ((f[i][j][1] + f[i - 1][j][0] + f[i - 1][j][1]) % mod * j) % mod;
			f[i][j][0] = (f[i][j][0] + f[i - 1][j][0] * (m - j)) % mod;
		} 
	}
	for (int i = 1; i <= n; i++) ans = (ans + f[n][i][1]) % mod;
	cout << ans;
	return 0;
}
