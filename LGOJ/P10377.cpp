#include <bits/stdc++.h>
using namespace std;
int n, a[13], b[13], vis[13], ans[13], mx = INT_MAX;
int cal() {
	int sum = 1;
	for (int i = 2; i <= n; i++) {
		sum += 1 + max(a[ans[i]], b[ans[i - 1]]);
	}
	return sum;
}
void dfs(int i) {
	if (i == n + 1) {
		mx = min(mx, cal());
		return;
	}
	for (int j = 1; j <= n; j++) {
		if (vis[j]) continue;
		vis[j] = 1;
		ans[i] = j;
		dfs(i + 1);
		vis[j] = 0;
	}
}
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= n; i++) cin >> b[i];
	dfs(1);
	cout << mx;
	return 0;
}
