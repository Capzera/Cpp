/*
问题描述：P0051
n?皇后问题 研究的是如何将 n?个皇后放置在 n×n 的棋盘上，并且使皇后彼此之间不能相互攻击。
给你一个整数 n ，返回所有不同的?n?皇后问题 的解决方案。
每一种解法包含一个不同的?n 皇后问题 的棋子放置方案，该方案中 'Q' 和 '.' 分别代表了皇后和空位。
示例 1：
输入：n = 4
输出：[[".Q..","...Q","Q...","..Q."],["..Q.","Q...","...Q",".Q.."]]
解释：如上图所示，4 皇后问题存在两个不同的解法。
示例 2：
输入：n = 1
输出：[["Q"]]
提示：
1 <= n <= 9
*/
class Solution {
public:
    vector<vector<string>> ans;
    void dfs(int n,vector<int> location,vector<bool> column,vector<bool> diagonal1,vector<bool>diagonal2,int i)
    {
        int j,k;
        for(int j=0;j<n;j++)//搜索1-n
        {
            if (!column[j]&&!diagonal1[i+j]&&!diagonal2[i-j+n])//该格可以填入皇后
            {
                location[i]=j;//记录该皇后的列
                column[j]=diagonal1[i+j]=diagonal2[i-j+n]=1;//做标记
                if (i==n-1)//所有皇后填完
                {
                    vector<string>tmp(n);//申请临时空间保存皇后位置
                    for(k=0;k<n;k++)//需要打印n行
                    {
                        for(int k1=0;k1<n;k1++)//控制列
                        {
                            if (k1==location[k]) tmp[k].push_back('Q');
                            else tmp[k].push_back('.');
                        }
                    }
                    ans.push_back(tmp);
                }
                else dfs(n,location,column,diagonal1,diagonal2,i+1);
                column[j]=diagonal1[i+j]=diagonal2[i-j+n]=0;
            }
        }
    }
    vector<vector<string>> solveNQueens(int n) {
        vector<int> location(n);
        vector<bool> column(n),diagonal1(2*n),diagonal2(2*n);
        dfs(n,location,column,diagonal1,diagonal2,0);
        return ans;
    }
};