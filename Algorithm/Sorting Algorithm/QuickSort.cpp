#include<bits/stdc++.h>
using namespace std;
void QSort(vector<int>& nums, int l, int r) {
    if (l >= r) return;
    int i = l, j = r, x = nums[l + (r - l) / 2];
    while (i <= j) {
        while (nums[i] < x) i++;
        while (nums[j] > x) j--;
        if (i <= j) swap(nums[i++], nums[j--]);
    }
    QSort(nums, i, r), QSort(nums, l, j);
}
int main() {
    int n;
    cin >> n;
    vector<int> nums(n);
    for (auto& x : nums) cin >> x;
    QSort(nums, 0, n - 1);
    for (auto& x : nums) cout<< x << " ";
    system("pause");
    return 0;
}