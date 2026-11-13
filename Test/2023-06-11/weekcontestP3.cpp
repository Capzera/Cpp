#include <bits/stdc++.h>
using namespace std;
long long minCost(vector<int>& nums, int x) {
    int n = nums.size();
    long long ans = accumulate(nums.begin(), nums.end(), 0);
    deque<int> tmp(nums.begin(), nums.end());
    for (int i = 0; i < n - 1; i++) {
        tmp.push_back(tmp[0]); tmp.pop_front();
        for (int i = 0; i < n; i++) {
            nums[i] = min(nums[i], tmp[i]);
        }
        long long sum = 1LL * accumulate(nums.begin(), nums.end(), 0) + x * (i + 1);
        ans = min(ans, sum);
    }
    return ans;
}
int main() {
    int n, x;
    cin >> n;
    vector<int> nums(n);
    for (int i = 0; i < n; i++) cin >> nums[i];
    cin >> x;
    cout << minCost(nums, x) << endl;
    system("pause");
    return 0;
}
/*
20 1 15  x = 5
2 3 1 5 4  x = 1
1

20 1 15 20
3
20 1 15
5

3
1 2 3
4

6
15 150 56 69 214 203
42
15 + 57 + 57 + 57 + 57 + 57
dp[i] 表示以i结尾最小的答案
15 
*/