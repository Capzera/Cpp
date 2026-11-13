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
    bool dfs(TreeNode* root,int target)
    {
        if (root==NULL) return false;
        else if (root->left==NULL&&root->right==NULL)
        {
            if (target-root->val==0) return true;
            else return false;
        }
        return dfs(root->left,target-root->val)||dfs(root->right,target-root->val);
    }
    bool hasPathSum(TreeNode* root, int targetSum) {
        if (root==NULL) return false;
        return dfs(root,targetSum);
    }
};