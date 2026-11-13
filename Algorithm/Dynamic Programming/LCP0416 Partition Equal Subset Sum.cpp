/*
问题描述：P0416
给你一个 只包含正整数 的 非空 数组?nums 。请你判断是否可以将这个数组分割成两个子集，使得两个子集的元素和相等。
示例 1：
输入：nums = [1,5,11,5]
输出：true
解释：数组可以分割成 [1, 5, 5] 和 [11] 。
示例 2：
输入：nums = [1,2,3,5]
输出：false
解释：数组不能分割成两个元素和相等的子集。
提示：
1 <= nums.length <= 200
1 <= nums[i] <= 100
*/
class Solution {
public:
    bool canPartition(vector<int>& nums) {
		int sum=0;
        for (int i:nums) sum+=i;
        if (sum%2) return 0;
        sum/=2;
        vector<bool> dp(sum+1);
        dp[0]=true;
        for (int num:nums){
			for (int j=sum;j>=num;j--)
				if (dp[j-num]) dp[j]=1;
		}
        return dp[sum];
    }
};