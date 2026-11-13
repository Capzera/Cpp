#include <bits/stdc++.h>
using namespace std;
struct TreeNode {
	char val;
	TreeNode *left = nullptr, *right = nullptr;
	TreeNode () {}
	TreeNode (char _v) : val(_v) {}
} *root, *mp[26];
void PreOrder(TreeNode *rt) {
	if (!rt) return;
	cout << rt->val;
	PreOrder(rt->left);
	PreOrder(rt->right);
}
TreeNode* build(char ch) {
	if (ch == '*') return nullptr;
	if (mp[ch - 'a']) return mp[ch - 'a'];
	TreeNode *root = new TreeNode(ch);
	mp[ch - 'a'] = root;
	return root;
}
int n;
char u, ul, ur;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> u >> ul >> ur;
		TreeNode *rt = build(u);
		if (i == 1) root = rt;
		rt->left = build(ul);
		rt->right = build(ur);
	}
	PreOrder(root);
	return 0;
}
