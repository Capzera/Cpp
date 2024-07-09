#include <bits/stdc++.h>
using namespace std;
int n, a[11], ans[11], vis[11];
void dfs(int idx) {
	if (idx == n) {
		for (int i = 0; i < n; i++) {
			cout << ans[i] << " ";
		}
		cout << endl;
		return;
	}
	for (int i = 0; i < n; i++) {
		if (vis[i]) continue;
		if (i > 0 && a[i] == a[i - 1] && vis[i - 1] == false) continue;
		vis[i] = 1;
		ans[idx] = a[i];
		dfs(idx + 1);
		vis[i] = 0;
	}
}
int main() {
	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> a[i];
	}
	sort(a.begin(), a.end());
	dfs(0);
	return 0;
}
