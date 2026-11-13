#pragma GCC optimize(2)
#include <bits/stdc++.h>
using namespace std;
void solve() {
	long long n, x, sum = 0;
	cin >> n >> x;
	vector<int> c(n), h(n);
	for (int i = 0; i < n; i++) {
		cin >> c[i] >> h[i];
		sum += h[i];
	}
	vector<long long> dp(sum + 1, -1); //dp[i][j] 表示 前i个元素，获得幸福度为j时最多还剩多少钱	
	if (c[0]) dp[0] = x;
	else dp[h[0]] = x;
	for (int i = 1; i < n; i++) {
		for (int j = sum; j >= 0; j--) {
			if (dp[j] == -1) continue;
			if (dp[j] >= c[i] && j + h[i] <= sum) {
				dp[j + h[i]] = max(dp[j + h[i]], dp[j] - c[i] + x);
			}
			dp[j] += x;
		}
	}
	for (int i = sum; i >= 0; i--) {
		if (dp[i] == -1) continue;
		cout << i << endl;
		break;
	}
} 
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	int _t = 1;
	cin >> _t;
	while (_t--) {
		solve();
	}
	return 0;
}
