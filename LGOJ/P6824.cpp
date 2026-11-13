#include <bits/stdc++.h>
using namespace std;
struct Trie {
	int val;
	Trie* child[2] = {nullptr};
	Trie(): val(0) {}
} *root;
int n, k, x, ans;
void build(int val) {
	auto p = root;
	for (int i = 21; ~i; i--) {
		int x = val >> i & 1;
		if (!p->child[x]) p->child[x] = new Trie();
		p = p->child[x];
		p->val++; // 路径和
	}
}
int query(int x) { // 已知值为x，对01Trie树进行异或，返回多少个数会小于k
	int ans = 0, f = 1;
	auto p = root;
	for (int i = 21; ~i; i--) {
		int xi = x >> i & 1, ki = k >> i & 1;
		if (ki && p->child[xi]) ans += p->child[xi]->val;
		if (!p->child[xi ^ ki]) {
			f = 0;
			break;
		}
		p = p->child[xi ^ ki];
	}
	if (f) ans += p->val;
	return ans;
}
int main() {
	cin >> n >> k;
	root = new Trie();
	while (n--) {
		cin >> x;
		build(x);
	}
	for (int x = 0; x < 1 << 21; x++) ans = max(ans, query(x));
	cout << ans;
	return 0;
}
