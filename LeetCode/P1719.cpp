/*
题目描述：
    给你一个数组?pairs ，其中?pairs[i] = [xi, yi]?，并且满足：
        pairs?中没有重复元素
        xi < yi
        令?ways?为满足下面条件的有根树的方案数：
            树所包含的所有节点值都在 pairs?中。
            一个数对?[xi, yi] 出现在?pairs?中?当且仅当?xi?是?yi?的祖先或者?yi?是?xi?的祖先。
    注意：构造出来的树不一定是二叉树。
    两棵树被视为不同的方案当存在至少一个节点在两棵树中有不同的父节点。
请你返回：
    如果?ways == 0?，返回?0?。
    如果?ways == 1?，返回 1?。
    如果?ways > 1?，返回?2?。
    一棵 有根树?指的是只有一个根节点的树，所有边都是从根往外的方向。
    我们称从根到一个节点路径上的任意一个节点（除去节点本身）都是该节点的 祖先?。根节点没有祖先。
示例 1：
输入：
    pairs = [[1,2],[2,3]]
输出：
    1
    解释：如上图所示，有且只有一个符合规定的有根树。
示例 2：
输入：
    pairs = [[1,2],[2,3],[1,3]]
输出：2
解释：
    有多个符合规定的有根树，其中三个如上图所示。
示例 3：
输入：
    pairs = [[1,2],[2,3],[2,4],[1,5]]
输出：0
解释：
    没有符合规定的有根树。
*/
class Solution {
public:
    int checkWays(vector<vector<int>>& pairs) {
        unordered_map<int, unordered_set<int>> adj;
        for (auto &p : pairs) {
            adj[p[0]].emplace(p[1]);
            adj[p[1]].emplace(p[0]);
        }
        /* 检测是否存在根节点*/
        int root = -1;
        for (auto &[node, neighbours] : adj) {
            if (neighbours.size() == adj.size() - 1) {
                root = node;
                break;
            }
        }
        if (root == -1) {
            return 0;
        }

        int res = 1;
        for (auto &[node, neighbours] : adj) {
            if (node == root) {
                continue;
            }
            int currDegree = neighbours.size();
            int parent = -1;
            int parentDegree = INT_MAX;

            /* 根据 degree 的大小找到 node 的父节点 parent */
            for (auto &neighbour : neighbours) {
                if (adj[neighbour].size() < parentDegree && adj[neighbour].size() >= currDegree) {
                    parent = neighbour;
                    parentDegree = adj[neighbour].size();
                }
            }
            if (parent == -1) {
                return 0;
            }

            /* 检测 neighbours 是否是 adj[parent] 的子集 */
            for (auto &neighbour : neighbours) {
                if (neighbour == parent) {
                    continue;
                }
                if (!adj[parent].count(neighbour)) {
                    return 0;
                }
            }
            if (parentDegree == currDegree) {
                res = 2;
            }
        }
        return res;
    }
};