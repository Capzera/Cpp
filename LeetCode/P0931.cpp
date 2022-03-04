/*
问题描述：P0931
给你一个 n x n 的 方形 整数数组?matrix ，请你找出并返回通过 matrix 的下降路径 的 最小和 。
下降路径 可以从第一行中的任何元素开始，并从每一行中选择一个元素。
在下一行选择的元素和当前行所选元素最多相隔一列（即位于正下方或者沿对角线向左或者向右的第一个元素）。
具体来说，位置 (row, col) 的下一个元素应当是 (row + 1, col - 1)、(row + 1, col) 或者 (row + 1, col + 1) 。
示例 1：
输入：matrix = [[2,1,3],[6,5,4],[7,8,9]]
输出：13
解释：如图所示，为和最小的两条下降路径
示例 2：
输入：matrix = [[-19,57],[-40,-5]]
输出：-59
解释：如图所示，为和最小的下降路径
提示：
n == matrix.length == matrix[i].length
1 <= n <= 100
-100 <= matrix[i][j] <= 100
*/
class Solution {
public:
    int minFallingPathSum(vector<vector<int>>& matrix) {
        int n=matrix.size(),ans=INT_MAX;
        if (n==1) return matrix[0][0];
        for(int i=n-2;i>=0;i--)
        {
            for(int j=0;j<n;j++)
            {
                int temp1=INT_MAX,temp2=INT_MAX;
                if (j>0) temp1=min(matrix[i][j]+matrix[i+1][j-1],matrix[i][j]+matrix[i+1][j]);
                if (j<n-1) temp2=min(matrix[i][j]+matrix[i+1][j],matrix[i][j]+matrix[i+1][j+1]);
                matrix[i][j]=min(temp1,temp2);
                if (!i) ans=min(ans,matrix[i][j]);
            }
        }
        return ans;
    }
};