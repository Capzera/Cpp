#include <bits/stdc++.h>
using namespace std;
struct Trie {
	int val;
	unordered_map<char, Trie*> child;
	Trie(int v = 0): val(v) {}
};
int n, ans;
string s;
int main() {
	cin >> n;
	Trie *root = new Trie();
	while (n--) {
		cin >> s;
		auto p = root;
		int cnt = 0;
		for (auto& ch : s) {
			if (!p->child.count(ch)) {
				p->child[ch] = new Trie();
			}
			p = p->child[ch];
			cnt += p->val;	
		}
		p->val++;
		ans = max(ans, cnt + 1);
	}
	cout << ans;
	return 0;
}
