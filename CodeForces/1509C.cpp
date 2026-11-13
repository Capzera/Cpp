#include <bits/stdc++.h>
using namespace std;
const int N = 2e3 + 5;
int n, a[N];
long long dp[N][N];
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	sort(a + 1, a + n + 1);
	for (int len = 2; len <= n; len++) {
		for (int i = 1; i + len - 1 <= n; i++) {
			int j = i + len - 1;
			dp[i][j] = min(dp[i + 1][j], dp[i][j - 1]) + a[j] - a[i];
		}
	} 
	cout << dp[1][n];
	return 0;
}
//https://codeforces.com/problemset/problem/1509/C
