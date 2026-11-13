/*
问题描述：P0587
在一个二维的花园中，有一些用 (x, y) 坐标表示的树。
由于安装费用十分昂贵，你的任务是先用最短的绳子围起所有的树。
只有当所有的树都被绳子包围时，花园才能围好栅栏。你需要找到正好位于栅栏边界上的树的坐标。
示例 1:
输入: [[1,1],[2,2],[2,0],[2,4],[3,3],[4,2]]
输出: [[1,1],[2,0],[4,2],[3,3],[2,4]]
解释:
示例 2:
输入: [[1,2],[2,2],[4,2]]
输出: [[1,2],[2,2],[4,2]]
解释:
即使树都在一条直线上，你也需要先用绳子包围它们。
注意:
所有的树应当被围在一起。你不能剪断绳子来包围树或者把树分成一组以上。
输入的整数在 0 到 100 之间。
花园至少有一棵树。
所有树的坐标都是不同的。
输入的点没有顺序。输出顺序也没有要求。
*/
class Solution {
public:
    int cross(vector<int> & p, vector<int> & q, vector<int> & r) {
        return (q[0] - p[0]) * (r[1] - q[1]) - (q[1] - p[1]) * (r[0] - q[0]);
    }

    vector<vector<int>> outerTrees(vector<vector<int>>& trees) {
        int n = trees.size();
        if (n < 4) {
            return trees;
        }
        int leftMost = 0;
        for (int i = 0; i < n; i++) {
            if (trees[i][0] < trees[leftMost][0]) {
                leftMost = i;
            }
        }

        vector<vector<int>> res;
        vector<bool> visit(n, false);
        int p = leftMost;
        do {
            int q = (p + 1) % n;
            for (int r = 0; r < n; r++) {
                /* 如果 r 在 pq 的右侧，则 q = r */ 
                if (cross(trees[p], trees[q], trees[r]) < 0) {
                    q = r;
                }
            }
            /* 是否存在点 i, 使得 p 、q 、i 在同一条直线上 */
            for (int i = 0; i < n; i++) {
                if (visit[i] || i == p || i == q) {
                    continue;
                }
                if (cross(trees[p], trees[q], trees[i]) == 0) {
                    res.emplace_back(trees[i]);
                    visit[i] = true;
                }
            }
            if  (!visit[q]) {
                res.emplace_back(trees[q]);
                visit[q] = true;
            }
            p = q;
        } while (p != leftMost);
        return res;
    }
};