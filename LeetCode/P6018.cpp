/*
问题描述：P6018 力扣第283场周赛
给你一个二维整数数组 descriptions ，其中 descriptions[i] = [parenti, childi, isLefti] 
表示 parenti 是 childi 在 二叉树 中的 父节点，二叉树中各节点的值 互不相同 。此外：
如果 isLefti == 1 ，那么 childi 就是 parenti 的左子节点。
如果 isLefti == 0 ，那么 childi 就是 parenti 的右子节点。
请你根据 descriptions 的描述来构造二叉树并返回其 根节点 。
测试用例会保证可以构造出 有效 的二叉树。
示例 1：
输入：descriptions = [[20,15,1],[20,17,0],[50,20,1],[50,80,0],[80,19,1]]
输出：[50,20,80,15,17,19]
解释：根节点是值为 50 的节点，因为它没有父节点。
结果二叉树如上图所示。
示例 2：
输入：descriptions = [[1,2,1],[2,3,0],[3,4,1]]
输出：[1,2,null,null,3,4]
解释：根节点是值为 1 的节点，因为它没有父节点。 
结果二叉树如上图所示。 
提示：
1 <= descriptions.length <= 104
descriptions[i].length == 3
1 <= parenti, childi <= 105
0 <= isLefti <= 1
descriptions 所描述的二叉树是一棵有效二叉树
*/
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    TreeNode* createBinaryTree(vector<vector<int>>& descriptions) {    
        unordered_map<int,TreeNode*> um1;//保存每个子二叉树
        unordered_map<int,int> um2;//保存每个结点作为孩子的次数，为0次的为最终根节点
        for (vector<int>& i:descriptions)
        {
            if (!um1.count(i[0])) um1[i[0]] = new TreeNode(i[0]);
            if (!um1.count(i[1])) um1[i[1]] = new TreeNode(i[1]);//产生新的结点
            if (i[2]) um1[i[0]]->left=um1[i[1]];//为左孩子的时候
            else um1[i[0]]->right=um1[i[1]];
            um2[i[1]]++;
        }
        for (auto [i,node]:um1)
            if (um2[i]==0) return node;
        return nullptr;
    }
};