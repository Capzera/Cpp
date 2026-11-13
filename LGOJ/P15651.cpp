#include <bits/stdc++.h>
using namespace std;
const int N = 255;
int xx, t, n, m, flag, a[N], b[N], nxt[N];
vector<int> ans;
unordered_set<string> vis;
int dfs1(string s) {
	int len = s.size();
	if (len == m) {
		for (int i = 0; i < m; i++)
			if (s[i] != i)
				return 0;
		return 1;
	}
	if (vis.count(s)) return 0;
	vis.insert(s);
	if (s[0] == s[1]) { // 前两个可以合并成同一个上块
		ans.push_back(1);
		if (dfs1(s.substr(2) + s[0])) return 1;
		ans.pop_back();
	}
	if (s[len - 1] == s[len - 2]) { // 最后两个可以合并成同一个下块
		ans.push_back(2);
		if (dfs1(s[len - 1] + s.substr(0, len - 2))) return 1;
		ans.pop_back();
	}
	return 0;
}
void dfs2(int st, int x, int y) {
	if (flag) return;
	if (y == m) {
		if (x != n) return;
		string s = "";
		for (int i = 0; i < n; i++) s += (char)nxt[i];
		ans.clear();
		if (dfs1(s)) flag = 1;
	}
	int sum = 0;
	for (int i = x; i < n; i++) {
		sum ^= a[(st + i) % n];
		nxt[(st + i) % n] = y;
		if (sum == b[y]) dfs2(st, i + 1, y + 1);
		if (flag) return;
	}
}
void solve() {
	cin >> n >> m;
	for (int i = 0; i < n; i++) cin >> a[i];
	for (int i = 0; i < m; i++) cin >> b[i];
	flag = 0;
	vis.clear();
	for (int i = 0; i < n; i++) {
		dfs2(i, 0, 0);
		if (flag) break;
	}
	if (!flag) {
		cout << "No\n";
		return;
	}
	cout << "Yes\n";
	for (auto& x : ans) cout << x << " ";
	cout << "\n";
}
int main() {
	cin >> xx >> t;
	while (t--) solve();
	return 0;
}
