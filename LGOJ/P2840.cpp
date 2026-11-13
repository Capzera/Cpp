#include <bits/stdc++.h>
using namespace std;
const int p = 1e9 + 7;
long long n, V, a[1001];
int main() {
	cin >> n >> V;
	for (int i = 1; i <= n; i++) cin >> a[i];
	vector<int> dp(V + 1);
	dp[0] = 1;
	for (int j = 1; j <= V; j++) {
		for (int i = 1; i <= n; i++) {
			if (j < a[i]) continue;
			dp[j] = (dp[j] + dp[j - a[i]]) % p;
		}
	}
	cout << dp[V];
	return 0;
}
