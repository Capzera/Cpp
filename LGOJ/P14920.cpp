#include <bits/stdc++.h>
using namespace std;
const int N = 501;
long long n, V, tot, w[N], c[N], dp[250001];
int main() {
	memset(dp, 0x3f, sizeof(dp));
	dp[0] = 0;
	cin >> n >> V;
	for (int i = 1; i <= n; i++) {
		cin >> c[i] >> w[i];
		tot += c[i];
	}
	for (int i = 1; i <= n; i++) {
		for (int j = tot; j >= c[i]; j--) {
			dp[j] = min(dp[j], dp[j - c[i]] + w[i]);
		}
	}
	for (int i = tot; ~i; i--) {
		if (dp[i] > V) continue;
		cout << i << endl;
		break;
	}
	return 0;
}
