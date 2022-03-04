/*
问题描述：P0994
在给定的?m x n?网格?grid?中，每个单元格可以有以下三个值之一：
值?0?代表空单元格；
值?1?代表新鲜橘子；
值?2?代表腐烂的橘子。
每分钟，腐烂的橘子?周围?4 个方向上相邻 的新鲜橘子都会腐烂。
返回 直到单元格中没有新鲜橘子为止所必须经过的最小分钟数。如果不可能，返回?-1?。
示例 1：
输入：grid = [[2,1,1],[1,1,0],[0,1,1]]
输出：4
示例 2：
输入：grid = [[2,1,1],[0,1,1],[1,0,1]]
输出：-1
解释：左下角的橘子（第 2 行， 第 0 列）永远不会腐烂，因为腐烂只会发生在 4 个正向上。
示例 3：
输入：grid = [[0,2]]
输出：0
解释：因为 0 分钟时已经没有新鲜橘子了，所以答案就是 0 。
提示：
m == grid.length
n == grid[i].length
1 <= m, n <= 10
grid[i][j] 仅为?0、1?或?2
*/
class Solution {
public:
    int orangesRotting(vector<vector<int>>& grid) {
        int min=0,fresh=0;
        queue<pair<int, int>> q;
        for(int i=0;i<grid.size();i++) {
            for(int j=0;j<grid[0].size();j++)
                if(grid[i][j] == 1) fresh++;//统计新鲜数
                else if(grid[i][j] == 2) q.push({i, j});
        }
        vector<pair<int, int>> dirs = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };//四个方向移动
        while(!q.empty()) {
            int n=q.size();
            bool rotten=false;
            for(int i=0;i<n;i++) {
                auto x=q.front();
                q.pop();
                for(auto cur:dirs) {
                    int i=x.first+cur.first;
                    int j=x.second+cur.second;
                    if(i>=0&&i<grid.size()&&j>=0&&j<grid[0].size()&&grid[i][j]==1) {
                        grid[i][j]=2;//新的烂橘子
                        q.push({i, j});//入队
                        fresh--;//减少新鲜数
                        rotten=true;
                    }
                }
            }
            if(rotten) min++;//时间增加
        } 
        return fresh? -1:min;
    }
};