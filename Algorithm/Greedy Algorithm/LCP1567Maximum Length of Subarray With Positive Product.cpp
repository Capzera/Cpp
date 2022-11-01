/*
问题描述：
给你一个整数数组 nums?，请你求出乘积为正数的最长子数组的长度。
一个数组的子数组是由原数组中零个或者更多个连续数字组成的数组。
请你返回乘积为正数的最长子数组长度。
示例? 1：
输入：nums = [1,-2,-3,4]
输出：4
解释：数组本身乘积就是正数，值为 24 。
示例 2：
输入：nums = [0,1,-2,-3,-4]
输出：3
解释：最长乘积为正数的子数组为 [1,-2,-3] ，乘积为 6 。
注意，我们不能把 0 也包括到子数组中，因为这样乘积为 0 ，不是正数。
示例 3：
输入：nums = [-1,-2,-3,0,1]
输出：2
解释：乘积为正数的最长子数组是 [-1,-2] 或者 [-2,-3] 。
*/
class Solution {
public:
    int getMaxLen(vector<int>& nums) {
        int n=nums.size(),i=0,ans=0,minus=0,left=n,right=-1;//贪心算法
        while (nums[i]==0) 
        {i++;if (i==n) return 0;}//去除前导0
        while (nums[n-1]==0) n--;//去除末尾0
        nums.resize(n+1);nums[n]=0;//补末尾0防止出错
        int last=i;
        for(;i<=n;i++)
        {
            if (nums[i]==0)//找到0为分隔符
            {
                if (minus%2==0) ans=max(ans,i-last);//区间内有偶数个负数，直接计算区间长度
                else ans=max(ans,max(right-last,i-1-left));
                last=i+1;minus=0;//下一个起始点为0后
                right=i;left=n;//重置指针
                continue;
            }
            if (nums[i]<0)
            {
                minus++;
                left=min(left,i);
                right=max(right,i);
            }
        }
        return ans;
    }
};