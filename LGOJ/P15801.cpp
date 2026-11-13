#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
struct TreeNode {
	int val;
	TreeNode *lt, *rt;
	TreeNode() {}
	TreeNode(int _v) : val(_v), lt(nullptr), rt(nullptr) {}
	TreeNode(int _v, TreeNode* _l, TreeNode* _r) : val(_v), lt(_l), rt(_r) {}
} *tr[N];
int n, l, r, ans;
vector<int> dfs(TreeNode* root) {
	if (!root) return {0, 1, 1};
	vector<int> ans(3), lt = dfs(root->lt), rt = dfs(root->rt);
	ans[0] = max(lt[0], rt[0]) + 1;
	ans[1] = lt[1] & rt[1] & lt[0] == rt[0];
	ans[2] = (lt[0] == rt[0] && lt[1] & rt[2]) || (lt[0] == rt[0] + 1 && lt[2] & rt[1]);
	::ans += ans[2];
	return ans;
}
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		tr[i] = new TreeNode(i);
	}
	for (int i = 1; i <= n; i++) {
		cin >> l >> r;
		if (l) tr[i]->lt = tr[l];
		if (r) tr[i]->rt = tr[r];
	}
	dfs(tr[1]);
	cout << ans;
	return 0;
}
