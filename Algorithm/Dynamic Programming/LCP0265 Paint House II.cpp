/*
问题描述：P0265
假如有一排房子共有?n?幢，每个房子可以被粉刷成 k?种颜色中的一种。
房子粉刷成不同颜色的花费成本也是不同的。你需要粉刷所有的房子并且使其相邻的两个房子颜色不能相同。
每个房子粉刷成不同颜色的花费以一个 n x k 的矩阵表示。
例如，costs[0][0] 表示第 0?幢房子粉刷成 0 号颜色的成本；costs[1][2]?表示第 1 幢房子粉刷成 2 号颜色的成本，以此类推。
返回 粉刷完所有房子的最低成本?。
示例 1：
输入: costs = [[1,5,3],[2,9,4]]
输出: 5
解释: 
将房子 0 刷成 0 号颜色，房子 1 刷成 2 号颜色。花费: 1 + 4 = 5; 
或者将 房子 0 刷成 2 号颜色，房子 1 刷成 0 号颜色。花费: 3 + 2 = 5. 
示例?2:
输入: costs = [[1,3],[2,4]]
输出: 5
提示：
costs.length == n
costs[i].length == k
1 <= n <= 100
2 <= k <= 20
1 <= costs[i][j] <= 20
*/
class Solution {
public:
    int minCostII(vector<vector<int>>& costs) {
        int n=costs.size(),ans=INT_MAX,k=costs[0].size(),i,j;
        if (n==1) {
            for (int i=0;i<k;i++) ans=min(ans,costs[0][i]);
        }
        for (i=1;i<n;i++) {
            for (j=0;j<k;j++) {
                int add=costs[i][j];
                costs[i][j]=1000;
                for (int t=0;t<k;t++) {
                    if (j==t) continue;
                    costs[i][j]=min(costs[i-1][t],costs[i][j]);
                }
                costs[i][j]+=add;
                if (i==n-1) ans=min(ans,costs[i][j]);
            }
        }
        return ans;
    }
};