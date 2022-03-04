/*
题目描述：
    在一个?n x n?的国际象棋棋盘上，一个骑士从单元格 (row, column)?开始，并尝试进行 k 次移动。
    行和列是 从 0 开始 的，所以左上单元格是 (0,0) ，右下单元格是 (n - 1, n - 1) 。
    象棋骑士有8种可能的走法，如下图所示。每次移动在基本方向上是两个单元格，然后在正交方向上是一个单元格。
    每次骑士要移动时，它都会随机从8种可能的移动中选择一种(即使棋子会离开棋盘)，然后移动到那里。
    骑士继续移动，直到它走了 k 步或离开了棋盘。
    返回：骑士在棋盘停止移动后仍留在棋盘上的概率 。
示例 1：
输入: 
    n = 3, k = 2, row = 0, column = 0
输出: 
    0.0625
    解释: 有两步(到(1,2)，(2,1))可以让骑士留在棋盘上。
    在每一个位置上，也有两种移动可以让骑士留在棋盘上。
    骑士留在棋盘上的总概率是0.0625。
示例 2：
输入:
    n = 1, k = 0, row = 0, column = 0
输出: 
    1.00000
*/
class Solution {
public:
    double knightProbability(int n, int k, int row, int column) {
        int move[8][2]={{-1,-2},{-2,-1},{-2,1},{-1,2},{1,-2},{2,-1},{1,2},{2,1}};//move保存棋子可移动的位置
        int i,j,step;
        vector<vector<double>> pdp(n,vector<double>(n)),dp(n,vector<double>(n));
        if (!k) return 1;
        for(i=0;i<n;i++)
            for(j=0;j<n;j++)pdp[i][j]=1;//pdp为上一步的dp结果
        for(step=1;step<=k;step++)
        {
            for(i=0;i<n;i++)
            {
                for(j=0;j<n;j++)
                {
                    dp[i][j]=0;
                    for(auto& mov:move)
                    {
                        int nx=i+mov[0],ny=j+mov[1];//nx，ny记录当前移动的坐标
                        if (nx>=0&&nx<n&&ny>=0&&ny<n) dp[i][j]+=(pdp[nx][ny]/8);//如果坐标落在棋盘内，dp[i][j]=Σ上一层的结果/8(Σ数8次)
                    }
                }
            }
            for(i=0;i<n;i++)
            {
                for(j=0;j<n;j++)pdp[i][j]=dp[i][j];//将本层dp结果保存为上一层dp结果供下次循环使用
            }
        }
        return dp[row][column];
    }
};