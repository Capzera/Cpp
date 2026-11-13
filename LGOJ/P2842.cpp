#include <bits/stdc++.h>
using namespace std;
long long n, V, a[1001];
int main() {
	cin >> n >> V;
	for (int i = 1; i <= n; i++) cin >> a[i];
	vector<int> dp(V + 1, 1e9);
	dp[0] = 0;
	for (int i = 1; i <= n; i++) {
		for (int j = a[i]; j <= V; j++) {
			dp[j] = min(dp[j], dp[j - a[i]] + 1);
		}
	}
	cout << dp[V];
	return 0;
}
