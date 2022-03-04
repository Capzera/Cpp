/*
问题描述：P0016
给你一个长度为 n 的整数数组?nums?和 一个目标值?target。请你从 nums 中选出三个整数，使它们的和与?target?最接近。
返回这三个数的和。
假定每组输入只存在恰好一个解。
示例 1：
输入：nums = [-1,2,1,-4], target = 1
输出：2
解释：与 target 最接近的和是 2 (-1 + 2 + 1 = 2) 。
示例 2：
输入：nums = [0,0,0], target = 1
输出：0
提示：
3 <= nums.length <= 1000
-1000 <= nums[i] <= 1000
-104 <= target <= 104
*/
class Solution {
public:
    int threeSumClosest(vector<int>& nums, int target) {
        sort(nums.begin(),nums.end());
        int n=nums.size(),ans=INT_MAX,distance=ans,sum;
        if (n==3) return nums[0]+nums[1]+nums[2];
        for(int pA=0;pA<n-2;pA++)
        {
            int pB=pA+1,pC=n-1;
            while (pB<pC)
            {
                sum=nums[pA]+nums[pB]+nums[pC];
                if (sum==target) return sum;
                if (sum<target) pB++;
                else pC--;
                if (abs(sum-target)<distance) 
                {
                    distance=(abs(sum-target));
                    ans=sum;
                }
            }
        }
        return ans;
    }
};