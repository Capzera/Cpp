#include <bits/stdc++.h>
using namespace std;
long long nums[200001], n, k, ans;
int main() {
    cin >> n >> k;
    for (int i = 0; i < n; i++) cin >> nums[i];
    sort(nums, nums + n);
    for (int i = 0; i < n; i++) {
        ans += upper_bound(nums, nums + n, nums[i] + k) - lower_bound(nums, nums + n, nums[i] + k);
    }
    cout << ans << endl;
    system("pause");
    return 0;
}