#include <bits/stdc++.h>
using namespace std;
int n, ans[31], vis[31];
void dfs(int idx) { // 正在取第idx张牌
	if (idx == n) {
		for (int i = 0; i < n; i++) printf("%5d", ans[i]);
		cout << endl;
		return;
	}
	for (int i = 1; i <= n; i++) {
		if (vis[i]) continue;
		ans[idx] = i;
		vis[i] = 1;
		dfs(idx + 1);
		vis[i] = 0;
	}
}
int main() {
	cin >> n;
	dfs(0);
	return 0;
}
