#include <bits/stdc++.h>
using namespace std;
const int N = 2e5 + 10, p = 1e9 + 7;
long long n, a, b, c, dp[N];
int main() {
	cin >> n >> a >> b >> c;
	dp[n] = 1;
	for (int i = n - 1; i >= c; i--) {
		dp[i] = ((i + a <= n ? dp[i + a] : 1) + (i + b <= n ? dp[i + b] : 1)) % p;
	}
	cout << dp[c];
	return 0;
}
