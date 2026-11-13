#include <bits/stdc++.h>
using namespace std;
int V, n, c, w, v;
int main() {
	cin >> V >> n >> c;
	vector<int> dp(c + 1);
	for (int i = 1; i <= n; i++) {
		cin >> v >> w;
		for (int j = c; j >= w; j--) {
			dp[j] = max(dp[j], dp[j - w] + v);
		}
	}
	if (dp[c] < V) {
		cout << "Impossible";
		return 0;
	}
	for (int j = 0; j <= c; j++) {
		if (dp[j] >= V) {
			cout << c - j;
			break;
		}
	}
	return 0;
}
