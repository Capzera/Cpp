#include <bits/stdc++.h>
using namespace std;
long long n, m, dp[90][90], mod = 1e9 + 7;
vector<long long> fac(81), invfac(81);
long long qpow(long long x, int p) {
	long long ans = 1;
	while (p) {
		if (p & 1) {
			ans = ans * x % mod;
		}
		x = x * x % mod;
		p >>= 1;
	}
	return ans;
}
void init(int n) {
	fac[0] = 1;
	for (int i = 1; i < n; i++) {
		fac[i] = fac[i - 1] * i % mod;
	}
	invfac[n - 1] = qpow(fac[n - 1], mod - 2);
	for (int i = n - 2; i >= 0; i--) {
		invfac[i] = invfac[i + 1] * (i + 1) % mod;
	}
}

long long C(int n, int m) {
	return fac[n] * invfac[m] % mod * invfac[n - m] % mod;
}
int main() {
	cin >> n >> m;
	init(81);
	for (int j = 1; j <= m; j++) dp[1][j] = j;
	if (m == 1) {
		long long ans = -C(n, 1);
		for (int i = 1; i <= n; i++) {
			cout << C(n, i) << endl;
			ans = (ans + C(n, i) * 2) % mod;
		}
		cout << ans;
		return 0;
	}
	return 0;
}
/*
C(3, 1) * 1 + C(3, 2) * 2 + C(3, 3) * 2
设DP[i][j] 第i行第j列作为子树的数目
dp[1][1] = 1  dp[1][2] = 2;  dp[1][3] = 
dp[2][1] = 3; dp[2][2] = 10;
*/
