/*
问题描述：P0046
给定一个不含重复数字的数组 nums ，返回其 所有可能的全排列 。你可以 按任意顺序 返回答案。
示例 1：
输入：nums = [1,2,3]
输出：[[1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]]
示例 2：
输入：nums = [0,1]
输出：[[0,1],[1,0]]
示例 3：
输入：nums = [1]
输出：[[1]]
提示：
1 <= nums.length <= 6
-10 <= nums[i] <= 10
nums 中的所有整数 互不相同
*/
class Solution {
public:
    vector<vector<int>> ans;//保存答案
    void dfs(int n,vector<int>nums,vector<int> loc,vector<int> index,int i)
    {
        for(int j=0;j<n;j++)
        {
            if (index[j]) continue;
            loc[i]=j;
            index[j]=1;
            if (i==n-1)//所有的数已经找齐了
            {
                vector<int> tmp;
                for(int k=0;k<n;k++) tmp.push_back(nums[loc[k]]);
                ans.push_back(tmp);
            }
            else dfs(n,nums,loc,index,i+1);
            index[j]=0;
        } 
    }
    vector<vector<int>> permute(vector<int>& nums) {
        int n=nums.size();
        vector<int> loc(n),index(n);//标记第i个数是否被使用
        dfs(n,nums,loc,index,0);
        return ans;
    }
};