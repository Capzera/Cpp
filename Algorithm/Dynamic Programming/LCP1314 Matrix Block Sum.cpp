/*
问题描述：P1314
给你一个?m x n?的矩阵?mat?和一个整数 k ，请你返回一个矩阵?answer?，
其中每个?answer[i][j]?是所有满足下述条件的元素?mat[r][c] 的和：?
i - k <= r <= i + k,
j - k <= c <= j + k 且
(r, c)?在矩阵内。
示例 1：
输入：mat = [[1,2,3],[4,5,6],[7,8,9]], k = 1
输出：[[12,21,16],[27,45,33],[24,39,28]]
示例 2：
输入：mat = [[1,2,3],[4,5,6],[7,8,9]], k = 2
输出：[[45,45,45],[45,45,45],[45,45,45]]
提示：
m ==?mat.length
n ==?mat[i].length
1 <= m, n, k <= 100
1 <= mat[i][j] <= 100
*/
class Solution {
public:
    vector<vector<int>> matrixBlockSum(vector<vector<int>>& mat, int k) {
        int m=mat.size(),n=mat[0].size();;
        vector<vector<int>> sum(m+1,vector<int>(n+1)),ans(m,vector<int>(n));
        if (m>0)
        {
            for(int i=0;i<m;i++)
            {
                for(int j=0;j<n;j++)
                {
                    sum[i+1][j+1]=sum[i][j+1]+sum[i+1][j]-sum[i][j]+mat[i][j];//处理前缀和
                }
            }
        }
        for(int i=0;i<m;i++)
        {
            for(int j=0;j<mat[0].size();j++) 
            {
                int r1=i-k>0? i-k:0,r2=i+k<m? i+k:m-1,c1=j-k>0? j-k:0,c2=j+k<n? j+k:n-1;
                ans[i][j]=sum[r2+1][c2+1]-sum[r2+1][c1]-sum[r1][c2+1]+sum[r1][c1];//矩阵面积公式
            }
        }
        return ans;
    }
};