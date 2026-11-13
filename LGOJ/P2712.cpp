#include<bits/stdc++.h>
using namespace std;
const int N = 510;
inline int read() {
	int ans = 0, f = 1;
	char ch = getchar();
	while (!isdigit(ch)) {
		if (ch == '-') f = -1;
		ch = getchar();
	}
	while (isdigit(ch)) {
		ans = (ans << 3) + (ans << 1) + ch - '0';
		ch = getchar();
	}
	return f * ans;
}
int n, m, u, v, cnt, ind[N], vis[N], mp[N], a[N];
vector<int> g[N];
queue<int> q;
int main() {
	n = read();
	for (int i = 1; i <= n; i++) {
		u = read(),  m = read();
		mp[u]++;
		a[i] = u;
		for (int j = 1; j <= m; j++) {
			v = read();
			g[u].push_back(v);
			ind[v]++;
		}
	}
	for (int i = 1; i <= n; i++) 
		if (ind[a[i]] == 0) q.push(a[i]);
	while (!q.empty()) {
		int u = q.front(); q.pop();
		vis[u] = 1;
		cnt++;
		for (auto& v : g[u]) {
			if (vis[v]) continue;
			ind[v]--;
			if (!ind[v] && mp[v]) q.push(v);
		}
	}
	if (n - cnt == 0) return puts("YES"), 0;
	cout << n - cnt;
	return 0;
}
