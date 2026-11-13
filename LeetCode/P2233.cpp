/*
问题描述：P2233 力扣第288场周赛
给你一个非负整数数组?nums?和一个整数?k?。每次操作，你可以选择?nums?中 任一?元素并将它 增加?1?。
请你返回 至多?k?次操作后，能得到的?nums的?最大乘积?。由于答案可能很大，请你将答案对?109 + 7?取余后返回。
示例 1：
输入：nums = [0,4], k = 5
输出：20
解释：将第一个数增加 5 次。
得到 nums = [5, 4] ，乘积为 5 * 4 = 20 。
可以证明 20 是能得到的最大乘积，所以我们返回 20 。
存在其他增加 nums 的方法，也能得到最大乘积。
示例 2：
输入：nums = [6,3,3,2], k = 2
输出：216
解释：将第二个数增加 1 次，将第四个数增加 1 次。
得到 nums = [6, 4, 3, 3] ，乘积为 6 * 4 * 3 * 3 = 216 。
可以证明 216 是能得到的最大乘积，所以我们返回 216 。
存在其他增加 nums 的方法，也能得到最大乘积。
提示：
1 <= nums.length, k <= 105
0 <= nums[i] <= 106
*/
class Solution {
public:
    int maximumProduct(vector<int>& nums, int k) {
        priority_queue<int,vector<int>,greater<>> q;
        int n=nums.size();
        long long ans=1;
        for (int i:nums) q.push(i);
        while (k--) {
            int t=q.top()+1;
            q.pop();
            q.push(t);
        }
        while (q.size()){
            ans=(ans*q.top())%1000000007;
            q.pop();
        }
        return ans;
    }
};