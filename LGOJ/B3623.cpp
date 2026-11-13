#include <bits/stdc++.h>
using namespace std;
int n, k, ans[11], vis[11];
void dfs(int i) {
	if (i == k) {
		for (int j = 0; j < k; j++) printf("%d ", ans[j]);
		puts("");
		return;
	}
	for (int j = 1; j <= n; j++) {
		if (vis[j]) continue;
		ans[i] = j;
		vis[j] = 1;
		dfs(i + 1);
		vis[j] = 0;
	}
}
int main() {
	cin >> n >> k;
	dfs(0);
	return 0;
}
