#include <bits/stdc++.h>
using namespace std;
const int N = 210;
int n, a[N][N], dp[N];


int main() {
	cin >> n;
	for (int i = 1; i < n; i++) {
		for (int j = i + 1; j <= n; j++) {
			cin >> a[i][j];
		}
		dp[i] = 1e9;
	}
	for (int i = n - 1; i; i--) {
		for (int j = i + 1; j <= n; j++) {
			dp[i] = min(dp[i], a[i][j] + dp[j]);
		}
	}
	cout << dp[1];
	return 0;
}
