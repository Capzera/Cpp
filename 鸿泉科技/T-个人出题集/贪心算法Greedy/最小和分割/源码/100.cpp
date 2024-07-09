#include <bits/stdc++.h>
using namespace std;
int main() {
	string str;
	cin >> str;
	vector<int> nums(2);
	sort(str.begin(), str.end());
	for (int i = 0; i < str.size(); i++) {
		nums[i & 1 ^ 1] = nums[i & 1 ^ 1] * 10 + str[i] - '0';
	}
	cout << nums[0] + nums[1] << endl;
	return 0;
}
