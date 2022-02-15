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
    vector<int> pt;
    void IOT(TreeNode *root)
    {
        if (root==NULL) return;
        IOT(root->left);
        pt.push_back(root->val);
        IOT(root->right);
    }
    vector<int> inorderTraversal(TreeNode* root) {
        IOT(root);
        return pt;
    }
};