#include <bits/stdc++.h>
using namespace std;
int main() {
	freopen("4.in", "r", stdin);
	int n;
	cin >> n;
	vector<long long> nums(n);
	for (auto& x : nums) cin >> x;
	long long sum1 = 0, sum2 = 0;
	for (int i = 0; i < 2783; i++) {
		sum1 += nums[i];
	}
	for (int i = 2784; i < n; i++) {
		sum2 += nums[i];
	}
	cout << sum1 << " " << sum2 << endl;
	return 0;
}

