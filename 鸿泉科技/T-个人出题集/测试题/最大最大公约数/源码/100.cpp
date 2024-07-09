#include <bits/stdc++.h>
using namespace std;
int main() {
	int n;
	cin >> n;
	vector<long long> nums(n); 
	long long ans = 0;
	for (auto& x : nums) cin >> x;
	for (int i = 1; i < n; i++) {
		ans = max(ans, __gcd(nums[i], nums[i - 1])); 
	}
	cout << ans << endl;
	return 0;
}
