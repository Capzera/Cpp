/*
问题描述：P0145
给你一棵二叉树的根节点 root ，返回其节点值的 后序遍历 。
示例 1：
输入：root = [1,null,2,3]
输出：[3,2,1]
示例 2：
输入：root = []
输出：[]
示例 3：
输入：root = [1]
输出：[1]
提示：
树中节点的数目在范围 [0, 100] 内
-100 <= Node.val <= 100
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
    vector<int> ans;
    vector<int> postorderTraversal(TreeNode* root) {
        if (root!=NULL)
        {
            postorderTraversal(root->left);
            postorderTraversal(root->right);
            ans.push_back(root->val);
        }
        return ans;
    }
};