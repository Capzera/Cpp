/*
问题描述：P0102
给你二叉树的根节点 root ，返回其节点值的 层序遍历 。 （即逐层地，从左到右访问所有节点）。
示例 1：
输入：root = [3,9,20,null,null,15,7]
输出：[[3],[9,20],[15,7]]
示例 2：
输入：root = [1]
输出：[[1]]
示例 3：
输入：root = []
输出：[]
提示：
树中节点数目在范围 [0, 2000] 内
-1000 <= Node.val <= 1000\
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
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> ans;
        if (root==NULL) return ans;
        queue<TreeNode*> Q;
        int i;
        Q.push(root);
        while (!Q.empty())
        {
            int n=Q.size();
            vector<int> tmp;
            for(i=0;i<n;i++)
            {
                TreeNode* node=Q.front();
                tmp.push_back(node->val);
                Q.pop();
                if (node->left!=NULL) Q.push(node->left);
                if (node->right!=NULL) Q.push(node->right);
            }
            ans.push_back(tmp);
        }
        return ans;
    }
};