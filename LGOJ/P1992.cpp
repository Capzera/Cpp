#include<bits/stdc++.h>
using namespace std;
const int N = 1e3 + 10, M = 1e4 + 10, p = 9997;
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
int n, m, u, v, cnt, ind[N], vis[N];
long long k;
vector<int> g[N];
queue<int> q;
long long ksm(long long a, long long b) {
	long long ans = 1;
	while (b) {
		if (b & 1) ans = a * ans % p;
		b >>= 1;
		a = a * a % p;
	}
	return ans;
}
int main() {
	n = read(), m = read(), k = read();
	for (int i = 1; i <= m; i++) {
		u = read(), v = read();
		g[u].push_back(v);
		ind[v]++;
	}
	for (int i = 1; i <= n; i++) 
		if (!ind[i]) q.push(i);
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
	if (n - cnt == 0) {
		puts("Yes");
		cout << ksm(2ll, k);
	} else {
		puts("No");
		cout << k * k;
	}
	return 0;
}
