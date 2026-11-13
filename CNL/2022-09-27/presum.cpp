#include <bits/stdc++.h>

using namespace std;

int main(){
    int n;
    cin>> n;
    vector<int> nums(n), sum(n + 1);
    for (int i = 0; i < n; i++) {
        cin >> nums[i];
        sum[i + 1] = sum[i] + nums[i];
    }
    for (int i = 0; i <= n; i++) cout << sum[i] << " ";
    system("pause");
    return 0;
}
//sum(l, r) = sum[r + 1] - sum[l]






























/*
P303
class Solution {
public:
    int minSubArrayLen(int target, vector<int>& nums) {
        int n = nums.size();
        vector<int> sum(n + 1);
        for (int i = 0; i < n; i++) 
            sum[i + 1] = sum[i] + nums[i];//sum(l, r) = sum[r + 1] - sum[l];
        if (sum.back() < target) return 0;
        int l = 0, r = n + 1;
        while (l < r) {
            int mid = l + (r - l) / 2, flag = false;
            for (int i = 0; i <n - mid + 1; i++) {
                if (sum[i + mid] - sum[i] < target) continue;
                flag = 1;
                break;
            }
            if (!flag)
                l = mid + 1;
            else r = mid;
        }
        return r;
    }
};
*/