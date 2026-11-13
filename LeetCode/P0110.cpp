/*
问题描述：P0110
给定一个二叉树，判断它是否是高度平衡的二叉树。  
本题中，一棵高度平衡二叉树定义为：
一个二叉树每个节点?的左右两个子树的高度差的绝对值不超过 1 。
示例 1：
输入：root = [3,9,20,null,null,15,7]
输出：true
示例 2：
输入：root = [1,2,2,3,3,null,null,4,4]
输出：false
示例 3：
输入：root = []
输出：true
提示：
树中的节点数在范围 [0, 5000] 内
-104 <= Node.val <= 104
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
    int height(TreeNode* root)
    {
        if(!root)return 0;
        int lh=height(root->left),rh=height(root->right);
        if(lh<0||rh<0||abs(lh-rh)>1) return -1;
        else return max(lh,rh)+1;
    }
    bool isBalanced(TreeNode* root) {
        return height(root)>=0;
    }
};