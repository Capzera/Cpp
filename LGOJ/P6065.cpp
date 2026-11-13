#include <bits/stdc++.h>
using namespace std;
const int p = 1e9;
int n;
long long dp[1000010];
int main() {
	cin >> n;
	vector<int> a;
	for (int now = 1; now <= n; now <<= 1) {
		a.push_back(now);
	}
	dp[0] = 1;
	for (int i = 0; i < a.size(); i++) {
		for (int j = a[i]; j <= n; j++) {
			dp[j] = (dp[j] + dp[j - a[i]]) % p;
		}
	}
	cout << dp[n];
	return 0;
}
