#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, m, l, r;
	cin >> n >> m;
	vector<int> nums(n);
	for (auto& x : nums) cin >> x;
	while (m--) {
		long long ans = 0;
		cin >> l >> r;
		for (int i = l - 1; i < r; i++) {
			ans += nums[i];
		}
		cout << ans << endl;
	}
	return 0;
}
