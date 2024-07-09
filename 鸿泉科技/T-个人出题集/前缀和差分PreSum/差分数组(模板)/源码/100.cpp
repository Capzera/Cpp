#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, m, l, r, x;
	cin >> n;
	cin >> m;
	vector<long long> nums(n), diff(n + 2);
	for (auto& x : nums) cin >> x;
	diff[1] = nums[0];
	for (int i = 1; i < n; i++) {
		diff[i + 1] = nums[i] - nums[i - 1];
	}
	while (m--) {
		cin >> l;
		cin >> r;
		cin >> x;
		diff[l] += x, diff[r + 1] -= x;
	}
	for (int i = 1; i <= n; i++) {
		diff[i] += diff[i - 1];
		cout << diff[i] << " ";
	}
	return 0;
}
