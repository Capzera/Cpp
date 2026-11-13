#include <bits/stdc++.h>
using namespace std;
const int N = 1e3 + 5;
int n, a[N], f[N], col[N];
int t1, t2, s1[N], s2[N];
string ans;
vector<int> g[N];
int dfs(int u, int color) {
	col[u] = color;
	for (auto& v : g[u]) {
		if (!col[v]) {
			if (!dfs(v, 3 - color)) return 0;
		} else if (col[v] == color) return 0;
	}
	return 1;
}
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	f[n + 1] = 0x3f3f3f3f;
	for (int j = n; j; j--) {
		f[j] = min(f[j + 1], a[j]);
	}
	for (int i = 1; i < n; i++) {
		for (int j = i + 1; j <= n; j++) {
			if (f[j + 1] < a[i] && a[i] < a[j]) {
				g[i].push_back(j);
				g[j].push_back(i);
			}
		}
	}
	for (int i = 1; i <= n; i++) {
		if (col[i]) continue;
		if (!dfs(i, 1)) return puts("0"), 0;
	}
	for (int x = 1, i = 1; x <= n;) {
		if (i <= n && col[i] == 1 && (!t1 || a[i] < s1[t1])) {
			s1[++t1] = a[i++];
			ans += "a";
		} else if (t1 && s1[t1] == x) {
			ans += "b";
			x++, t1--;
		} else if (i <= n && col[i] == 2 && (!t2 || a[i] < s2[t2])) {
			s2[++t2] = a[i++];
			ans += "c";
		} else if (t2 && s2[t2] == x) {
			ans += "d";
			x++, t2--;
		}
	}
	for (auto& x : ans) cout << x << " ";
	return 0;
}
