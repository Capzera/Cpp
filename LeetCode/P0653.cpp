/*
问题描述：P0653
给定一个二叉搜索树 root 和一个目标结果 k，如果 BST 中存在两个元素且它们的和等于给定的目标结果，则返回 true。
示例 1：
输入: root = [5,3,6,2,4,null,7], k = 9
输出: true
示例 2：
输入: root = [5,3,6,2,4,null,7], k = 28
输出: false
提示:
二叉树的节点个数的范围是??[1, 104].
-104?<= Node.val <= 104
root?为二叉搜索树
-105?<= k <= 105
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
    map<int,int> nums;
    void preorder(TreeNode* root)
    {
        if (!root) return;
        nums[root->val]++;
        preorder(root->left);
        preorder(root->right);
    }
    bool findTarget(TreeNode* root, int k) {
        preorder(root);
        map<int,int> num(nums);
        for(auto& i:num)
        {
            i.second--;
            nums[i.first]--;
            if (nums[k-i.first]) return true;
            i.second++;
            nums[i.first]++;
        }
        return false;
    }
};