#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
int n, ans, dp[N], a[N];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= n; i++) {
		if (a[i] > dp[ans]) {
			dp[++ans] = a[i];
			continue;
		}
		*lower_bound(dp + 1, dp + ans + 1, a[i]) = a[i];
	}
	cout << n - ans;
	return 0;
}
