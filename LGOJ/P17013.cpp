#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
int n, ans, tr[N][2];
int dfs(int u) {
	if (!u) return 0;
	int lt = dfs(tr[u][0]);
	int rt = dfs(tr[u][1]);
	if (lt != -1 && lt == rt) {
		ans++;
		return lt + 1;
	}
	return -1;
}
int main() {
	cin >> n;
	for (int u = 1; u <= n; u++) {
		cin >> tr[u][0] >> tr[u][1];
	}
	dfs(1);
	cout << ans;
	return 0;
}
