/*
问题描述：P0542 01矩阵
给定一个由 0 和 1 组成的矩阵 mat?，请输出一个大小相同的矩阵，其中每一个格子是 mat 中对应位置元素到最近的 0 的距离。
两个相邻元素间的距离为 1 。
示例 1：
输入：mat = [[0,0,0],[0,1,0],[0,0,0]]
输出：[[0,0,0],[0,1,0],[0,0,0]]
示例 2：
输入：mat = [[0,0,0],[0,1,0],[1,1,1]]
输出：[[0,0,0],[0,1,0],[1,2,1]]
提示：
m == mat.length
n == mat[i].length
1 <= m, n <= 104
1 <= m * n <= 104
mat[i][j] is either 0 or 1.
mat 中至少有一个 0?
*/
class Solution {
public:
    vector<vector<int>> updateMatrix(vector<vector<int>>& mat) {
        int m=mat.size(),n=mat[0].size(),i,j;
        for(i=0;i<m;i++)
            for(j=0;j<n;j++)
            {
                if (!mat[i][j])continue;
                mat[i][j]=m+n;
                if (i>0) mat[i][j]=min(mat[i][j],mat[i-1][j]+1);//计算左
                if (j>0) mat[i][j]=min(mat[i][j],mat[i][j-1]+1);//和上
            }
        for(i=m-1;i>=0;i--)//二次遍历
            for(j=n-1;j>=0;j--)
            {
                if (i<m-1) mat[i][j]=min(mat[i][j],mat[i+1][j]+1);//计算右
                if (j<n-1) mat[i][j]=min(mat[i][j],mat[i][j+1]+1);//和下
            }
        return mat;
    }
};