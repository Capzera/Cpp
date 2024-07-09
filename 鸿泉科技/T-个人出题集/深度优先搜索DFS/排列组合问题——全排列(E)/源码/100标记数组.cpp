#include <bits/stdc++.h>
using namespace std;
int n, ans[11], vis[11];
void dfs(int idx) {
	if (idx == n) {
		for (int i = 0; i < n; i++) {
			cout << ans[i] << " ";
		}
		cout << endl;
		return;
	}
	for (int i = 1; i <= n; i++) {
		if (vis[i]) continue;
		vis[i] = 1;
		ans[idx] = i;
		dfs(idx + 1);
		vis[i] = 0;
	}
}
int main() {
	cin >> n;
	dfs(0);
	return 0;
}
