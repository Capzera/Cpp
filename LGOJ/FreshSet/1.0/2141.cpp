#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, ans = 0;
    cin >> n;
    vector<int> nums(n);
    unordered_set<int> mp;
    for (auto& x : nums) {
        cin >> x;
        mp.emplace(x);
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (nums[i] - nums[j] == nums[j]) continue;
            if (!mp.count(nums[i] - nums[j])) continue;
            ans++;
            break;
        }
    }
    cout << ans << endl;
    system("pause");
    return 0;
}