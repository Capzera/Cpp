#include <bits/stdc++.h>
using namespace std;
struct info {
	long long r, v, id;
	string s;
	bool operator < (const struct info& rhs) const {
		if (r != rhs.r) return r < rhs.r;
		return id < rhs.id;
	}
} a[110001];
struct Trie {
	long long val;
	Trie* child[26] = {nullptr};
	Trie() : val(0) {}
} *root;
void build(string s, int v) {
	auto p = root;
	for (auto& ch : s) {
		int x = ch - 'a';
		p = p->child[x] ? p->child[x] : p->child[x] = new Trie();
		p->val += v; // val表示经过该点的价值总和
	}
}
long long query(string s) {
	auto p = root;
	for (auto& ch : s) {
		int x = ch - 'a';
		if (!p->child[x]) p->child[x] = new Trie(); // 建树和查询被打乱顺序了，所以需要在查询时建树，保证正确性
		p = p->child[x];
	}
	return p->val; // 最深处价值
}
string s;
long long n, m, r, v, ans[100001];
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		cin >> a[i].r >> a[i].v >> a[i].s;
	}
	for (int i = 1; i <= m; i++) {
		cin >> a[i + n].r >> a[i + n].s;
		a[i + n].id = i;
	}
	sort(a + 1, a + n + m + 1);
	root = new Trie();
	for (int i = 1; i <= n + m; i++) {
		if (a[i].v) build(a[i].s, a[i].v);
		if (a[i].id) ans[a[i].id] = query(a[i].s);
	}
	for (int i = 1; i <= m; i++) cout << ans[i] << endl;
	return 0;
}
