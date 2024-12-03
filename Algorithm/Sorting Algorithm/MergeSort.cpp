#include <bits/stdc++.h>
using namespace std;
void MergeSort(vector<int>& nums, int l, int r) {
	if (l >= r) return;
	int mid = l + (r - l) / 2;
	MergeSort(nums, l, mid);
	MergeSort(nums, mid + 1, r);
	vector<int> vec(r - l + 1);
	int i = l, j = mid + 1, k = 0;
	while (i <= mid && j <= r) {
		if (nums[i] <= nums[j]) vec[k++] = nums[i++];
		else vec[k++] = nums[j++];
	}
	while (i <= mid) vec[k++] = nums[i++];
	while (j <= r) vec[k++] = nums[j++];
	copy(vec.begin(), vec.end(), nums.begin() + l);
}
int main() {
	int n;
	cin >> n;
	vector<int> nums(n);
	for (auto& x : nums) cin >> x;
	MergeSort(nums, 0, n - 1);
	for (auto& x : nums) cout << x << " ";
	return 0;
}
/*
1 2 3 4 5 6 7 8 9 10 11 12
(b - 1) * a - b
3x - 7
21 3 * 7 + 7 * 0
22 3 * 5 + 7 * 1
23 3 * 3 + 7 * 2
24 3 * 1 + 7 * 3
25 3 * 6 + 7 * 1
26 3 * 4 + 7 * 2
27 3 * 5 + 7 * 3
*/
