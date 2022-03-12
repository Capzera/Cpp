/*
问题描述：P0052
n?皇后问题 研究的是如何将 n?个皇后放置在 n × n 的棋盘上，并且使皇后彼此之间不能相互攻击。
给你一个整数 n ，返回 n 皇后问题 不同的解决方案的数量。
示例 1：
输入：n = 4
输出：2
解释：如上图所示，4 皇后问题存在两个不同的解法。
示例 2：
输入：n = 1
输出：1
提示：
1 <= n <= 9
*/
class Solution {
public:
    vector<bool> col,dia1,dia2;
    int ans=0,n;
    void dfs(int i)
    {
        if (i==n)
        {
            ans++;
            return;
        }
        for(int j=0;j<n;j++)
        {
            if (col[j]||dia1[i+j]||dia2[i-j+n]) continue;
            col[j]=dia1[i+j]=dia2[i-j+n]=1;
            dfs(i+1);
            col[j]=dia1[i+j]=dia2[i-j+n]=0;            
        }
    }
    int totalNQueens(int n) {
        this->n=n;
        col.resize(n);
        dia1.resize(2*n);
        dia2.resize(2*n);
        dfs(0);
        return ans;
    }