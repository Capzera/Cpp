#include <bits/stdc++.h>
using namespace std;
long long n, k, x, total, ans;
int main() {
	cin >> n >> k;
	vector<long long> sum(n);
	for (int i = 1; i < n; i++) {
		cin >> x;
		total += x;
		sum[i] = sum[i - 1] + x;
	}
	for (int i = k; i < n; i++) {
		ans = max(ans, sum[i] - sum[i - k]);
	}
	cout << total - ans;
	return 0;
}
