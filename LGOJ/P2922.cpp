#include <bits/stdc++.h>
using namespace std;
struct Trie {
	int val, end;
	Trie* child[2] = {nullptr};
	Trie(): val(0), end(0) {}
} *root;
int n, m, k, x;
int main() {
	cin >> n >> m;
	root = new Trie;
	while (n--) {
		cin >> k;
		auto p = root;
		while (k--) {
			cin >> x;
			if (!p->child[x]) p->child[x] = new Trie;
			p = p->child[x];
			p->val++;
		}
		p->end++;
	}
	while (m--) {
		cin >> k;
		auto p = root;
		int f = 0, ans = 0;
		while (k--) {
			cin >> x;
			if (f) continue;
			if (!p->child[x]) {
				f = 1;
				continue;
			}
			p = p->child[x];
			ans += p->end;
		}
		cout << ans + (f ? 0 : p->val - p->end) << endl;
	}
	return 0;
}
