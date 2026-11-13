#include <bits/stdc++.h>
using namespace std;
struct Trie {
	int val = 0;
	Trie* child[62] = {nullptr};
} *root;
int get(char x) {
	if (islower(x)) return x - 'a';
	if (isupper(x)) return x - 'A' + 26;
	return x - '0' + 52;
}
void build(string s) {
	auto p = root;
	for (auto& ch : s) {
		int x = get(ch);
		p = p->child[x] ? p->child[x] : p->child[x] = new Trie;
		p->val++;
	}
}
int query(string s) {
	auto p = root;
	for (auto& ch : s) {
		int x = get(ch);
		if (!p->child[x]) return 0;
		p = p->child[x];
	}
	return p->val;
	
}
int t, n, q;
string s;
void solve() {
	cin >> n >> q;
	while (n--) {
		cin >> s;
		build(s);
	}
	while (q--) {
		cin >> s;
		cout << query(s) << endl;
	}
}
int main() {
	cin >> t;
	while (t--) {
		root = new Trie;
		solve();
		delete root;
	}
	return 0;
}
