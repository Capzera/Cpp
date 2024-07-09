#include <bits/stdc++.h>
using namespace std;
int main() {
	int n;
	cin >> n;
	vector<int> nums(n);
	for (auto& x : nums) cin >> x;
	for (int i = 0; i < n ; i++) {
		int sum = 0, sum2 = 0;
		for (int j = 0; j < i; j++) {
			sum += nums[j];
		}
		for (int j = i + 1; j < n; j++) {
			sum2 += nums[j];
		}
		if (sum == sum2) {
			cout << i << endl;
			return 0;
		}
	}
	cout << -1 << endl;
	return 0;
}

