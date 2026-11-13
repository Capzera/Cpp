#include <bits/stdc++.h>
using namespace std;
int n, x, y, u1, l1, u2, l2;
vector<int> g[30];
string s1, s2;
void reset(int u, int l) {
	for (int i = l + 1; i < g[u].size(); i++) {
		int v = g[u][i];
		g[v].push_back(v);
	}
	g[u].resize(l + 1);
}
void move(int f) {
	reset(u1, l1), g[u1].pop_back();
	if (f) reset(u2, l2);
	g[u2].push_back(x);
}

void pile(int f) {
	if (f) reset(u2, l2);
	for (int i = l1; i < g[u1].size(); i++) {
		int v = g[u1][i];
		g[u2].push_back(v);
	}
	g[u1].resize(l1);
}
int main() {
	cin >> n;
	for (int i = 0; i < n; i++) g[i].push_back(i);
	while (cin >> s1, s1 != "quit") {
		cin >> x >> s2 >> y;
		for (int u = 0; u < n; u++) {
			for (int i = 0; i < g[u].size(); i++) {
				int v = g[u][i];
				if (v == x) u1 = u, l1 = i;
				if (v == y) u2 = u, l2 = i;
			}
		}
		if (u1 == u2) continue;
		if (s1 == "move" && s2 == "onto") move(1);
		if (s1 == "move" && s2 == "over") move(0);
		if (s1 == "pile" && s2 == "onto") pile(1);
		if (s1 == "pile" && s2 == "over") pile(0);
	}
	for (int u = 0; u < n; u++) {
		cout << u << ": ";
		for (auto& v : g[u]) cout << v << " ";
		cout << '\n';
	}
	return 0;
}
