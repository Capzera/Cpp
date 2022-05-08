/*
问题描述：P1305
给你?root1 和 root2?这两棵二叉搜索树。请你返回一个列表，其中包含?两棵树?中的所有整数并按 升序 排序。.
示例 1：
输入：root1 = [2,1,4], root2 = [1,0,3]
输出：[0,1,1,2,3,4]
示例 2：
输入：root1 = [1,null,8], root2 = [8,1]
输出：[1,1,8,8]
提示：
每棵树的节点数在?[0, 5000] 范围内
-105?<= Node.val <= 105
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
    vector<int> v1,v2;
    void po(TreeNode* root,vector<int>& v){
        if (!root) return;
        po(root->left,v);
        v.push_back(root->val);
        po(root->right,v);
    }
    vector<int> getAllElements(TreeNode* root1, TreeNode* root2) {
        po(root1,v1);
        po(root2,v2);
        vector<int> ans;
        int n1=v1.size(),n2=v2.size();
        int i=0,j=0;
        while (i<n1&&j<n2) {
            if (v1[i]<=v2[j]) ans.push_back(v1[i++]);
            else ans.push_back(v2[j++]);
        }
        while (i<n1) ans.push_back(v1[i++]);
        while (j<n2) ans.push_back(v2[j++]);
        return ans;
    }
};