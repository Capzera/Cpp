#include <bits/stdc++.h>
using namespace std;
struct Trie {
	int val;
	Trie* child[26] = {nullptr};
	Trie(): val(0) {}
} *root;
int t, n;
string s;
void build(string s) {
	auto p = root;
	for (int i = s.size() - 1; i >= 0; i--) {
		int x = s[i] - 'A';
		if (!p->child[x]) p->child[x] = new Trie();
		p = p->child[x];
		p->val++; // 路径和
	}
}
int dfs(Trie* root) {
	int ans = 0;
	for (int ch = 0; ch < 26; ch++) {
		if (!root->child[ch]) continue;
		ans += dfs(root->child[ch]);
	}
	if (ans + 2 <= root->val) ans += 2; // 如果当前这个点的子树配队数 比当前这个点的总路径少2，说明可以在当前点处完成一组配对
	return ans;
}
int solve() {
	cin >> n;
	while (n--) {
		cin >> s;
		build(s);
	}
	return dfs(root);
}
int main() {
	cin >> t;
	for (int i = 1; i <= t; i++) {
		root = new Trie();
		cout << "Case #" << i << ": ";
		cout << solve() << endl;
		delete root;
	}
	return 0;
}
