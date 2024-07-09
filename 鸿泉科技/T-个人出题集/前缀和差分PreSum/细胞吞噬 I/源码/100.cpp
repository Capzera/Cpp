#include <bits/stdc++.h>
using namespace std;
int main() {
	long long n, k, x, ans = 0;
	cin >> n >> k;
	vector<long long> sum(n + 1);
	for (int i = 1; i <= n; i++) {
		cin >> x;
		sum[i] = sum[i - 1] + x;
		if (i >= k - 1) ans = max(ans, sum[i] - sum[i - k - 1]);
	}
	cout << ans;
	return 0;
}
