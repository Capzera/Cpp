#include <bits/stdc++.h>
using namespace std;
    bool Point24(vector<int>& nums) {
        function<bool(int, double)> dfs = [&] (int idx, double sum) -> bool {
            if (idx == 3) {
                return fabs(sum - 24) < 1e-6;
            }
            if (dfs(i, sum + nums[idx + 1])) return true;
            if (dfs(i, sum * nums[idx + 1])) return true;
            if (dfs(i, sum - nums[idx + 1])) return true;
            if (dfs(idx + 1, 1.0 * sum / nums[idx + 1])) return true;
            return false;
        };
        while (next_permutation(nums.begin(), nums.end())) {
            if (dfs(1, nums[0])) return true;
        }
        return false;
    }
int main() {
    vector<int> nums(4);
    for (auto& x : nums) cin >> x;
    if (Point24(nums)) cout << "true" ;
    else cout << "false";
    system("pause");
    return 0;
}