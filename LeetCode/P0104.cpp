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
    int maxd=1;
public:
    void ans(TreeNode* root,int depth)
    {
        if (!root->left&&!root->right){
            maxd=max(maxd,depth);
            return;
        }
        if (root->left!=NULL) ans(root->left,depth+1);
        if (root->right!=NULL) ans(root->right,depth+1);
    }
    int maxDepth(TreeNode* root) {
        if (!root) return 0;
        ans(root,1);
        return maxd;
    }
};