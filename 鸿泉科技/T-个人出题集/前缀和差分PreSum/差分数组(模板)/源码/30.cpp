#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, m, l, r, x;
	cin >> n >> m;
	vector<long long> nums(n);
	for (auto& x : nums) cin >> x;
	while (m--) {
		cin >> l >> r >> x;
		for (int i = l - 1; i < r; i++) nums[i] += x;
	}
	for (auto& x : nums) cout << x << " ";
	return 0;
}
