/*
问题描述：P0077
给定两个整数 n 和 k，返回范围 [1, n] 中所有可能的 k 个数的组合。
你可以按 任何顺序 返回答案。
示例 1：
输入：n = 4, k = 2
输出：
[
  [2,4],
  [3,4],
  [2,3],
  [1,2],
  [1,3],
  [1,4],
]
示例 2：
输入：n = 1, k = 1
输出：[[1]]
提示：
1 <= n <= 20
1 <= k <= n
*/
class Solution {
public:
    vector<vector<int>> ans;
    void dfs(int n,int i,int k,vector<int>loc,vector<int>index,int last)
    {
        for(int j=i;j<n;j++)
        {
            if (index[j]||j<last) continue;//如果第j个数已经被使用，跳过
            loc[i]=j;
            index[j]=1;
            if (i==k-1)//k个数已经找齐的情况
            {
                vector<int> tmp(k);
                for(int x=0;x<k;x++) tmp[x]=loc[x]+1;
                ans.push_back(tmp);
            }
            else dfs(n,i+1,k,loc,index,j);
            index[j]=0;
        }
    }
    vector<vector<int>> combine(int n, int k) {
        vector<int> loc(k),index(n);
        dfs(n,0,k,loc,index,0);
        return ans;
    }
};