#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, k;
	cin >> n;
	cin >> k;
	vector<int> nums(n), ans(k), vis(n);
	for (auto& x : nums) {
		cin >> x;
	}
	sort(nums.begin(), nums.end());
	function<void(int, int)> dfs = [&](int index, int last) -> void {
		if (index == k) {
			for (auto& x : ans) cout << x << " ";
			cout << endl;
			return;
		}
		for (int i = last; i < n; i++) {
			if (vis[i] || (i && nums[i] == nums[i - 1] && !vis[i - 1])) continue;
			vis[i] = 1;
			ans[index] = nums[i];
			dfs(index + 1, i + 1);
			vis[i] = 0;
		}
	};
	dfs(0, 0);
	return 0;
}
