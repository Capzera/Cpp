/*
问题描述：P0022
数字 n?代表生成括号的对数，请你设计一个函数，用于能够生成所有可能的并且 有效的 括号组合。
示例 1：
输入：n = 3
输出：["((()))","(()())","(())()","()(())","()()()"]
示例 2：
输入：n = 1
输出：["()"]
提示：
1 <= n <= 8
*/
class Solution {
public:
    vector<string> s;
    void dfs(int l,int r,string tmp,int n)//值传递不需要回溯
    {
        if (l==n&&r==n)//左右括号使用完
        {
            s.push_back(tmp);//结果压入
            return;
        }
        if (l<=n) dfs(l+1,r,tmp+"(",n);//dfs下一个左括号且不允许左括号超量
        if (r<=n&&l>=r+1) dfs(l,r+1,tmp+")",n);//dfs下一个右括号且不允许超过当前左括号数量
    }
    vector<string> generateParenthesis(int n) { 
        dfs(0,0,"",n);//开始递归，0个左、右括号，初始结果为空，最大边界为n
        return s;
    }
};