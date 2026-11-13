#include <bits/stdc++.h>
using namespace std;
int n, t, vis[11], cnt[11][11];
string s1, s2;
int solve() {
	cin >> n >> s1 >> s2;
	memset(cnt, 0, sizeof(cnt));
	memset(vis, 0, sizeof(vis));
	vector<int> num, f(1024, -1);
	for (int i = 0; i < n; i++) {
		cnt[s1[i] - '0'][s2[i] - '0']++;
		vis[s1[i] - '0'] = 1;
	}
	for (int x = 0; x <= 9; x++) {
		if (!vis[x]) continue;
		num.push_back(x);
	}
	f[0] = 0;
	for (auto& x : num) {
		vector<int> g(1024, -1); // 当前一行
		for (int mask = 0; mask < 1 << 10; mask++) {
			if (f[mask] == -1) continue;
			for (int i = 0; i < 10; i++) {
				if (mask >> i & 1) continue;
				g[mask | 1 << i] = max(g[mask | 1 << i], f[mask] + cnt[x][i]);
			}
		}
		f = g;
	}
	return *max_element(f.begin(), f.end());
}

int main() {
	cin >> t;
	while (t--) cout << solve() << '\n';
	return 0;
}

001001000 + 
