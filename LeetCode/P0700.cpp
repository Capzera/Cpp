/*
问题描述：P0700
给定二叉搜索树（BST）的根节点?root?和一个整数值?val。
你需要在 BST 中找到节点值等于?val?的节点。 返回以该节点为根的子树。 如果节点不存在，则返回?null?。
示例 1:
输入：root = [4,2,7,1,3], val = 2
输出：[2,1,3]
Example 2:
输入：root = [4,2,7,1,3], val = 5
输出：[]
提示：
数中节点数在?[1, 5000]?范围内
1 <= Node.val <= 107
root?是二叉搜索树
1 <= val <= 107
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
    TreeNode* searchBST(TreeNode* root, int val) {
        if (!root) return NULL;
        if (val<root->val) return searchBST(root->left,val);
        if (val>root->val) return searchBST(root->right,val);
        else return root;
    }
};