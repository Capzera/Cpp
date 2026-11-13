/*
问题描述：P0040
给定一个候选人编号的集合?candidates?和一个目标数?target?，找出?candidates?中所有可以使数字和为?target?的组合。
candidates?中的每个数字在每个组合中只能使用?一次?。
注意：解集不能包含重复的组合。?
示例?1:
输入: candidates =?[10,1,2,7,6,1,5], target =?8,
输出:
[
[1,1,6],
[1,2,5],
[1,7],
[2,6]
]
示例?2:
输入: candidates =?[2,5,2,1,2], target =?5,
输出:
[
[1,2,2],
[5]
]
提示:
1 <=?candidates.length <= 100
1 <=?candidates[i] <= 50
1 <= target <= 30
*/
class Solution {
public:
    vector<vector<int>> ans;
    vector<int> tmp;
    int n;
    void dfs(vector<int> candidates,vector<int> tmp,int target,int index)
    {
        if (!target)
        {
            ans.push_back(tmp);
            return;
        }
        for(int i=index;i<n;i++)
        {
            if (i>index&&candidates[i]==candidates[i-1]) continue;
            if (target-candidates[i]>=0)
            {
                tmp.push_back(candidates[i]);
                dfs(candidates,tmp,target-candidates[i],i+1);
                tmp.pop_back();
            }
            else break;
        }
    }
    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
        this->n=candidates.size();
        sort(candidates.begin(),candidates.end());
        dfs(candidates,tmp,target,0);
        return ans;
    }
};