/*
问题描述：P6057 力扣第291场周赛
给你一棵二叉树的根节点 root ，找出并返回满足要求的节点数，要求节点的值等于其 子树 中值的 平均值 。
注意：
n 个元素的平均值可以由 n 个元素 求和 然后再除以 n ，并 向下舍入 到最近的整数。
root 的 子树 由 root 和它的所有后代组成。
示例 1：
输入：root = [4,8,5,0,1,null,6]
输出：5
解释：
对值为 4 的节点：子树的平均值 (4 + 8 + 5 + 0 + 1 + 6) / 6 = 24 / 6 = 4 。
对值为 5 的节点：子树的平均值 (5 + 6) / 2 = 11 / 2 = 5 。
对值为 0 的节点：子树的平均值 0 / 1 = 0 。
对值为 1 的节点：子树的平均值 1 / 1 = 1 。
对值为 6 的节点：子树的平均值 6 / 1 = 6 。
示例 2：
输入：root = [1]
输出：1
解释：对值为 1 的节点：子树的平均值 1 / 1 = 1。
提示：
树中节点数目在范围 [1, 1000] 内
0 <= Node.val <= 1000
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
    int ans=0;
    TreeNode* tree=new TreeNode;
    TreeNode* cnt=new TreeNode;
    void preorder(TreeNode* root,TreeNode* tree,TreeNode* cnt) {
        if (!root) return ;
        tree->val=root->val;
        if (root->left) {
            TreeNode* p=new TreeNode();
            TreeNode* q=new TreeNode();
            tree->left=p;
            cnt->left=q;
            preorder(root->left,tree->left,cnt->left);
        }
        if (root->right){
            TreeNode* p=new TreeNode();
            TreeNode* q=new TreeNode();
            tree->right=p;
            cnt->right=q;
            preorder(root->right,tree->right,cnt->right);
        }
    }
    int dep(TreeNode* cnt) {
        if (!cnt) return 0;
        int val1=dep(cnt->left),val2=dep(cnt->right);
        cnt->val=val1+val2+1;
        return cnt->val;
    }
    int cul(TreeNode* tree) {
        if (!tree) return 0;
        int val1=cul(tree->left),val2=cul(tree->right); 
        tree->val+=val1+val2;
        return tree->val;   
    }
    void po(TreeNode* root,TreeNode* tree,TreeNode* cnt){
        if (!tree||!cnt||!root) return;
        if (tree->val/cnt->val==root->val) ans++;
        po(root->left,tree->left,cnt->left);
        po(root->right,tree->right,cnt->right);
    }
    int averageOfSubtree(TreeNode* root) {
        preorder(root,tree,cnt);
        dep(cnt);
        cul(tree);
        po(root,tree,cnt);    
        return ans;
    }
};