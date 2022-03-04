class Solution {
public:
    bool check(TreeNode* p,TreeNode* q)
    {
        if (!p&&!q) return true;
        if (!p||!q) return false;
        return check(p->left,q->right)&&check(p->right,q->left)&&p->val==q->val;
    }
    bool isSymmetric(TreeNode* root) {
        return check(root->left,root->right);
    }
};