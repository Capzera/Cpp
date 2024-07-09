#include <bits/stdc++.h>
using namespace std;
int n;
int ans[100], vis[100];
void dfs(int now) {
	for (int i = 1; i <= n; i++) {
		if (vis[i]) continue;
		vis[i] = 1;
		ans[i] = i;
		for (int i = 0; i < ans.size(); i++) {
			cout << ans[i] << " ";
		}
		cout << endl;
		dfs(i + 1);
		vis[i] = 0;

	}
}
int main() {
	cin >> n;
	dfs(1);
	return 0;
}

