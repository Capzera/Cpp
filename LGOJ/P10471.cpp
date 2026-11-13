#include <bits/stdc++.h>
using namespace std;
struct Trie {
	int val;
	Trie* child[2] = {nullptr, nullptr};
	Trie() : val(0) {}
} *root;
void build(int val) {
	auto p = root;
	for (int j = 31; ~j; j--) {
		int bit = val >> j & 1;
		p = p->child[bit] ? p->child[bit] : p->child[bit] = new Trie();
	}
}
int query(int val) {
	auto p = root;
	int mask = 0;
	for (int j = 31; ~j; j--) {
		int bit = val >> j & 1;
		if (p->child[bit ^ 1]) {
			p = p->child[bit ^ 1];
			mask ^= 1 << j;
		} else p = p->child[bit];
	}
	return mask;
}
int n, ans, a[100001];
int main() {
	cin >> n;
	root = new Trie();
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		build(a[i]);
	}
	for (int i = 1; i <= n; i++) ans = max(ans, query(a[i]));
	cout << ans;
	return 0;
}

