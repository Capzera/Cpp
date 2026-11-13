#include <bits/stdc++.h>
using namespace std;
vector<int> nums;
vector<string> ans;
int dfs(int i, int last) {
	if (i == 4) {
		return last == 24;
	}
	int a = last, b = nums[i];
	if (a < b) swap(a, b);
	if (dfs(i + 1, a + b)) {
		ans.emplace_back(to_string(a) + '+' + to_string(b) + "=" + to_string(a + b));
		return 1;
	}
	if (dfs(i + 1, a - b)) {
		ans.emplace_back(to_string(a) + '-' + to_string(b) + "=" + to_string(a - b));
		return 1;
	}
	if (dfs(i + 1, a * b)) {
		ans.emplace_back(to_string(a) + '*' + to_string(b) + "=" + to_string(a * b));
		return 1;
	}
	if (b && a % b == 0) {
		if (dfs(i + 1, a / b)) {
			ans.emplace_back(to_string(a) + '/' + to_string(b) + "=" + to_string(a / b));
			return 1;
		}
	}
	return 0;
}
bool check() {
	for (int l = 0; l < 2; l++) {
		for (int r = 0; r < 2; r++) {
			for (int m = 0; m < 2; m++) {
				string ch1, ch2, ch3;
				int ll, rr, ret;
				if (l == 0) {
					ch1 = "+";
					ll = nums[0] + nums[1];
				} else {
					ch1 = "-";
					ll = nums[0] - nums[1];
				}
				if (r == 0) {
					ch2 = "+";
					rr = nums[2] + nums[3];
				} else {
					ch2 = "-";
					rr = nums[2] - nums[3];
				}
				if (m == 0) {
					ch3 = "*";
					ret = ll * rr;
				} else if (m == 1 && rr && ll % rr == 0){
					ch1 = "/";
					ret = ll / rr;
				} else continue;
				if (ret == 24) {
					ans.emplace_back(to_string(ll) + ch3 + to_string(rr) + "=24");
					ans.emplace_back(to_string(nums[1]) + ch1 + to_string(nums[0]) + "=" + to_string(ll));
					ans.emplace_back(to_string(nums[2]) + ch2 + to_string(nums[3]) + "=" + to_string(rr));
					return 1;
				}
			}
		}
	}
	return 0;
}
int main() {
	nums = vector<int>(4);
	int flag = 0;
	for (auto& x : nums) cin >> x;
	sort(nums.begin(), nums.end());
	if (dfs(1, nums[0])) {
		for (int i = 2; i >= 0; i--) cout << ans[i] << endl;
		flag = 1;
	}
	while (! flag && next_permutation(nums.begin(), nums.end())) {
		ans = vector<string>(0);
		if (dfs(1, nums[0])) {
			for (int i = 2; i >= 0; i--) cout << ans[i] << endl;
			flag = 1;
			break;
		}
		if (check()) {
			for (int i = 2; i >= 0; i--) cout << ans[i] << endl;
			flag = 1;
			break;
		}
	}
	if (!flag) {
		cout << "No answer!" << endl;
	}
	return 0;
}
/*

9 7 5 7
*/
