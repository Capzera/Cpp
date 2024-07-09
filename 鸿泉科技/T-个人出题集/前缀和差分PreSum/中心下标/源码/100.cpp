#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, ans = INT_MAX;
	cin >> n;
	vector<long long> nums(n);
	for (auto& x : nums) cin >> x;
	long long total = accumulate(nums.begin(), nums.end(), 0LL), sum = 0;
	for (int i = 0; i < n; i++) {
		if (total - nums[i] == 2LL * sum) {
			ans = min(ans, i);
		}
		sum += nums[i];
	}
	if (ans == INT_MAX) ans = -1;
	cout << ans << endl;
	return 0;
} 
