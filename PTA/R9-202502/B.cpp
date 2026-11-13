#include <bits/stdc++.h>
using namespace std;
const int N = 2e5 + 10;
int n, ans = INT_MIN, x, dp;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> x;
		dp = x + max(dp, 0);
		ans = max(ans, dp);
	}
	cout << ans;
	return 0;
}
