class Solution {
public:
    bool isSameTree(TreeNode* p, TreeNode* q) {
        if (p==NULL) return q==NULL;
        return q!=NULL && p->val==q->val && isSameTree(p->left,q->left) && isSameTree(p->right,q->right);
    }
};