#include <bits/stdc++.h>
using namespace std;
int n, h, w[101], v[101];
int main() {
	cin >> n >> h;
	for (int i = 1; i <= n; i++) {
		cin >> v[i] >> w[i];
	}
	int V = (h + v[1] - 1) / v[1] * w[1];
	vector<int> dp(V + 1);
	for (int i = 1; i <= n; i++) {
		for (int j = w[i]; j <= V; j++) {
			dp[j] = max(dp[j], dp[j - w[i]] + v[i]);
		}
	}
	for (int j = 0; j <= V; j++) {
		if (dp[j] >= h) {
			cout << j;
			return 0;
		}
	}
}
