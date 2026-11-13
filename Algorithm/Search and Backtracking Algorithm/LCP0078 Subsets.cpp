/*
问题描述：P0078
给你一个整数数组?nums ，数组中的元素 互不相同 。返回该数组所有可能的子集（幂集）。
解集 不能 包含重复的子集。你可以按 任意顺序 返回解集。
示例 1：
输入：nums = [1,2,3]
输出：[[],[1],[2],[1,2],[3],[1,3],[2,3],[1,2,3]]
示例 2：
输入：nums = [0]
输出：[[],[0]]
提示：
1 <= nums.length <= 10
-10 <= nums[i] <= 10
nums 中的所有元素 互不相同
*/
class Solution {
public:
    vector<vector<int>> ans;
    vector<int> tmp;
    vector<bool>check;
    int n;
    void dfs(vector<int> nums,int index,vector<int>tmp)
    {
        if (index==n) return;
        for(int i=index;i<n;i++)
        {
            if (check[i]||(tmp.size()&&nums[i]<tmp.back())) continue;//判断可行及去重
            check[i]=1;//做标记
            tmp.push_back(nums[i]);//插数
            ans.push_back(tmp);//插子集
            dfs(nums,index+1,tmp);//dfs下一层
            check[i]=0;//回溯
            tmp.pop_back();
        }
    }
    vector<vector<int>> subsets(vector<int>& nums) {
        this->n=nums.size();
        sort(nums.begin(),nums.end());
        check.resize(n);
        ans.push_back(tmp);
        dfs(nums,0,tmp);
        return ans;
    }
};