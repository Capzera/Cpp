#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, m, num;
    cin >> n >> m;
    vector<int> nums(n + 1), vec;
    for (int i = 1; i <= n; i++) cin >> nums[i];
    while (m--) {
        cin >> num;
        int ans = 0;
        int loc = lower_bound(nums.begin(), nums.end(), num) - nums.begin();
        for (auto& x : vec) {
            if (x < loc) ans++;
        }
        vec.emplace_back(loc);
        cout << loc - ans << endl;
    }
    system("pause");
    return 0;
}