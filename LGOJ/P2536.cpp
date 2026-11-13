#include <bits/stdc++.h>
using namespace std;
struct Trie {
	int val;
	Trie* child[4] = {nullptr};
	Trie() : val(0) {}
} *root;
int get(char x) {
	if (x == 'A') return 0;
	if (x == 'T') return 1;
	if (x == 'G') return 2;
	return 3;
}
set<pair<int, Trie*> > vis;
string model, text, mp = "ATGC";
int n, m, ans;
void build(string s) {
	auto p = root;
	for (auto& ch : s) {
		int x = get(ch);
		p = p->child[x] ? p->child[x] : p->child[x] = new Trie();
	}
	p->val++;
}
void dfs(int idx, Trie* root) {
	if (idx == m) {
		ans += root->val;
		root->val = 0;
		return;
	}
	if (vis.count({idx, root})) return;
	vis.insert({idx, root});
	char cur = model[idx];
	if (isalpha(cur)) {
		if (!root->child[get(cur)]) return;
		dfs(idx + 1, root->child[get(cur)]);
	}
	if (cur == '?') {
		for (auto& nxt : mp) {
			int x = get(nxt);
			if (!root->child[x]) continue;
			dfs(idx + 1, root->child[x]);
		}
	}
	if (cur == '*') {
		dfs(idx + 1, root);
		for (auto& nxt : mp) {
			int x = get(nxt);
			if (!root->child[x]) continue;
			dfs(idx + 1, root->child[x]);
			dfs(idx, root->child[x]);
		}
	}
}
int main() {
	cin >> model >> n;
	m = model.size();
	root = new Trie();
	for (int i = 1; i <= n; i++) {
		cin >> text;
		build(text);
	}
	dfs(0, root);
	cout << n - ans;
	return 0;
}
