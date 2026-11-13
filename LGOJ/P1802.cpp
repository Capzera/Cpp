#include <bits/stdc++.h>
using namespace std;
const int N = 1e3 + 10, M = 1e3 + 10;
int n, m, w[N], cl[N], cw[N], dp[M];
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		cin >> cl[i] >> cw[i] >> w[i];
	}
	for (int i = 1; i <= n; i++) {
		for (int j = m; j >= w[i]; j--) {
			dp[j] = max(dp[j] + cl[i], dp[j - w[i]] + cw[i]);
		}
		for (int j = w[i] - 1; ~j; j--) {
			dp[j] += cl[i];
		}
	}
	cout << 5ll * dp[m];
	return 0;
}

