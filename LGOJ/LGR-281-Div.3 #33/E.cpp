#include <bits/stdc++.h>
using namespace std;
const int N = 1e6 + 10;
inline long long read() {
	long long ans = 0, f = 1;
	char ch = getchar();
	while (!isdigit(ch)) {
		if (ch == '-') f = -1;
		ch = getchar();
	}
	while (isdigit(ch)) {
		ans = ans * 10 + ch - '0';
		ch = getchar();
	}
	return ans * f;
}
inline void write(long long x) {
	if (x < 0) putchar('-'), x = -x;
	if (x > 9) write(x / 10);
	putchar(x % 10 + '0');
}
struct Trie {
	int val, flag;
	Trie *lt = nullptr, *rt = nullptr;
	Trie() : lt(nullptr), rt(nullptr) {}
} *root;
Trie* build(Trie* root, int val, int fg) {
	if (root) return root;
	Trie *nd = new Trie();
	nd->val = val;
	nd->flag = fg;
	return nd;
}
int t, n, idx, sz, ans, a[N];
int dfs(Trie* root) {
	if (root->flag) return 1;
	int ans = 0;
	if (root->lt) ans += dfs(root->lt);
	if (root->rt) ans += dfs(root->rt);
	if (ans == 2) ::ans++;
	return ans;
}
long long solve() {
	n = read();
	ans = 0;
	int mx = LLONG_MIN, mn = LLONG_MAX;
	root = new Trie();
	for (int i = 1; i <= n; i++) {
		a[i] = read();
		mx = max(mx, a[i]);
		mn = min(mn, a[i]);
	}
	if (n <= 500 && mx <= 512) {
		for (int i = 1; i <= n; i++) {
			for (int j = i + 1; j <= n; j++) {
				int flag = 1;
				for (int k = 1; k <= n; k++) {
					for (int x = 0; x <= 2 *mx; x++) {
						int sum = a[k] ^ x;
						if (sum > (a[i] ^ x) && sum < (a[j] ^ x)) {
							flag = 0;
							break;
						}
					}
					if (!flag) break;
				}
				ans += flag;
			}
		}
		return ans;
	}
	sz = log2(mx) + 1;
	Trie *nd = root;
	for (int i = 1; i <= n; i++) {
		for (int j = sz; ~j; j--) {
			int x = a[i] >> j & 1;
			int sp = !j ? 1 : 0;
			if (x == 0) nd = build(nd->lt, 0, sp);
			else nd = build(nd->rt, 1, sp);
		}
	}
	dfs(root);
	return ans;
}
int main() {
	t = read();
	while (t--) cout << solve() << '\n';
	return 0;
}

