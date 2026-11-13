#include <bits/stdc++.h>
using namespace std;
struct Trie {
	int val;
	Trie* child[26] = {nullptr};
	Trie() : val(0) {}
} *root;
void build(string val) {
	auto p = root;
	for (auto& ch : val) {
		p = p->child[ch - 'a'] ? p->child[ch - 'a'] : p->child[ch - 'a'] = new Trie();
	}
	p->val++;
}
int query(string val) {
	auto p = root;
	int ans = 0;
	for (auto& ch : val) {
		if (!p->child[ch - 'a']) return ans;
		p = p->child[ch - 'a'];
		ans += p->val;
	}
	return ans;
}
int n, m;
string s;
int main() {
	cin >> n >> m;
	root = new Trie();
	while (n--) {
		cin >> s;
		build(s);
	}
	while (m--) {
		cin >> s;
		cout << query(s) << endl;
	}
	return 0;
}
