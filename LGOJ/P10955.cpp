#include <bits/stdc++.h>
using namespace std;
const long long p = 1ll << 31, N = 4010;
long long n, dp[N];
int main() {
	cin >> n;
	dp[0] = 1;
	for (int i = 1; i <= n; i++) {
		for (int j = i; j <= n; j++) {
			dp[j] = (dp[j] + dp[j - i]) % p;
		}
	}
	cout << (dp[n] - 1 + p) % p;
	return 0;
}
