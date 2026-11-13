#include <bits/stdc++.h>
using namespace std;
int main() {
	int n;
	vector<int> a(n);
	for(auto& x : a) cin >> a;
	vector<long long> dp(n* 2);
	dp[0] = 0, dp[1] = 0;
	for (int i = 1; i < n; i++) {
		if (a[i] - a[i - 1] == 1) dp[i] = dp[i - 1] + 1;
		else {
			int step = log(a[i]) / log(2) + 1;
			if (step > a[i])
		}
	}
	return 0;
}
