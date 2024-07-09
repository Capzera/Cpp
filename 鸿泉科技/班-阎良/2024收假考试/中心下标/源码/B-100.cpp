#include <iostream>
#include <vector>
#include <cmath>
#include <climits> 
using namespace std;
int main() {
	int n, ans = INT_MAX;
	cin >> n;
	vector<long long> nums(n);
	long long total = 0, sum = 0;
	for (int i = 0; i < n; i++) {
		cin >> nums[i];
		total += nums[i];
	}
	int c = 2;
	for (int i = 0; i < n; i++) {
		if (total - nums[i] == c * 1ll * sum) {
			ans = min(ans, i);
		}
		sum += nums[i];
	}
	if (ans == INT_MAX) ans = -1;
	cout << ans << endl;
	return 0;
} 
