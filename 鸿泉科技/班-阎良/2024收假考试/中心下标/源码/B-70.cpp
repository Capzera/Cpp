#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
using namespace std;
int main() {
	int n;
	cin >> n;
	vector<int> nums(n);
	for (int i = 0; i < n; i++) {
		cin >> nums[i];
	}
	for (int i = 0; i < n ; i++) {
		int sum1 = 0, sum2 = 0;
		for (int j = 0; j < i; j++) {
			sum1 += nums[j];
		}
		for (int j = i + 1; j < n; j++) {
			sum2 += nums[j];
		}
		if (sum1 == sum2) {
			cout << i << endl;
			return 0;
		}
	}
	cout << -1 << endl;
	return 0;
}

