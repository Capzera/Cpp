#include <bits/stdc++.h>
using namespace std;
long long x, s[4], c1, c2, ans;
int main() {
	for (int i = 0; i < 4; i++) cin >> s[i];
	for (int t = 0; t < 4; t++) {
		int sum = 0;
		vector<int> a(s[t]);
		for (auto& x : a) cin >> x, sum += x;
		vector<int> dp(sum + 1);
		for (auto& w : a)
			for (int j = sum / 2; j >= w; j--)
				dp[j] = max(dp[j], dp[j - w] + w);
		ans += sum - dp[sum / 2];
	}
	cout << ans;
	return 0;
}
/*

*/
