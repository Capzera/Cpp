#include <bits/stdc++.h>
using namespace std;
int n, ans, dp[10];
string s;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> s;
		int a = s[0] - '0', b = s.back() - '0';
		dp[b] = max(dp[b], dp[a] + 1);
		ans = max(ans, dp[b]);
	}
	cout << n - ans;
	return 0;
}
