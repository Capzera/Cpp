#include <bits/stdc++.h>
using namespace std;
int main() {
	set<int> s;
	int n;
	cin >> n;
	vector<int> nums(n);
	for (int i = 0; i < n; i++) {
		cin >> nums[i];
	}
	for (int i = 1; i < n; i++) {
		s.emplace(abs(nums[i] - nums[i - 1]));
	}
	for (int i = 1; i <= n - 1; i++) {
		if (!s.count(i)) {
			cout << "Not jolly";
			return 0;
		}
	}
	cout << "Jolly";
	return 0;
}
