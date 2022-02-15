/*
题目描述：
    给你一个非负整数数组nums,你最初位于数组的第一个位置。
    数组中的每个元素代表你在该位置可以跳跃的最大长度。
    你的目标是使用最少的跳跃次数到达数组的最后一个位置。
    假设你总是可以到达数组的最后一个位置。
示例 1:
输入: 
    nums = [2,3,1,1,4]
输出:
    2
    解释: 跳到最后一个位置的最小跳跃数是 2。
    从下标为 0 跳到下标为 1 的位置,跳1步,然后跳3步到达数组的最后一个位置。
示例 2:
输入:
    nums = [2,3,0,1,4]
输出:
    2 
*/
class Solution {
public:
    int jump(vector<int>& nums) {
        int n=nums.size(),i,current=0,ans=0,farthest=0;
        for(i=0;i<n-1;i++)
        {
            if (i>=current)//current保存当前跳跃边界，如果当前遍历的位置大于，更新边界
            {
                current=max(farthest,i+nums[i]);//更新新的边界为上个边界内最远的跳跃距离
                ans++;//跳跃次数加一
            }
            else farthest=max(farthest,i+nums[i]);//在跳跃边界内寻找最远的跳跃距离
        }
        return ans;
    }
};