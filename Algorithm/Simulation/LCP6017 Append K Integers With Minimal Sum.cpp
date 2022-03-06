/*
问题描述：P6017 力扣第283场周赛
给你一个整数数组 nums 和一个整数 k 。请你向 nums 中追加 k 个 未 出现在 nums 中的、互不相同 的 正 整数，并使结果数组的元素和 最小 。
返回追加到 nums 中的 k 个整数之和。
示例 1：
输入：nums = [1,4,25,10,25], k = 2
输出：5
解释：在该解法中，向数组中追加的两个互不相同且未出现的正整数是 2 和 3 。
nums 最终元素和为 1 + 4 + 25 + 10 + 25 + 2 + 3 = 70 ，这是所有情况中的最小值。
所以追加到数组中的两个整数之和是 2 + 3 = 5 ，所以返回 5 。
示例 2：
输入：nums = [5,6], k = 6
输出：25
解释：在该解法中，向数组中追加的两个互不相同且未出现的正整数是 1 、2 、3 、4 、7 和 8 。
nums 最终元素和为 5 + 6 + 1 + 2 + 3 + 4 + 7 + 8 = 36 ，这是所有情况中的最小值。
所以追加到数组中的两个整数之和是 1 + 2 + 3 + 4 + 7 + 8 = 25 ，所以返回 25 。
提示：
1 <= nums.length <= 105
1 <= nums[i], k <= 109
*/
class Solution {
public:
    long long sum(int l,int r)
    {
        return (long long)(l+r)*(long long )(r-l+1)/2;
    }
    long long minimalKSum(vector<int>& nums, int k) {
        long long ans=0;
        set<int> tmp;
        tmp.insert(nums.begin(),nums.end());
        nums.assign(tmp.begin(),tmp.end());
        int n=nums.size(),key=1;
        for(int i=0;i<n;i++)
        {
            if (k<=0) break;
            if (key==nums[i]||key>nums[i])
            {
                key=nums[i]+1;
                continue;
            }
            if (nums[i]-key+1<=k)
            {
                ans+=sum(key,nums[i]-1);
                k-=nums[i]-key;
                key=nums[i]+1;
            }
            else
            {
                ans+=sum(key,key+k-1);
                k=0;
            }
        }
        if (k>0) ans+=sum(nums[n-1]+1,nums[n-1]+k);
        return ans;
    }
};