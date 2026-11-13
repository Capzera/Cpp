/*
问题描述：P0047
给定一个可包含重复数字的序列 nums ，按任意顺序 返回所有不重复的全排列。
示例 1：
输入：nums = [1,1,2]
输出：
[[1,1,2],
 [1,2,1],
 [2,1,1]]
示例 2：
输入：nums = [1,2,3]
输出：[[1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]]
提示：
1 <= nums.length <= 8
-10 <= nums[i] <= 10
*/
class Solution {
public:
    vector<vector<int>> ans;
    vector<bool> check;
    vector<int> loc;
    int n;
    void dfs(vector<int>nums,vector<bool>check,vector<int>loc,int index)
    {
        if (index==n)
        {
            ans.push_back(loc);
            return;
        }
        for(int i=0;i<n;i++)
        {
            if (check[i]||(i>0&&nums[i]==nums[i-1]&&!check[i-1])) continue;
            check[i]=1;
            loc[index]=nums[i];
            dfs(nums,check,loc,index+1);
            check[i]=0;
        }
    }
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        this->n=nums.size();
        this->check=vector<bool>(n);
        this->loc=vector<int>(n);
        sort(nums.begin(),nums.end());
        dfs(nums,check,loc,0);
        return ans;
    }
};