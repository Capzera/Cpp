#include<bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
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
int n, u, v, cnt, ind[N], vis[N];
vector<int> g[N];
queue<int> q;
int main() {
	n = read();
	for (int u = 1; u <= n; u++) {
		v = read();
		g[u].push_back(v);
		ind[v]++;
	}
	for (int i = 1; i <= n; i++) 
		if (ind[i] == 0) q.push(i);
	while (!q.empty()) {
		u = q.front(); q.pop();
		vis[u] = 1;
		cnt++;
		for (auto& v : g[u]) {
			if (vis[v]) continue;
			ind[v]--;
			if (ind[v] == 0) q.push(v);
		}
	}
	cout << n - cnt;
	return 0;
}
