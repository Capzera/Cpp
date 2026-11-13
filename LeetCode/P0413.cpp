/*
问题描述：P0413
如果一个数列 至少有三个元素 ，并且任意两个相邻元素之差相同，则称该数列为等差数列。
例如，[1,3,5,7,9]、[7,7,7,7] 和 [3,-1,-5,-9] 都是等差数列。
给你一个整数数组 nums ，返回数组 nums 中所有为等差数组的 子数组 个数。
子数组 是数组中的一个连续序列。
示例 1：
输入：nums = [1,2,3,4]
输出：3
解释：nums 中有三个子等差数组：[1, 2, 3]、[2, 3, 4] 和 [1,2,3,4] 自身。
示例 2：
输入：nums = [1]
输出：0
提示：
1 <= nums.length <= 5000
-1000 <= nums[i] <= 1000
*/
//暴搜
class Solution {
public:
    int numberOfArithmeticSlices(vector<int>& nums) {
        int n=nums.size(),i,j,k,ans=0;
        if (n<3) return 0;
        for(i=n;i>2;i--)//暴搜，i控制窗口大小
        {
            for(j=0;j<n-i+1;j++)//窗口指针
            {
                int minus=nums[j+1]-nums[j],check=0;
                for(k=2;k<i;k++)//窗口内查询k+j为下标
                {
                    if (nums[k+j]-nums[k+j-1]!=minus)
                    {
                        check=1;
                        break;
                    }
                }
                if (!check)ans++;
            }
        }
        return ans;
    }
};
//动态规划
