#include <bits/stdc++.h>
using namespace std;
int n, k, ans[31];
void dfs(int idx, int st) { // 正在取第idx张牌
	if (idx == k) {
		for (int i = 0; i < k; i++) printf("%3d", ans[i]);
		cout << endl;
		return;
	}
	for (int i = st; i <= n; i++) {
		ans[idx] = i;
		dfs(idx + 1, i + 1);
	}
}
int main() {
	cin >> n >> k;
	dfs(0, 1);
	return 0;
}
