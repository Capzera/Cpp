#include <bits/stdc++.h>
using namespace std;
const int N = 1e2 + 10, M = 4e4 + 10;
int n, m, a, b, k, dp[M];
int main() {
	cin >> n >> m;
	vector<int> w, c;
	for (int i = 1; i <= n; i++) {
		cin >> a >> b >> k;
		int j = 1;
		for (int j = 1; j <= k; k -= j, j <<= 1){
			w.push_back(j * b), c.push_back(j * a);
		}
		if (k) w.push_back(k * b), c.push_back(k * a);
	}
	for (int i = 0; i < w.size(); i++) {
		for (int j = m; j >= w[i]; j--) {
			dp[j] = max(dp[j], dp[j - w[i]] + c[i]);
		}
	}
	cout << dp[m];
	return 0;
}
