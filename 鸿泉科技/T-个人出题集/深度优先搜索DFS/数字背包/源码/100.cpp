#include <bits/stdc++.h>
using namespace std;
int n, k;
int ans[10];
void dfs(int idx, int sum, int lst) {
	if (idx == k) {
		if (sum == n) {
			for (int i = 0; i < k; i++) cout << ans[i] << " ";
			cout << endl;
		}
		return;
	}
	for (int i = lst; i <= 9; i++) {
		if (sum + i > n) break;
		ans[idx] = i;
		dfs(idx + 1, sum + i, i + 1);
	}
}
int main() {
	cin >> n >> k;
	dfs(0, 0, 1);
	return 0;
}
