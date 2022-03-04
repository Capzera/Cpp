/*
问题描述：
给你一个大小为 m x n 的二进制矩阵 grid 。
岛屿?是由一些相邻的1(代表土地) 构成的组合，这里的「相邻」要求两个1必须在水平或者竖直的四个方向上相邻。
你可以假设?grid 的四个边缘都被 0（代表水）包围着。
岛屿的面积是岛上值为1的单元格的数目。
计算并返回grid中最大的岛屿面积。如果没有岛屿，则返回面积为 0 。
示例 1：
输入：grid = [[0,0,1,0,0,0,0,1,0,0,0,0,0],[0,0,0,0,0,0,0,1,1,1,0,0,0],[0,1,1,0,1,0,0,0,0,0,0,0,0],[0,1,0,0,1,1,0,0,1,0,1,0,0],[0,1,0,0,1,1,0,0,1,1,1,0,0],[0,0,0,0,0,0,0,0,0,0,1,0,0],[0,0,0,0,0,0,0,1,1,1,0,0,0],[0,0,0,0,0,0,0,1,1,0,0,0,0]]
输出：6
解释：答案不应该是 11 ，因为岛屿只能包含水平或垂直这四个方向上的 1 。
示例 2：
输入：grid = [[0,0,0,0,0,0,0,0]]
输出：0

*/
class Solution {
public:
    int move[4][2]={{-1,0},{0,-1},{1,0},{0,1}};//上左下右
    void dfs(vector<vector<bool>>& check,vector<vector<int>> grid,int m,int n,int x,int y,int& ans)
    {
        if (!check[x][y]) 
        {
            check[x][y]=true;
            if (grid[x][y]==1) ans++;
        }
        else return;
        for(int i=0;i<4;i++)
        {
            int dx=x+move[i][0],dy=y+move[i][1];
            if (dx>=0&&dx<m&&dy>=0&&dy<n&&check[dx][dy]==0&&grid[dx][dy]==1) dfs(check,grid,m,n,dx,dy,ans);
        }
    }
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        int m=grid.size(),n=grid[0].size(),i,j,maxtot=0;
        vector<vector<bool>> check(m,vector<bool>(n));
        for(i=0;i<m;i++)
            for(j=0;j<n;j++) if (!grid[i][j]) check[i][j]=1;
        for(i=0;i<m;i++)
            for(j=0;j<n;j++)
            {
                int area=0;
                if (grid[i][j]&&check[i][j]==false) 
                {
                    dfs(check,grid,m,n,i,j,area);
                    maxtot=max(maxtot,area);
                }
            }
        return maxtot;
    }
};