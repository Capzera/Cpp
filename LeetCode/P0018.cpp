/*
问题描述：P0018
给你一个由 n 个整数组成的数组?nums ，和一个目标值 target 。
请你找出并返回满足下述全部条件且不重复的四元组?[nums[a], nums[b], nums[c], nums[d]]?
（若两个四元组元素一一对应，则认为两个四元组重复）：
0 <= a, b, c, d?< n
a、b、c 和 d 互不相同
nums[a] + nums[b] + nums[c] + nums[d] == target
你可以按 任意顺序 返回答案 。
示例 1：
输入：nums = [1,0,-1,0,-2,2], target = 0
输出：[[-2,-1,1,2],[-2,0,0,2],[-1,0,0,1]]
示例 2：
输入：nums = [2,2,2,2,2], target = 8
输出：[[2,2,2,2]]
提示：
1 <= nums.length <= 200
-109 <= nums[i] <= 109
-109 <= target <= 109
*/
class Solution {
public:
    vector<vector<int>> fourSum(vector<int>& nums, int target) {
        int n=nums.size();
        vector<vector<int>> ans;
        vector<int> tmp(4);
        if (n<4) return ans;
        sort(nums.begin(),nums.end());
        for(int i=0;i<n-3;i++)
        {
            if (i&&nums[i]==nums[i-1]) continue;//防止重复的i被算入答案
            if ((long)nums[i]+nums[i+1]+nums[i+2]+nums[i+3]>target) break;//剪枝，如果该i最大和大于target
            if ((long)nums[i]+nums[n-1]+nums[n-2]+nums[n-3]<target) continue;//剪枝，如果该i最小和小于target
            for(int j=i+1;j<n-2;j++)
            {
                if (j>i+1&&nums[j]==nums[j-1]) continue;
                if ((long)nums[i]+nums[j]+nums[j+1]+nums[j+2]>target) break;//剪枝，如果该j最大和大于target
                if ((long)nums[i]+nums[j]+nums[n-2]+nums[n-1]<target) continue;//剪枝，如果该j最小和小于target
                int x=j+1,y=n-1;//赋左右指针初值
                while (x<y)
                {
                    int tot=nums[i]+nums[j]+nums[x]+nums[y];//保存当前的四数之和
                    if (tot==target)//找到答案
                    {
                        ans.push_back({nums[i],nums[j],nums[x],nums[y]});
                        while (x<y&&nums[x]==nums[x+1]) x++;//防止因左指针重复数重复答案
                        x++;
                        while (x<y&&nums[y]==nums[y-1]) y--;//防止因右指针重复数重复答案
                        y--;
                    }
                    else if (tot>target) y--;//和较大，左移右指针
                    else x++;//和较小，右移左指针
                }
            }
        }
        return ans;
    }
};