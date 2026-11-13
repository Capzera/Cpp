#include <bits/stdc++.h>
using namespace std;
int main() {
    long long n, m, sum = 0, diff = 0;
    cin >> n >> m;
    vector<long long> nums(n);
    for (auto& x : nums) cin >> x;
    sort(nums.begin(), nums.end());
    for (int i = 1; i < n - 1; i++) {
        sum += nums[i];
        diff += nums[n - 1] - nums[i];
    }
    if (m <= diff) cout << sum + m << endl;
    else {
        m -= diff + 1;
        cout << sum + diff + m / (n - 1) * (n - 2) + m % (n - 1) << endl;
    }
    return 0;
}