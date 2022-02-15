/*
题目描述：
    给定一个长度为n的环形整数数组nums，返回nums的非空子数组的最大可能和?。
    环形数组?意味着数组的末端将会与开头相连呈环状。
        形式上，nums[i] 的下一个元素是nums[(i+1)%n],nums[i]的前一个元素是nums[(i-1+n)%n] 。
    子数组最多只能包含固定缓冲区nums中的每个元素一次。
        形式上，对于子数组nums[i],nums[i+1],...,nums[j],不存在i<=k1,k2<=j其中k1%n==k2%n?。
示例 1：
输入：
    nums = [1,-2,3,-2]
输出：
    3
解释：
    从子数组 [3] 得到最大和 3
示例 2：
输入：
    nums = [5,-3,5]
输出：
    10
    解释：从子数组 [5,5] 得到最大和 5 + 5 = 10
示例 3：
输入：
    nums = [3,-2,2,-3]
输出：
    3
解释：从子数组 [3] 和 [3,-2,2] 都可以得到最大和 3
*/
class Solution {
public:
    int maxSubarraySumCircular(vector<int>& nums) {
        int n=nums.size(),i,sum=0,ans1=INT_MIN,ans2=INT_MAX,SUM=0;
        for(i=0;i<n;i++)
        {
            SUM+=nums[i];
            sum+=nums[i];
            if (nums[i]>=sum) sum=nums[i];
            ans1=max(ans1,sum);
        }
        sum=0;
        for(i=0;i<n;i++)
        {
            sum+=nums[i];
            if (nums[i]<=sum) sum=nums[i];
            ans2=min(ans2,sum);
        }
        if (SUM==ans2) return ans1;
        return max(ans1,SUM-ans2);
    }
};