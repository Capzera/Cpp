#include <bits/stdc++.h>
using namespace std;
int nums[100], ans[100], vis[100];
int n;
void dfs(int i) {
	if (i == n) {
		for (int i = 0; i < n; i++) {
			cout << ans[i] << " ";
		}
		cout << endl;
		return;
	}
	for (int j = 0; j < n; j++) {
		if (vis[j] || (j && nums[j] == nums[j - 1] && !vis[j - 1])) continue;
		vis[j] = 1;
		ans[i] = nums[j];
		dfs(i + 1);
		vis[j] = 0;
	}
}
int main() {
	cin >> n;
	for (int i = 0; i < n; i++) cin >> nums[i];
	sort(nums, nums + n);
	dfs(0);
	return 0;
}
