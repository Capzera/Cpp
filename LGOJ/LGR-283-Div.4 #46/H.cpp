#include <bits/stdc++.h>
using namespace std;
string s, s1, s2, tmp;
unordered_map<string, int> mp;
vector<int> g[501];
int q, cnt, id, e[501][501];
void init(string s) {
	s += ",";
	for (auto& x : s) {
		if (!isalpha(x)) {
			if (tmp.empty()) continue;
			mp[tmp] = ++cnt;
			tmp = "";
		} else tmp += x;	
	}
	for (auto& x : s) {
		if (isalpha(x)) {
			if (tmp.empty()) continue;
			id++;
			if (tmp == "<=" || tmp == "<=>") g[id + 1].push_back(id);
			if (tmp == "=>" || tmp == "<=>") g[id].push_back(id + 1);
			tmp = "";
		} else tmp += x;
	}
}
void dfs(int rt, int u, int fa) {
	if (e[rt][u]) return;
	e[rt][u] = 1;
	for (auto& v : g[u]) {
		if (v == fa) continue;
		dfs(rt, v, u);
	}
}
int main() {
	cin >> s;
	init(s);
	for (int u = 1; u <= cnt; u++) dfs(u, u, -1);
	cin >> q;
	while (q--) {
		cin >> s1 >> s2;
		if (!mp.count(s1) || !mp.count(s2)) {
			cout << (s1 == s2 ? "Yes\n" : "No\n");
			continue;
		}
		cout << (e[mp[s1]][mp[s2]] ? "Yes\n" : "No\n");
	}
	return 0;
}





下午自行选择
