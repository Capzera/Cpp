#include <bits/stdc++.h>
using namespace std;
const int N = 110, M = 5e3 + 10;
int n, m, a, b, x, t[N], w[N], c[N], dp[M];
int main() {
	cin >> a >> b >> m >> n;
	for (int i = 1; i <= n; i++) {
		cin >> x;
		t[i] = x * (b / a);
	}
	for (int i = 1; i <= m; i++) {
		cin >> x >> c[i];
		w[i] = t[x];
	}
	
	
	
	
	
	
	
	cin >> x;
	for (int i = 1; i <= m; i++) {
		for (int j = x; j >= w[i]; j--) {
			dp[j] = max(dp[j], dp[j - w[i]] + c[i]);
		}
	}
	cout << dp[x];
	return 0;
}
